from sage.all_cmdline import *   # import sage library
from time import gmtime, strftime

benchmark = Graph({
  1: [2, 5, 6],
  2: [3, 7],
  3: [2, 4, 6, 7],
  4: [3, 5, 6, 7],
  5: [1, 4, 7],
  6: [1, 3, 4],
  7: [2, 3, 4, 5]
},name="Sample Graph")

benchmark.show()
treedec = benchmark.treewidth(certificate=True)
treedec.show()