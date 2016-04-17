import unittest
import yamloi

class MappingNodeTests(unittest.TestCase):

    def test_is_mapping(self):
        node = yamloi.MappingNode()
        self.assertFalse(node.is_scalar())
        self.assertFalse(node.is_sequence())
        self.assertTrue(node.is_mapping())

    def test_single_item(self):
        node = yamloi.MappingNode()
        node.add(yamloi.ScalarNode('key'), yamloi.ScalarNode('value'))
        self.assertEqual('{key: value}', node.dump())

    def test_multi_item(self):
        node = yamloi.MappingNode()
        nodes = []
        nodes.append((yamloi.ScalarNode(1), yamloi.ScalarNode('one')))
        nodes.append((yamloi.ScalarNode(2), yamloi.ScalarNode('two')))
        nodes.append((yamloi.ScalarNode('one'), yamloi.ScalarNode(1)))
        nodes.append((yamloi.ScalarNode('two'), yamloi.ScalarNode(2)))
        for n1, n2 in zip(nodes[:-1], nodes[1:]):
            self.assertEqual(n1[0] < n2[0], n1[0].dump() < n2[0].dump())
            node.add(n1[0], n1[1])
        node.add(n2[0], n2[1])
        self.assertEqual('{1: one, 2: two, one: 1, two: 2}', node.dump())

