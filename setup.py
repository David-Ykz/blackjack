from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        'round_manager',
        ['bindings.cpp'],
        include_dirs=[pybind11.get_include()],
        language='c++'
    ),
]

setup(
    name='round_manager',
    ext_modules=ext_modules,
)