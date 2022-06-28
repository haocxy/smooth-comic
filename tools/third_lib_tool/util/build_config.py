from dataclasses import dataclass
from enum import Enum


class BuildType(Enum):
    Debug = 1
    Release = 2


class LinkType(Enum):
    Static = 1
    Dynamic = 2


@dataclass
class RuntimeInfo:
    linkType: LinkType = None
    useDebug: bool = None


@dataclass
class LibInfo:
    linkType: LinkType = None
    useDebug: bool = None


@dataclass
class BuildConfig:
    runtime: RuntimeInfo = RuntimeInfo()
    lib: LibInfo = LibInfo()

