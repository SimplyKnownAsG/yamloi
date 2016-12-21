import math
import random
import string
import timeit
import unittest
from matplotlib import pyplot
import numpy

import yamloi


class NodeComparisonTests(unittest.TestCase):

    def setUp(self):
        self.scalar1 = yamloi.ScalarNode('1')
        self.scalar2 = yamloi.ScalarNode('2')

        self.sequence1 = yamloi.SequenceNode()
        self.sequence1.add(self.scalar1)
        self.sequence2 = yamloi.SequenceNode()
        self.sequence2.add(self.scalar2)

        self.mapping1 = yamloi.MappingNode()
    
    def test_lt(self):
        self.assertTrue(self.scalar1 < self.sequence1)
        self.assertFalse(self.sequence1 < self.scalar1)

        self.assertTrue(self.scalar1 < self.mapping1)
        self.assertFalse(self.mapping1 < self.scalar1)

        self.assertTrue(self.sequence1 < self.mapping1)
        self.assertFalse(self.mapping1 < self.sequence1)

    def test_eq(self):
        # ScalarNode
        self.assertFalse(self.scalar1 == self.sequence1)
        self.assertFalse(self.sequence1 == self.scalar1)
        self.assertFalse(self.scalar1 == self.scalar2)
        self.assertTrue(self.scalar1 == self.scalar1)
        # check that an equivalent ScalarNode is equal
        scalar = yamloi.ScalarNode('1')
        self.assertNotEqual(id(self.scalar1), id(scalar))
        self.assertTrue(self.scalar1 == scalar)

        # SequenceNode
        self.assertFalse(self.sequence1 == self.mapping1)
        self.assertFalse(self.mapping1 == self.sequence1)
        self.assertFalse(self.sequence1 == self.sequence2)
        self.assertTrue(self.sequence1 == self.sequence1)
        # check that an equivalent SequenceNode is equal
        sequence = yamloi.SequenceNode()
        sequence.add(scalar)
        self.assertNotEqual(id(self.sequence1), id(sequence))
        self.assertTrue(self.sequence1 == sequence)
        # corner case...
        scalar_trick = yamloi.ScalarNode('[1]')
        self.assertFalse(self.sequence1 == scalar_trick)

        self.assertFalse(self.sequence1 == self.mapping1)
        self.assertFalse(self.mapping1 == self.sequence1)

    def test_gt(self):
        self.assertFalse(self.scalar1 > self.sequence1)
        self.assertTrue(self.sequence1 > self.scalar1)

        self.assertFalse(self.scalar1 > self.mapping1)
        self.assertTrue(self.mapping1 > self.scalar1)

        self.assertFalse(self.sequence1 > self.mapping1)
        self.assertTrue(self.mapping1 > self.sequence1)


random_chars = string.ascii_letters + string.digits # + string.whitespace + string.punctuation
def random_strings(n_chars):
    base_str = ''.join(random.choice(random_chars) for _ in xrange(n_chars))
    slow_str = base_str[:-1] + chr(ord(base_str[-1]) + 1)
    fast_str = chr(ord(base_str[0]) + 1) + base_str[1:]
    return base_str, slow_str, fast_str

def pow10floor(x):
    return 10**(math.floor(math.log(x,10)) - 0.5)

def pow10ceil(x):
    return 10**(math.ceil(math.log(x,10)) + 0.5)


