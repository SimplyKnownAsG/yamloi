import math
import random
import string
import timeit
import unittest
from matplotlib import pyplot
import numpy

import yamloi


class LoaderTests(unittest.TestCase):

    def test_simple_scalar(self):
        parser = yamloi.Loader('hello world!', True)
        node = parser.load()
        self.assertTrue(node.is_scalar())
        self.assertEqual('hello world!', node.dump())
 
    def test_simple_sequence(self):
        parser = yamloi.Loader('[hello world!, from,here]', True)
        node = parser.load()
        self.assertTrue(node.is_sequence())
        self.assertEqual('[hello world!, from, here]', node.dump())

    def test_empty_sequence(self):
        parser = yamloi.Loader('[]', True)
        node = parser.load()
        self.assertTrue(node.is_sequence())
        self.assertEqual('[]', node.dump())

    def test_nested_simple_sequence(self):
        parser = yamloi.Loader('[[hello,hola], [world,   mundo  ]]', True)
        node = parser.load()
        self.assertTrue(node.is_sequence())
        self.assertEqual('[[hello, hola], [world, mundo]]', node.dump())

