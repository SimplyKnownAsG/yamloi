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
        self.assertEqual('[thirty, three]', node.dump(yamloi.Format(yamloi.FlowStyle)))
        self.assertEqual('- thirty\n- three\n', node.dump(yamloi.Format(yamloi.BlockStyle)))

    def test_list_of_lists(self):
        node = yamloi.SequenceNode()
        for lhs in range(-1, 2):
            seq_node = yamloi.SequenceNode()
            for rhs in range(-1, 2):
                seq_node.add(yamloi.ScalarNode('{} {}'.format(lhs, rhs)))
            node.add(seq_node)

        self.assertEqual('[[-1 -1, -1 0, -1 1], [0 -1, 0 0, 0 1], [1 -1, 1 0, 1 1]]', node.dump())
        self.assertEqual('[[-1 -1, -1 0, -1 1], [0 -1, 0 0, 0 1], [1 -1, 1 0, 1 1]]',
                node.dump(yamloi.Format(yamloi.FlowStyle)))
        self.assertEqual('\n'.join((
            '- - -1 -1',
            '  - -1 0',
            '  - -1 1',
            '- - 0 -1',
            '  - 0 0',
            '  - 0 1',
            '- - 1 -1',
            '  - 1 0',
            '  - 1 1',
            ''
            )),
            node.dump(yamloi.Format(yamloi.BlockStyle)))

    def test_list_of_things(self):
        node = yamloi.SequenceNode()
        seq_node = yamloi.SequenceNode()
        node.add(seq_node)
        for n in range(3):
            seq_node.add(n)
        node.add('Chikens in the rain')
        seq_node = yamloi.SequenceNode()
        node.add(seq_node)
        for w in ['Lucy', 'walking']:
            seq_node.add(w)

        self.assertEqual('[[0, 1, 2], Chikens in the rain, [Lucy, walking]]', node.dump())
        self.assertEqual('\n'.join((
            '- - 0',
            '  - 1',
            '  - 2',
            '- Chikens in the rain',
            '- - Lucy',
            '  - walking', 
            ''
            )),
            node.dump(yamloi.Format(yamloi.BlockStyle)))
        node[0].format = yamloi.Format(yamloi.FlowStyle)
        self.assertEqual('\n'.join((
            '- [0, 1, 2]',
            '- Chikens in the rain',
            '- - Lucy',
            '  - walking', 
            ''
            )),
            node.dump(yamloi.Format(yamloi.BlockStyle)))

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


if __name__ == '__main__':
    unittest.main()
