try:
    from pybind11.setup_helpers import Pybind11Extension
except ImportError:
    from setuptools import Extension as Pybind11Extension

from pathlib import Path
from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

source_dir = str(Path("..").resolve().absolute())
sources = [
    f"{source_dir}/example.cpp",
    f"{source_dir}/shtmTreeItem.cpp",
    f"{source_dir}/SimpleHtmlDocTree.cpp",
]
extra_compile_args=[f"-I{source_dir}/include", f"-I{source_dir}/pybind11/include"]
ext_modules = [
    Pybind11Extension(
        "python_example",
        sorted(sources),  # Sort source files for reproducibility
        extra_compile_args=extra_compile_args,
        cxx_std=17,
    ),
]

setup(
    name='mypackage',
    version='0.0.1',
    ext_modules=ext_modules,
)