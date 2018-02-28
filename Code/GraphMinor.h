#ifndef _GRAPH_MINOR_H_
#define _GRAPH_MINOR_H_

#include "ProjectUtil.h"

#define REQUIRED_ARGS 2
#define BAD_ARGS_STR "Usage: %s <Path-To-Graph> <Path-To-Fixed-Minor>"

#define GRAPH_INDEX 1
#define MINOR_INDEX 2

int k_disjoint_paths(graph_t **graph, graph_t **minor, int g_vertices, int g_edges, int h_vertices, int h_edges);
int get_tree_decomposition(graph_t **graph, int g_vertices, int g_edges, int k);
int graph_has_minor(graph_t **graph, graph_t **minor);

#endif
