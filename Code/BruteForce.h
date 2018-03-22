#ifndef _BRUTE_FORCE_H_
#define _BRUTE_FORCE_H_

// Code that is relevant to Bodlaender's 1992 tree width bounding (treewidth with decomposition retrieval) algorithm
#include "ProjectUtil.h"

void get_decomp_brute_force(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k);

#endif

// is there a way to get tw without decomposition? Is that useful?
// Clarify why treewidth is a popular parameter and why it is useful for dynamic programming
