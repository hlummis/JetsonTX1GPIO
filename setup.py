from distutils.core import setup, Extension

setup(
    ext_modules=[Extension("_jetsonGPIO", ["_jetsonGPIO.c", "jetsonGPIO.c"])]
)