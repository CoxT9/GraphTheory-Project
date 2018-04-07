#ifndef _TREE_WIDTH_H_
#define _TREE_WIDTH_H_

// Code that is relevant to Bodlaender's 1992 tree width bounding (treewidth with decomposition retrieval) algorithm
#include "ProjectUtil.h"
#include "Bodlaender1992.h"
#include "Bodlaender2012.h"

#define SMALL_GRAPH_SIZE 1000

void get_tree_decomposition(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k);

#endif