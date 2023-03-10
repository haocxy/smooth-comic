from pathlib import Path

from third_lib_tool.util.cmake_util import cmake_build_and_install
from third_lib_tool.util.compress_util import smart_extract

from third_lib_tool.util.build_context import BuildContext
from third_lib_tool.util.build_config import BuildConfig


def install(source_dir: Path, build_dir: Path, install_dir: Path, config: BuildConfig):
    cmake_build_and_install(
        source_dir=source_dir,
        build_dir=build_dir,
        install_dir=install_dir,
        build_config=config
    )


def prepare(context: BuildContext, config: BuildConfig):
    lib_name: str = 'zlib'
    if not context.need_build(lib_name):
        return

    source_dir = smart_extract(
        archive=context.find_newest_in_repo('zlib/zlib-?.?.?.tar.gz'),
        dest_dir=context.get_extract_dir(lib_name)
    )

    cmake_build_dir_name: str = 'build-debug' if config.lib.useDebug else 'build-release'
    cmake_build_dir: Path = source_dir / cmake_build_dir_name
    cmake_install_dir: Path = context.get_install_dir(lib_name)

    install(
        source_dir=source_dir,
        build_dir=cmake_build_dir,
        install_dir=cmake_install_dir,
        config=config
    )
