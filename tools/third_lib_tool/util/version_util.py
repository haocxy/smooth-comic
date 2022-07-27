import os
import re
from dataclasses import dataclass
from pathlib import Path
from re import Pattern, Match
from typing import Optional


def convert_name_template_part_to_regex_str(part: str) -> str:
    if part is None:
        return '([0-9]+)'
    else:
        result: str = ''
        for c in part:
            if c in {'.', ':', '(', ')', '-', '+', '*'}:
                result += '\\'
                result += c
            else:
                result += c
        return result


def convert_name_template_parts_to_regex_str(parts: list[Optional[str]]) -> str:
    result: str = ''
    for part in parts:
        result += convert_name_template_part_to_regex_str(part)
    return result


def split_name_template_to_parts(name_template: str) -> list[Optional[str]]:
    part: str = ''
    parts: list[Optional[str]] = list[Optional[str]]()
    for c in name_template:
        if c != '?':
            part += c
        else:
            if len(part) != 0:
                parts.append(part)
            part = ''
            parts.append(None)
    if len(part) != 0:
        parts.append(part)
    return parts


def mk_pattern_for_name_template(name_template: str) -> re.Pattern:
    parts: list[Optional[str]] = split_name_template_to_parts(name_template)
    reg_content: str = convert_name_template_parts_to_regex_str(parts)
    return re.compile(reg_content)


@dataclass
class LibVersion:
    parts: list[int]

    def newer_than(self, version) -> bool:
        if len(self.parts) != len(version.parts):
            raise Exception('Part count of LibVersion should be same')
        count: int = len(self.parts)
        for i in range(count):
            if self.parts[i] > version.parts[i]:
                return True
        return False

    def __str__(self) -> str:
        result: str = self.__class__.__name__
        is_first: bool = True
        result += '('
        for part in self.parts:
            if not is_first:
                result += ','
            result += str(part)
            is_first = False
        result += ')'
        return result


def get_lib_version_by_name_template(name_template: re.Pattern, name: str) -> Optional[LibVersion]:
    match: re.Match = name_template.fullmatch(name)
    if match is None:
        return None
    groups: tuple = match.groups()
    if groups is None:
        return None
    return LibVersion([int(group) for group in groups])


PAT_MULTI_VOLUMN: Pattern = re.compile(r'^(.*)\.\d+$')


def try_find_multi_volumn_part(name: str) -> str:
    match: Match = PAT_MULTI_VOLUMN.match(name)
    if match is not None:
        return match.group(1)
    else:
        return name


def find_newest_lib(directory: Path, name_template: str, is_multi_volumn: bool = False) -> Optional[Path]:
    if not directory.is_dir():
        raise Exception('Parameter [directory] should be directory')
    pat: re.Pattern = mk_pattern_for_name_template(name_template)
    newest_libver: Optional[LibVersion] = None
    newest_file: Optional[str] = None
    for name in os.listdir(directory):
        if is_multi_volumn:
            name = try_find_multi_volumn_part(str(name))
        libver: LibVersion = get_lib_version_by_name_template(pat, str(name))
        if libver is None:
            continue
        if newest_libver is None or libver.newer_than(newest_libver):
            newest_libver = libver
            newest_file = str(name)
    if newest_file is None:
        return None
    else:
        return directory / newest_file
