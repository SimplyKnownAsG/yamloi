import unittest
from random import shuffle

import yamloi


class ScalarNodeTests(unittest.TestCase):

    def test_is_scalar(self):
        node = yamloi.ScalarNode('content')
        self.assertTrue(node.is_scalar())
        self.assertFalse(node.is_sequence())
        self.assertFalse(node.is_mapping())

    def test_string_only(self):
        node = yamloi.ScalarNode('thirty three')
        self.assertEqual('thirty three', node.as_string())
        
        for func_name in ['as_long', 'as_double']:
            with self.assertRaises(TypeError):
                getattr(node, func_name)()

        self.assertEqual('thirty three', node.dump())

    def test_string(self):
        node = yamloi.ScalarNode('37')
        self.assertEqual(37, node.as_long())
        self.assertEqual(37.0, node.as_double())
        self.assertEqual('37', node.dump())

    def test_int(self):
        node = yamloi.ScalarNode(14)
        self.assertEqual(14, node.as_long())
        self.assertEqual(14.0, node.as_double())

        self.assertEqual('14', node.dump())

    def test_long(self):
        for maxmin64 in [2 ** 63 - 1, -2**63]:
            node = yamloi.ScalarNode(maxmin64)
            self.assertEqual(maxmin64, node.as_long())
            self.assertEqual(float(maxmin64), node.as_double())     
            self.assertEqual(str(maxmin64), node.dump())

    def test_int64_range(self):
        for maxmin64 in [2 ** 63, -2**63 - 1]:
            node = yamloi.ScalarNode(maxmin64)
            # assert NOT equal ... SWIG/c++ converts to a float
            self.assertNotEqual(str(maxmin64), node.dump())

            float_val = float(maxmin64)
            node = yamloi.ScalarNode(float_val)
            self.assertEqual(float_val, node.as_double())

    def test_sort_order(self):
        strings = ['abc', 'ABC', '123', '321', 'zzz', '!chicken', '#!$@?']
        shuffle(strings)
        while sorted(strings) == strings:
            shuffle(strings)
        scalars = [yamloi.ScalarNode(ss) for ss in strings]
        self.assertEqual(sorted(strings), [sn.dump() for sn in sorted(scalars)])



class SpecialCharacterScalarNodeTests(unittest.TestCase):

    def test_multiline_string(self):
        node = yamloi.ScalarNode('thirty\nthree\n plus 13')
        self.assertEqual('thirty\nthree\n plus 13', node.as_string())

        for func_name in ['as_long', 'as_double']:
            with self.assertRaises(TypeError):
                getattr(node, func_name)()

        self.assertEqual("'thirty\\nthree\\n plus 13'", node.dump())

    def test_comma(self):
        node = yamloi.ScalarNode('one,two,three')
        self.assertEqual("'one,two,three'", node.dump())

    def test_single_quote(self):
        node = yamloi.ScalarNode("one'two'three")
        self.assertEqual("one'two'three", node.dump())

    def test_double_quote(self):
        node = yamloi.ScalarNode('one"two"three')
        self.assertEqual('one"two"three', node.dump())

    def test_squote_comma(self):
        node = yamloi.ScalarNode('one\'two,three')
        self.assertEqual('"one\'two,three"', node.dump())

    def test_dquote_comma(self):
        node = yamloi.ScalarNode('one\"two,three')
        self.assertEqual("'one\"two,three'", node.dump())

    def test_sadquote_comma(self):
        node = yamloi.ScalarNode('one\"two,three\'four')
        self.assertEqual("'one\"two,three\\'four'", node.dump())

    def test_lbracket(self):
        node = yamloi.ScalarNode('left[bracket')
        self.assertEqual("'left[bracket'", node.dump())

    def test_rbracket(self):
        node = yamloi.ScalarNode('right]bracket')
        self.assertEqual("'right]bracket'", node.dump())

    def test_lbrace(self):
        node = yamloi.ScalarNode('left{bracket')
        self.assertEqual("'left{bracket'", node.dump())

    def test_rbrace(self):
        node = yamloi.ScalarNode('right}bracket')
        self.assertEqual("'right}bracket'", node.dump())


if __name__ == '__main__':
    unittest.main()
