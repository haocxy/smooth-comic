from dataclasses import dataclass, field
from enum import Enum
from typing import Optional


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
    cmakeToolchainFile: Optional[str] = None
    isForAndroid: bool = False
    androidAbi: Optional[str] = None
    androidPlatform: Optional[str] = None


@dataclass
class BuildConfig:
    cmakeCommand: Optional[str] = None
    cmakeGenerator: Optional[str] = None
    cmakeMakeProgram: Optional[str] = None
    runtime: RuntimeInfo = field(
        default_factory=lambda: RuntimeInfo()
    )
    lib: LibInfo = field(
        default_factory=lambda: LibInfo()
    )


