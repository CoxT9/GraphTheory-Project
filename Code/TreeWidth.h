#ifndef _TREE_WIDTH_H_
#define _TREE_WIDTH_H_

// Code that is relevant to Bodlaender's 1992 tree width bounding (tree with decomposition retrieval) algorithm
#include "ProjectUtil.h"

typedef struct {
  int u;
  int v;
} edgepair_t;

typedef struct {
  int treewidth_bounded;
} tree_decomp_t;

int get_total_friendly_vertices(graph_t **graph, int g_vertices, int g_edges, double degree_threshold);
int get_maximal_matching(edgepair_t **matching, graph_t **graph, int num_vertices);
void get_tree_decomposition(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k);
graph_t *get_contracted_graph(graph_t **graph, int g_vertices, edgepair_t **matching, int matching_size);

#endif
