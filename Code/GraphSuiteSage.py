from sage.all_cmdline import *   # import sage library
from time import gmtime, strftime

thomassen = Graph({
  1: [30, 14, 2],
  2: [16, 3],
  3: [18, 4],
  4: [15, 5],
  5: [23, 17, 6],
  6: [20, 7],
  7: [22, 8],
  8: [24, 9],
  9: [27, 10],
  10: [29, 11],
  11: [26, 12],
  12: [38, 28, 13],
  13: [31, 14],
  14: [33],
  15: [19, 16],
  16: [17],
  17: [18],
  18: [19],
  19: [32],
  20: [24, 21],
  21: [25, 22],
  22: [23],
  23: [24],
  25: [29, 26],
  26: [27],
  27: [28],
  28: [29],
  30: [34, 31],
  31: [23],
  32: [33],
  33: [34],
},name="Thomassen34")

tree = Graph({
  1: [9, 5, 2],
  2: [4, 3],
  5: [8, 7, 6],
  9: [10],
  7: [14, 13, 12, 11]
},name="Spanning Tree")

graphs = [
  graphs.CompleteGraph(4),
  graphs.CompleteGraph(5),
  graphs.CompleteGraph(6),
  graphs.CompleteBipartiteGraph(3,3),
  graphs.CompleteBipartiteGraph(4,5),
  graphs.PathGraph(3),
  graphs.PathGraph(5),
  graphs.WheelGraph(6),
  graphs.WheelGraph(7),
  graphs.PetersenGraph(),
  graphs.CubeGraph(3),
  graphs.CubeGraph(4),
  graphs.DodecahedralGraph(),
  tree,
  graphs.GridGraph([5,5]),
  thomassen
]

the_time = strftime("%Y-%m-%d %H:%M:%S", gmtime())
print(the_time)

for g in graphs:
  x = g.treewidth()
  print(x, g.name())
  the_time = strftime("%Y-%m-%d %H:%M:%S", gmtime())
  print(the_time)