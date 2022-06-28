import sys
from pathlib import Path

from third_lib_tool.util.build_config import BuildConfig, LinkType
from third_lib_tool.util.build_context import BuildContext

from third_lib_tool import lib_zlib
from third_lib_tool import lib_libpng
from third_lib_tool import lib_boost
from third_lib_tool import lib_openssl
from third_lib_tool import lib_protobuf
from third_lib_tool import lib_freetype
from third_lib_tool import lib_libarchive
from third_lib_tool import repo_thirdlibs

if sys.platform == 'win32':
    from third_lib_tool import lib_nasm


NEED_ZLIB: bool = True
NEED_LIBPNG: bool = True
NEED_BOOST: bool = True
NEED_PROTOBUF: bool = False
NEED_NASM: bool = False and (sys.platform == 'win32')
NEED_OPENSSL: bool = False
NEED_FREETYPE: bool = True
NEED_LIBARCHIVE: bool = True


def main():
    cmake_source_dir = Path(sys.argv[1]).resolve()
    print(f'Preparing libs, cmake_source_dir: [{cmake_source_dir}]')
    prepare_dir = cmake_source_dir / 'prepare'
    thirdlib_repo_dir = prepare_dir / 'thirdlibs_repo'
    repo_thirdlibs.download(clone_dir=thirdlib_repo_dir)

    build_config = BuildConfig()
    build_config.runtime.linkType = LinkType.Dynamic
    build_config.runtime.useDebug = False
    build_config.lib.linkType = LinkType.Dynamic
    build_config.lib.useDebug = False

    build_context = BuildContext(
        base_repo_dir=thirdlib_repo_dir,
        base_extract_dir=prepare_dir / 'source',
        base_install_dir=prepare_dir / 'install'
    )

    zlib_base_dir: Path = cmake_source_dir / 'prepare' / 'zlib'
    if NEED_ZLIB:
        lib_zlib.prepare(
            context=build_context,
            config=build_config
        )
    if NEED_LIBPNG:
        lib_libpng.prepare(
            thirdlibs_repo_dir=thirdlib_repo_dir,
            base_dir=cmake_source_dir / 'prepare' / 'libpng',
            zlib_base_dir=zlib_base_dir / 'install'
        )
    if NEED_BOOST:
        lib_boost.prepare(
            thirdlibs_repo_dir=thirdlib_repo_dir,
            base_dir=cmake_source_dir / 'prepare' / 'boost'
        )
    if NEED_PROTOBUF:
        lib_protobuf.prepare(
            thirdlibs_repo_dir=thirdlib_repo_dir,
            base_dir=cmake_source_dir / 'prepare' / 'protobuf'
        )
    if NEED_NASM:
        lib_nasm.unpack(
            archive_file_dir=thirdlib_repo_dir / 'nasm',
            extract_dir=cmake_source_dir / 'prepare' / 'nasm'
        )
    if NEED_OPENSSL:
        nasm_exe_path: Path = None
        if NEED_NASM:
            nasm_exe_path = cmake_source_dir / 'prepare' / 'nasm' / 'nasm'
        lib_openssl.prepare(
            thirdlibs_repo_dir=thirdlib_repo_dir,
            base_dir=cmake_source_dir / 'prepare' / 'openssl',
            nasm_exe_dir=nasm_exe_path
        )
    if NEED_FREETYPE:
        lib_freetype.prepare(
            thirdlibs_repo_dir=thirdlib_repo_dir,
            base_dir=cmake_source_dir / 'prepare' / 'freetype'
        )
    if NEED_LIBARCHIVE:
        lib_libarchive.prepare(
            thirdlibs_repo_dir=thirdlib_repo_dir,
            base_dir=cmake_source_dir / 'prepare' / 'libarchive',
            zlib_base_dir=zlib_base_dir / 'install'
        )


if __name__ == '__main__':
    main()
