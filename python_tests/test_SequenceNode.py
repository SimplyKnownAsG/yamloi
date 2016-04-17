import unittest
import yamloi

class SequenceNodeTests(unittest.TestCase):

    def test_is_sequence(self):
        node = yamloi.SequenceNode()
        self.assertFalse(node.is_scalar())
        self.assertTrue(node.is_sequence())
        self.assertFalse(node.is_mapping())

    def test_string_only(self):
        node = yamloi.SequenceNode()
        node.add(yamloi.ScalarNode('thirty'))
        node.add(yamloi.ScalarNode('three'))
        # node = yamloi.SequenceNode([yamloi.ScalarNode('thirty'), yamloi.ScalarNode('three')])
        self.assertEqual('[thirty, three]', node.dump())

    def test_list_of_lists(self):
        node = yamloi.SequenceNode()
        for lhs in range(-1, 2):
            seq_node = yamloi.SequenceNode()
            for rhs in range(-1, 2):
                seq_node.add(yamloi.ScalarNode('{} {}'.format(lhs, rhs)))
            node.add(seq_node)

        self.assertEqual('[[-1 -1, -1 0, -1 1], [0 -1, 0 0, 0 1], [1 -1, 1 0, 1 1]]', node.dump())

    def test_item_with_comma(self):
        node = yamloi.SequenceNode()
        node.add(yamloi.ScalarNode('1,2'))
        node.add(yamloi.ScalarNode('abc'))
        self.assertEqual("['1,2', abc]", node.dump())

    def test_item_with_squote(self):
        node = yamloi.SequenceNode()
        node.add(yamloi.ScalarNode('1\'2'))
        node.add(yamloi.ScalarNode('abc'))
        self.assertEqual("[1'2, abc]", node.dump())

    def test_item_with_dquote(self):
        node = yamloi.SequenceNode()
        node.add(yamloi.ScalarNode('1\"2'))
        node.add(yamloi.ScalarNode('abc'))
        self.assertEqual('[1"2, abc]', node.dump())

