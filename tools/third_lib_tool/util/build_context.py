from pathlib import Path
from typing import Union

from third_lib_tool.util.version_util import find_newest_lib


class BuildContext:

    def __init__(self, base_repo_dir: Path, base_extract_dir: Path, base_install_dir: Path):
        self.base_repo_dir = base_repo_dir
        self.base_extract_dir = base_extract_dir
        self.base_install_dir = base_install_dir

    def get_repo_path(self, relative_path: Union[str | Path]) -> Path:
        return self.base_repo_dir / relative_path

    def get_extract_dir(self, lib_name: Union[str | Path]) -> Path:
        return self.base_extract_dir / lib_name

    def get_install_dir(self, lib_name: Union[str | Path]) -> Path:
        return self.base_install_dir / lib_name

    def find_newest_in_repo(self, relative_path: Union[str | Path]) -> Path:
        full_path: Path = self.get_repo_path(relative_path)
        return find_newest_lib(full_path.parent, full_path.name)

    def need_build(self, lib_name: str) -> bool:
        if (self.base_install_dir / lib_name).exists():
            print(f'Lib [{lib_name}] already built, skip it', flush=True)
            return False
        else:
            return True

