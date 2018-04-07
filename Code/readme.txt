COMP 4060 - Graph Theory
Final Project: Programming Component

The primary folder of this submission contains these entries:
- Main.c
- Main.h
- Treewidth.c
- Treewidth.h
- Bodlaender1992.c
- Bodlaender1992.h
- Bodlaender2012.c
- Bodlaender2012.h
- ProjectUtil.c
- ProjectUtil.h
- makefile
- run
- benchmark (folder)

Main.c/h is where the main method is found. This file sets up the inputs and calls the Treewidth code. Treewidth.c/h is an interface for the actual Treewidth algorithms. The files also cover some surface-level code for the two algorithms implemented. Bodlaender1992.c/h is an implemenation attempt of Bodlaender's 1992 linear time treewidth algorithm. Note that this code is not fully tested and is not expected to function. This code does however provide a benificial illustration of how the algorithm may be constructed in practice. Bodlaender2012.c/h is a working implementation of Bodlaender's 2012 dynamic programming algorithm for treewidth. Finally, Projectutil.c/h provides supplemental code/structures for use by all other programs.

Note: The Treewidth code provided is ultimately meant to check how large the graph is, and uses the graph size as a basis for deciding which algorithm to use. However, since only one algorithm currently functions, the parameter SMALL_GRAPH_SIZE in Treewidth.h is set to an artificially high value.

The code takes in a graph specified the same way as the assignments and displays the treewidth of the graph. Note that for larger graphs (such as Thomassen, Grid55, Dodecahedron and Random25), the algorithm will run for _many_ hours.

The 'benchmark' folder contains sample inputs for all graphs executed in the report. Some interesting graphs to compute the treewidth for (without spending multiple hours) are the Petersen, SpanningTree and Cube4 graphs.

To compute a graph's treewidth, open a terminal in this directory and run the following:

$> ./run <input> <k> <Reflective 0/1>

There is no need to build the makefile. the 'run' script clears and rebuilds, and then executes the code against the parameters provided.

The parameter <input> is a valid path to a text file specifying a graph of interest. The directory must be relative to the directory of the code. 

The parameter <k> is a nonzero positive integer. The original intention of the treewidth algorithm is to solve the treewidth problem which is 'given a graph G and an integer k, is tw(g) <= k?'. Therefore, a constant should be passed to the algorithm (Bodlaender's 1992 algorithm relies on k, but the 2012 algorithm ultimately does not). For the purpose of this submission, it is suitable to leave k as an arbitrary large value (ie: 100).

The parameter <Reflective> is an integer 0 or 1 to represent a boolean. 'Reflective' is a description of the input file. Some graph inputs are 'reflective'. That is, for every recorded edge uv, the edge vu is also recorded. Non-reflective inputs only record the edge uv (and the code therefore accounts for vu automatically). The graphs inputs should be checked for their reflectivity before executing the code. For reference, Petersen and Cube4 are reflective, and the SpanningTree is not. Therefore, to run the Petersen, Cube4 and SpanningTree, open a terminal and run any of the following:

$> ./run benchmark/Petersen.txt 100 1
$> ./run benchmark/SpanningTree.txt 100 0
$> ./run benchmark/Cube4.txt 100 1

The program will output the contents of the graph as well as other basic information. As the algorithm runs, a printout will appear for every row of the dynamic programming table built. The three examples in this readme will run for a few seconds. Finally, it will be printed that the graph has bounded treewidth. For the samples provided, this is always true. The ultimate intension of this code is to be able to decide if a graph has bounded treewidth (bounded treewidth is a highly relevant parameter for many NP-Hard problems).

Completed by Taylor Cox, 7669196, 4/7/2018