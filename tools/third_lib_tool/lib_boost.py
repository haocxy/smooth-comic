import os
import sys
from pathlib import Path

from third_lib_tool import logs
from third_lib_tool.util.build_config import BuildConfig, LinkType
from third_lib_tool.util.build_context import BuildContext
from third_lib_tool.util.compress_util import smart_unpack, smart_extract

BOOST_LIB_NAME: str = 'boost_1_78_0'

REQUIRED_BOOST_MODULES = [
    'headers'
]


def unpack(unpack_dir: Path, archive_dir: Path) -> Path:
    return smart_unpack(
        archive_file=archive_dir / f'{BOOST_LIB_NAME}.7z',
        unpack_dir=unpack_dir,
        base_name='boost.src'
    )


def get_boost_booststrap_cmd_name() -> str:
    if sys.platform == 'win32':
        return 'bootstrap'
    else:
        return 'sh bootstrap.sh'


def get_boost_b2_cmd_name() -> str:
    if sys.platform == 'win32':
        return 'b2'
    else:
        return './b2'


def make_boost_b2_install_cmd_line(install_dir: Path, config: BuildConfig, modules: list) -> str:
    b2_cmd_name: str = get_boost_b2_cmd_name()
    cmdline: str = ''
    cmdline += b2_cmd_name
    cmdline += f' --prefix=\"{install_dir}\"'
    for module in modules:
        cmdline += f' --with-{module}'

    cmdline += ' variant='
    if config.lib.useDebug:
        cmdline += 'debug'
    else:
        cmdline += 'release'

    cmdline += ' link=static'

    cmdline += ' runtime-link='
    if config.runtime.linkType == LinkType.Static:
        cmdline += 'static'
    else:
        cmdline += 'shared'

    cmdline += ' install'
    return cmdline


def install(source_dir: Path, install_dir: Path, config: BuildConfig):
    bootstrap_cmd_name: str = get_boost_booststrap_cmd_name()
    install_cmdline: str = make_boost_b2_install_cmd_line(
        install_dir=install_dir,
        config=config,
        modules=REQUIRED_BOOST_MODULES)
    print(f'Boost b2 cmd: {install_cmdline}')
    old_dir: str = os.getcwd()
    os.chdir(source_dir)
    os.system(f'{bootstrap_cmd_name}')
    os.system(install_cmdline)
    os.chdir(old_dir)


def prepare(context: BuildContext, config: BuildConfig):
    lib_name: str = 'boost'
    if not context.need_build(lib_name):
        return

    extract_dir = context.get_extract_dir(lib_name)
    if extract_dir.exists():
        return

    smart_extract(
        archive=context.find_newest_in_repo('boost/boost_?_?_?.7z', is_multi_volumn=True),
        dest_dir=extract_dir
    )
