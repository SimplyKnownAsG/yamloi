
import glob
import os
import shutil
import sys

import unittest

import bs
from bs import builders

lib_builder = builders.get_builder('c++-lib')
exe_builder = builders.get_builder('c++-exe')

srcs = [src for src in glob.glob('./src/*.cpp') if 'main' not in src]

yamloilib = bs.SharedLibrary('yamloi', *srcs)

lib_builder.build(yamloilib)

swigged = bs.SwigSource('yamloi.i')#, *glob.glob('./src/*.hpp'))
swigged.target_language = 'python'
swigged.cpp = True
swigged.args.append('-I./src')
swigged.args.append('-builtin')
swigged.create()

yamloi_py = bs.SharedLibrary('_yamloi', *(srcs + [bs.Object(swigged)]))

lib_builder.build(yamloi_py)
bs.copy(yamloi_py.path, os.path.basename(yamloi_py.path.replace('.dll', '.pyd')))

unittest.main(argv=['unittest', 'discover', '-bv'])