class NodeComparisonPerformanceTests(unittest.TestCase):

    count = 100

    def setUp(self):
        self.complexities = []
        self.slow = []
        self.fast = []

    def tearDown(self):
        pyplot.figure()
        pyplot.scatter(self.complexities, self.slow, c='red', label='slow')
        pyplot.scatter(self.complexities, self.fast, c='green', label='fast')
        pyplot.xlabel('complexity')
        pyplot.ylabel('time ($\\mu s$)')
        pyplot.legend(loc='upper left')
        # pyplot.xlim(0.9 * (min(self.complexities)), 1.1 * (max(self.complexities)))
        pyplot.ylim(0.9 * (min(self.slow.min(), self.fast.min())), 1.1 * (max(self.slow.max(), self.fast.max())))
        pyplot.xlim(pow10floor(min(self.complexities)), pow10ceil(max(self.complexities)))
        # pyplot.ylim(pow10floor(min(self.slow + self.fast)), pow10ceil(max(self.slow + self.fast)))
        pyplot.xscale('log')
        # pyplot.yscale('log')
        pyplot.savefig(self.id().split('.')[-1] + '.png')
        pyplot.close()

    def assert_performance(self):
        self.complexities = numpy.array(self.complexities)
        self.slow = 1e6 * numpy.array(self.slow)
        self.fast = 1e6 * numpy.array(self.fast)
        try:
            self.assertTrue(all(self.slow <=  self.slow[0] * self.complexities))
        except AssertionError:
            raise AssertionError('should be less, but is not:\n  {}\n  {}'
                    .format(self.slow, self.slow[0] * self.complexities))
        try:
            self.assertTrue(numpy.allclose(self.fast, self.fast[0], atol=self.fast[0]*0.5))
        except AssertionError:
            raise AssertionError('not all close:\n  {}\n {}'
                    .format(self.fast, self.fast[0]))

    def execute_timers(self, n, t_slow, t_fast):
        self.complexities.append(n)
        self.slow.append(t_slow.timeit(self.count) / self.count)
        self.fast.append(t_fast.timeit(self.count) / self.count)

    def test_lt_performance_scalar(self):
        for n in [10**x for x in range(4)]:
            base_str, slow_str, fast_str = random_strings(n)
            base_node = yamloi.ScalarNode(base_str)
            slow_node = yamloi.ScalarNode(slow_str)
            fast_node = yamloi.ScalarNode(fast_str)
            t_slow = timeit.Timer(lambda: base_node < slow_node)
            t_fast = timeit.Timer(lambda: base_node < fast_node)
            self.execute_timers(n, t_slow, t_fast)
        self.assert_performance()

    def test_eq_performance_scalar(self):
        for n in [10**x for x in range(4)]:
            base_str, slow_str, fast_str = random_strings(n)
            base_node = yamloi.ScalarNode(base_str)
            slow_node = yamloi.ScalarNode(slow_str)
            fast_node = yamloi.ScalarNode(fast_str)
            t_slow = timeit.Timer(lambda: base_node == slow_node)
            t_fast = timeit.Timer(lambda: base_node == fast_node)
            self.execute_timers(n, t_slow, t_fast)
        self.assert_performance()

    def test_gt_performance_scalar(self):
        for n in [10**x for x in range(4)]:
            base_str, slow_str, fast_str = random_strings(n)
            base_node = yamloi.ScalarNode(base_str)
            slow_node = yamloi.ScalarNode(slow_str)
            fast_node = yamloi.ScalarNode(fast_str)
            t_slow = timeit.Timer(lambda: base_node > slow_node)
            t_fast = timeit.Timer(lambda: base_node > fast_node)
            self.execute_timers(n, t_slow, t_fast)
        self.assert_performance()

    def _build_sequences(self, x):
        base_str, slow_str, fast_str = random_strings(20)
        base_seq = yamloi.SequenceNode()
        slow_seq = yamloi.SequenceNode()
        fast_seq = yamloi.SequenceNode()

        # prevent making a ton of actual objects... I think
        base_node = yamloi.ScalarNode(base_str)
        slow_node = yamloi.ScalarNode(slow_str)
        fast_node = yamloi.ScalarNode(fast_str)

        # build up sequences            
        base_seq.add(base_node)
        fast_seq.add(fast_node)
        for n in xrange(max(1, 10**x - 1)):
            base_seq.add(base_node)
            fast_seq.add(base_node)
            slow_seq.add(base_node)
        slow_seq.add(slow_node)

        return base_seq, slow_seq, fast_seq

    def test_lt_performance_seq(self):
        for x in range(4):
            base_seq, slow_seq, fast_seq = self._build_sequences(x)
            
            self.assertTrue(base_seq < slow_seq)
            self.assertTrue(base_seq < fast_seq)
            self.assertFalse(base_seq < base_seq)

            t_slow = timeit.Timer(lambda: base_seq < slow_seq)
            t_fast = timeit.Timer(lambda: base_seq < fast_seq)
            self.execute_timers(max(1, 10**x - 1), t_slow, t_fast)
        self.assert_performance()

    def test_eq_performance_seq(self):
        for x in range(4):
            base_seq, slow_seq, fast_seq = self._build_sequences(x)
            
            self.assertFalse(base_seq == slow_seq)
            self.assertFalse(base_seq == fast_seq)
            self.assertTrue(base_seq == base_seq)

            t_slow = timeit.Timer(lambda: base_seq == slow_seq)
            t_fast = timeit.Timer(lambda: base_seq == fast_seq)
            self.execute_timers(max(1, 10**x - 1), t_slow, t_fast)
        self.assert_performance()

    def test_gt_performance_seq(self):
        for x in range(4):
            base_seq, slow_seq, fast_seq = self._build_sequences(x)
            
            self.assertFalse(base_seq > slow_seq)
            self.assertFalse(base_seq > fast_seq)
            self.assertFalse(base_seq > base_seq)

            t_slow = timeit.Timer(lambda: base_seq > slow_seq)
            t_fast = timeit.Timer(lambda: base_seq > fast_seq)
            self.execute_timers(max(1, 10**x - 1), t_slow, t_fast)
        self.assert_performance()

    def _build_maps(self, x):
        base_str, slow_str, fast_str = random_strings(20)
        base_map = yamloi.MappingNode()
        slow_map = yamloi.MappingNode()
        fast_map = yamloi.MappingNode()

        # prevent making a ton of actual objects... I think
        base_node = yamloi.ScalarNode(base_str)
        slow_node = yamloi.ScalarNode(slow_str)
        fast_node = yamloi.ScalarNode(fast_str)

        # build up mapuences            
        base_map.add(base_node)
        fast_map.add(fast_node)
        for n in xrange(max(1, 10**x - 1)):
            base_map.add(base_node, base_node)
            fast_map.add(base_node, base_node)
            slow_map.add(base_node, base_node)
        slow_map.add(slow_node)

        return base_map, slow_map, fast_map

    @unittest.skip('tbd')
    def test_lt_performance_map(self):
        for x in range(4):
            base_map, slow_map, fast_map = self._build_maps(x)
            
            self.assertTrue(base_map < slow_map)
            self.assertTrue(base_map < fast_map)
            self.assertFalse(base_map < base_map)

            t_slow = timeit.Timer(lambda: base_map < slow_map)
            t_fast = timeit.Timer(lambda: base_map < fast_map)
            self.execute_timers(max(1, 10**x - 1), t_slow, t_fast)
        self.assert_performance()

    @unittest.skip('tbd')
    def test_eq_performance_map(self):
        for x in range(4):
            base_map, slow_map, fast_map = self._build_maps(x)
            
            self.assertFalse(base_map == slow_map)
            self.assertFalse(base_map == fast_map)
            self.assertTrue(base_map == base_map)

            t_slow = timeit.Timer(lambda: base_map == slow_map)
            t_fast = timeit.Timer(lambda: base_map == fast_map)
            self.execute_timers(max(1, 10**x - 1), t_slow, t_fast)
        self.assert_performance()

    @unittest.skip('tbd')
    def test_gt_performance_map(self):
        for x in range(4):
            base_map, slow_map, fast_map = self._build_maps(x)
            
            self.assertFalse(base_map > slow_map)
            self.assertFalse(base_map > fast_map)
            self.assertFalse(base_map > base_map)

            t_slow = timeit.Timer(lambda: base_map > slow_map)
            t_fast = timeit.Timer(lambda: base_map > fast_map)
            self.execute_timers(max(1, 10**x - 1), t_slow, t_fast)
        self.assert_performance()


if __name__ == '__main__':
    unittest.main()
