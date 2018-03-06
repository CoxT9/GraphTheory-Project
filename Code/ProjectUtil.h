#ifndef _PROJECT_UTIL_H_
#define _PROJECT_UTIL_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define INPUT_LENGTH 2048
#define OUT_BUFFER_LENGTH 4096

typedef struct Node {
  struct Node* next;
  int vertex_id;
} node_t;

typedef struct {
    node_t** adjacencies; // an array of nodes (linked lists)
    int *degrees;
    int num_vertices;
    int num_edges;
} graph_t;

typedef struct {
  int treewidth_bounded;
} tree_decomp_t;

typedef enum { FALSE, TRUE } bool;

time_t timer;
struct tm* timeinfo;

char out_buffer[OUT_BUFFER_LENGTH];

void out(char *data);
void output_graph(graph_t **graph);
void generate_graph(graph_t **graph, char *path_to_graph);
node_t **init_adjacencies(int num_vertices);
int *init_degrees(int num_vertices);
void add_new_node(node_t **adjacencies, int src, int dest);

#endif
