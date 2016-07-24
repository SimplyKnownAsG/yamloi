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

    def test_fails_with_non_scalar_key(self):
        node = yamloi.MappingNode()
        with self.assertRaises(TypeError):
            node.add(yamloi.SequenceNode(), yamloi.ScalarNode(1))
        with self.assertRaises(TypeError):
            node.add(yamloi.MappingNode(), yamloi.ScalarNode(4))

    def test_fails_to_get_nonexistent_item(self):
        node = yamloi.MappingNode()
        with self.assertRaises(IndexError):
            print(node[yamloi.SequenceNode()])

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

    @unittest.skip('yamloi.nodalize not implemented')
    def test_create_MappingNode(self):
        expected = {'one': 2, 'three': {4: 'five'}, 6: '7'}
        output = yamloi.nodalize(expected)

