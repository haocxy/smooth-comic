import sys
import argparse

from pathlib import Path

from third_lib_tool.util.build_config import BuildConfig, LinkType
from third_lib_tool.util.build_context import BuildContext

from third_lib_tool import repo_thirdlibs

from third_lib_tool import lib_zlib
from third_lib_tool import lib_liblzma
from third_lib_tool import lib_libarchive
from third_lib_tool import lib_boost


NEED_ZLIB: bool = True
NEED_LIBLZMA: bool = True
NEED_LIBARCHIVE: bool = True

NEED_BOOST: bool = True


def main():
    parser = argparse.ArgumentParser(description='Prepare dependencies')
    parser.add_argument('-s', '--source_dir', help='use CMAKE_SOURCE_DIR')
    parser.add_argument('-p', '--prepare_dir', help='Output directory for prepared libs')
    parser.add_argument('-r', '--repo_dir', help='Directory of repo in which can find libs')
    parser.add_argument('-c', '--config', choices=['debug', 'release'])
    args = parser.parse_args()

    print(f'args: {args}', flush=True)

    cmake_source_dir = Path(args.source_dir)
    print(f'Preparing libs, cmake_source_dir: [{cmake_source_dir}]')
    prepare_dir = Path(args.prepare_dir)
    thirdlib_repo_dir: Path = Path(args.repo_dir)
    repo_thirdlibs.download(clone_dir=thirdlib_repo_dir)

    build_config = BuildConfig()
    build_config.runtime.linkType = LinkType.Dynamic
    build_config.runtime.useDebug = False
    build_config.lib.linkType = LinkType.Dynamic
    build_config.lib.useDebug = (args.config == 'debug')

    build_context = BuildContext(
        base_repo_dir=thirdlib_repo_dir,
        base_extract_dir=prepare_dir / 'source',
        base_install_dir=prepare_dir / 'install'
    )

    if NEED_ZLIB:
        lib_zlib.prepare(
            context=build_context,
            config=build_config
        )
    if NEED_LIBLZMA:
        lib_liblzma.prepare(
            context=build_context,
            config=build_config
        )
    if NEED_LIBARCHIVE:
        lib_libarchive.prepare(
            context=build_context,
            config=build_config
        )
    if NEED_BOOST:
        lib_boost.prepare(
            context=build_context,
            config=build_config
        )


if __name__ == '__main__':
    main()
