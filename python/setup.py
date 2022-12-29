try:
    from pybind11.setup_helpers import Pybind11Extension
except ImportError:
    from setuptools import Extension as Pybind11Extension

from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

source_dir = ".."
sources = [
    f"{source_dir}/example.cpp", f"{source_dir}/example.h", f"{source_dir}/library.cpp",
    f"{source_dir}/shtmItem.h", f"{source_dir}/shtmTreeItem.h", f"{source_dir}/utils.h",
    f"{source_dir}/shtmTreeItem.cpp", f"{source_dir}/SimpleHtmlDocTree.cpp",
    f"{source_dir}/SimpleHtmlDocTree.h", f"{source_dir}/errors.h",
]
ext_modules = [
    Pybind11Extension(
        "python_example",
        sorted(sources),  # Sort source files for reproducibility
    ),
]

setup(
    name='mypackage',
    version='0.0.1',
    ext_modules=ext_modules,
)