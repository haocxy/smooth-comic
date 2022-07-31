import os
import subprocess
from pathlib import Path
from typing import Optional

from third_lib_tool.util.build_config import BuildConfig


def empty_str(s: Optional[str]) -> bool:
    return s is None or len(s) == 0


def mk_cmake_prefix_path_str(cmake_prefix_path: Optional[list[str]]) -> str:
    if cmake_prefix_path is None or len(cmake_prefix_path) == 0:
        return ''
    result: str = ''
    is_first: bool = True
    for path in cmake_prefix_path:
        if not is_first:
            result += ';'
        result += path
        is_first = False
    return result


def objlist_to_strlist(objlist: list) -> list[str]:
    result: list[str] = []
    for item in objlist:
        result.append(str(item))
    return result


def cmake_build_and_install(
        source_dir: Path,
        build_dir: Path,
        install_dir: Path,
        build_config: BuildConfig,
        other_params: Optional[list[str]] = None,
        install_retry_times: int = 1,
        cmake_prefix_path: Optional[list[str]] = None):

    if not build_dir.exists():
        os.makedirs(build_dir)

    cmake_exe: str = 'cmake'
    if build_config.cmakeCommand is not None:
        cmake_exe = build_config.cmakeCommand

    generate_args: list[str] = []

    generate_args += [cmake_exe, '-S', source_dir, '-B', build_dir]

    if build_config.cmakeGenerator is not None:
        generate_args += [f'-DCMAKE_GENERATOR={build_config.cmakeGenerator}']

    if build_config.cmakeMakeProgram is not None:
        generate_args += [f'-DCMAKE_MAKE_PROGRAM={build_config.cmakeMakeProgram}']

    generate_args += ['-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH']
    generate_args += ['-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH']
    generate_args += ['-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH']

    if build_config.lib.cmakeToolchainFile is not None:
        generate_args += [f'-DCMAKE_TOOLCHAIN_FILE={build_config.lib.cmakeToolchainFile}']

    if build_config.lib.isForAndroid:
        if build_config.lib.androidAbi is not None:
            generate_args += [f'-DANDROID_ABI={build_config.lib.androidAbi}']
        if build_config.lib.androidPlatform is not None:
            generate_args += [f'-DANDROID_PLATFORM={build_config.lib.androidPlatform}']

    generate_args += [f'-DCMAKE_BUILD_TYPE={"Debug" if build_config.lib.useDebug else "Release"}']

    generate_args += [f'-DCMAKE_INSTALL_PREFIX={install_dir}']

    cmake_prefix_path_str: str = mk_cmake_prefix_path_str(cmake_prefix_path)
    if not empty_str(cmake_prefix_path_str):
        generate_args += [f'-DCMAKE_PREFIX_PATH={cmake_prefix_path_str}']

    if other_params is not None:
        generate_args += other_params

    generate_str_args = objlist_to_strlist(generate_args)
    print(f'CMake Configure CMD: {generate_str_args}', flush=True)
    subprocess.run(generate_str_args)

    job_count: int = max(1, os.cpu_count() - 1)

    build_and_install_args: list[str] = []
    build_and_install_args += [cmake_exe, '--build', build_dir, '-t', 'install', '--config']
    build_and_install_args += ['Debug' if build_config.lib.useDebug else 'Release']

    build_and_install_args += ['-j', job_count]

    build_and_install_str_args = objlist_to_strlist(build_and_install_args)

    for i in range(install_retry_times):
        print(f'CMake Install CMD: {build_and_install_str_args}', flush=True)
        subprocess.run(build_and_install_str_args)
