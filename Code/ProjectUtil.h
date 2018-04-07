#ifndef _PROJECT_UTIL_H_
#define _PROJECT_UTIL_H_

#define _BSD_SOURCE
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>


#define INPUT_LENGTH 2048
#define OUT_BUFFER_LENGTH 4096

#define min(x, y) ( x < y ? x : y)
#define max(x, y) ( x > y ? x : y)

typedef struct Node {
  struct Node* next;
  int value;
} node_t;

typedef struct {
    node_t** adjacencies; // an array of nodes (linked lists)
    int *degrees;
    int num_vertices;
    int num_edges;
} graph_t;

typedef struct {
  int u;
  int v;
} edgepair_t;

typedef struct nested_node_t {
  struct nested_node_t* next;
  node_t* value;
} nested_node_t;

typedef struct {
  node_t* entries; // a linked list of the vertices captured in this node
  node_t* connections; // linked list of other treenodes this node is connected to
} tree_node_t;

typedef struct {
  int treewidth_bounded;
  nested_node_t* vertex_subsets; // this is going to be a linked list of linked lists. each bag Xi contains n_i vertices
  tree_node_t* tree_root;
} tree_decomp_t;

typedef struct generic_node_t {
  struct generic_node_t* next;
  void* data;
} generic_node_t;

typedef enum { FALSE, TRUE } bool;

time_t timer;
struct tm* timeinfo;

char out_buffer[OUT_BUFFER_LENGTH];

void out(char *data);
void output_graph(graph_t **graph);
void generate_graph(graph_t **graph, char *path_to_graph, int reflected);
node_t **init_adjacencies(int num_vertices);
int *init_degrees(int num_vertices);
void add_new_node(node_t **adjacencies, int src, int dest);
int get_ll_size(node_t **head);
bool all_nodes_present(node_t **head_inner, node_t **head_outer);
bool all_neighbours_present(graph_t **graph, int v, int w);
int contract_edge(graph_t **graph, int u, int v);
int has_neighbour(graph_t **graph, int u, int v);
int ll_search(node_t* head, int target);

#endif
