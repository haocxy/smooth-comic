import os
import shutil
import sys
from pathlib import Path

from third_lib_tool.util.build_config import BuildConfig
from third_lib_tool.util.build_context import BuildContext
from third_lib_tool.util.compress_util import smart_extract


def copy_built(from_file: Path, to_file: Path):
    if not from_file.exists():
        raise Exception(f'cannot copy non existed file: [{from_file}]')
    os.makedirs(to_file.parent, exist_ok=True)
    shutil.copy(src=from_file, dst=to_file)


def build_and_install_win(source_dir: Path, install_dir: Path, config: BuildConfig):
    old_dir: str = os.getcwd()

    sln_dir: Path = source_dir / 'windows' / 'vs2019'
    os.chdir(sln_dir)

    sln_config = 'Debug' if config.lib.useDebug else 'Release'
    sln_platform = 'x64'
    sln_toolset = 'v143'

    build_cmd: str = ''
    build_cmd += 'msbuild xz_win.sln -t:build'
    build_cmd += f' -p:configuration={sln_config}'
    build_cmd += f' -p:platform={sln_platform}'
    build_cmd += f' -p:PlatformToolset={sln_toolset}'

    print(f'CMD: {build_cmd}')
    os.system(build_cmd)

    built_dir: Path = sln_dir / sln_config / sln_platform / 'liblzma_dll'
    copy_built(built_dir / 'liblzma.dll', install_dir / 'bin' / 'liblzma.dll')
    copy_built(built_dir / 'liblzma.lib', install_dir / 'lib' / 'liblzma.lib')

    shutil.copytree(
        src=source_dir / 'src' / 'liblzma' / 'api',
        dst=install_dir / 'include',
        dirs_exist_ok=True
    )

    os.chdir(old_dir)


def build_and_install_android(context: BuildContext, install_dir: Path, config: BuildConfig):
    smart_extract(
        archive=context.find_newest_in_repo(f'xz/xz-?.?.?-android-{config.lib.androidAbi}.tar.gz'),
        dest_dir=install_dir
    )


def build_and_install(context: BuildContext, install_dir: Path, config: BuildConfig, lib_name: str):
    if config.lib.isForAndroid:
        build_and_install_android(
            context=context,
            install_dir=install_dir,
            config=config
        )
    elif sys.platform == 'win32':
        source_dir = smart_extract(
            archive=context.find_newest_in_repo('xz/xz-?.?.?.tar.gz'),
            dest_dir=context.get_extract_dir(f'{lib_name}/xz.tmp2')
        )
        build_and_install_win(
            source_dir=source_dir,
            install_dir=install_dir,
            config=config
        )
    else:
        raise Exception('Unimplemented logic')


def prepare(context: BuildContext, config: BuildConfig):
    lib_name = 'liblzma'
    if not context.need_build(lib_name):
        return

    build_and_install(
        context=context,
        install_dir=context.get_install_dir(lib_name),
        config=config,
        lib_name=lib_name
    )
