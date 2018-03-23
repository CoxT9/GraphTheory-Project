#ifndef _BODLAENDER12_H_
#define _BODLAENDER12_H_

// Code that is relevant to Bodlaender's 2010 Tree Width Dynamic Programming Algorithm (exponential time)
#include "ProjectUtil.h"

typedef struct {
  node_t* set;
  int width;
} treewidth_pair_t;

void get_decomp_brute_force(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k);
int get_treewidth_dp(graph_t **graph);
void max_clique_recurse(graph_t **graph, int* curr_clique, int* curr_size, int* clique_vertices, int* max_size, int depth);
int max_clique(graph_t **graph, int** clique_vertices);
int get_total_reachable_vertices(graph_t **graph, treewidth_pair_t* tw_pair, int excluded_vertex);
treewidth_pair_t* find_set_in_candidate(int size, node_t* head, int new_vtx, generic_node_t* candidate_list);
bool all_present(int n, node_t* head1, node_t* head2);

#endif

// is there a way to get tw without decomposition? Is that useful?
// Clarify why treewidth is a popular parameter and why it is useful for dynamic programming
