import unittest
import yamloi

class FormatTests(unittest.TestCase):

    def test_is_accessible_from_namespace(self):
        self.assertNotEqual(yamloi.FlowStyle, yamloi.BlockStyle)


if __name__ == '__main__':
    unittest.main()
