from pathlib import Path
import sys
import shutil


class PreparedDynamicLibTool:

    def __init__(self, install_root: Path, output_root: Path):
        self._install_root = install_root
        self._output_root = output_root

    def copy_dlib(self, dlib_path: str) -> None:
        src_file: Path = self._install_root / dlib_path
        if not src_file.exists():
            return
        dst_file: Path = self._output_root / src_file.name
        print(f"PreparedDynamicLibTool copy_dlib [{src_file}] => [{dst_file}]")
        shutil.copy(src=src_file, dst=dst_file)


def move_dynamic_libs(install_root: Path, output_root: Path):
    if sys.platform != 'win32':
        return
    tool = PreparedDynamicLibTool(install_root=install_root, output_root=output_root)
    tool.copy_dlib('zlib/bin/zlib.dll')
    tool.copy_dlib('zlib/bin/zlibd.dll')
    tool.copy_dlib('liblzma/bin/liblzma.dll')
    tool.copy_dlib('liblzma/bin/liblzmad.dll')
    tool.copy_dlib('libarchive/bin/archive.dll')
    tool.copy_dlib('libarchive/bin/archived.dll')


def main():
    print(f'Update dynamic libs, args: [{sys.argv}]')
    move_dynamic_libs(
        install_root=Path(sys.argv[1]).absolute(),
        output_root=Path(sys.argv[2]).absolute()
    )


if __name__ == '__main__':
    main()
