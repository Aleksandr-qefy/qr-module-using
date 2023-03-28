# https://learn.microsoft.com/ru-RU/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022
# https://scipy-lectures.org/advanced/interfacing_with_c/interfacing_with_c.html#id1
from setuptools import setup, Extension
#from distutils.core import setup, Extension
#import numpy

## define the extension module

sfc_module = Extension('qr_module_alpha', sources = ['module.cpp'])

setup(
    name='qr_module_alpha',
    version='1.0',
    description='Provides qr C++ functions',
    ext_modules=[sfc_module]
)