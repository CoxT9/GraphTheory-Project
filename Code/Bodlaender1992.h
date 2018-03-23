#ifndef _BODLAENDER92_H_
#define _BODLAENDER92_H_

// Code that is relevant to Bodlaender's 1992 tree width bounding (treewidth with decomposition retrieval) algorithm
#include "ProjectUtil.h"

#define C1 0.5
#define C2 0.5

int get_total_friendly_vertices(graph_t **graph, int g_vertices, int g_edges, double degree_threshold);
int get_maximal_matching(edgepair_t **matching, graph_t **graph, int num_vertices);
void get_tree_decomposition(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k);
graph_t *get_contracted_graph(graph_t **graph, int g_vertices, edgepair_t **matching, int matching_size);
graph_t *get_improved_graph(graph_t **graph, int g_vertices, int k, double degree_threshold);
void get_decomp_brute_force(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k);
void dense_simplical_vertex_found(graph_t **improved_graph, int k);
node_t *get_i_simplical_vertices(graph_t **improved_graph, double degree_threshold, int k);
bool is_i_simplical(graph_t **graph, int v, double degree_threshold);
bool common_low_degree_neighbours(graph_t **graph, int w, int v, double degree_threshold);
int get_matching_repr(int x_vertex, edgepair_t** matching, int matching_size);
void generate_candidate_original_treedecomp(tree_decomp_t **larger_decomposition, tree_decomp_t **tdc, edgepair_t** matching, int matching_size);
void drop_i_simplical_vertices(graph_t **improved_graph, node_t* i_simplical_vertices_list_head);

#endif
