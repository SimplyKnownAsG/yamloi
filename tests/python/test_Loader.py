import math
import random
import string
import timeit
import unittest
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

    @unittest.skip('block mode not supported!')
    def test_block_sequence(self):
        parser = yamloi.Loader('- one\n- 2\n- three.3', True)
        node = parser.load()
        self.assertTrue(node.is_sequence())
        self.assertEqual('[one, 2, three.3]', node.dump())


if __name__ == '__main__':
    unittest.main()
