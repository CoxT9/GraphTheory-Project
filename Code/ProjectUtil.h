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

#define min(x, y) ( x < y ? x : y)

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
get_ll_size(node_t **head);
bool all_nodes_present(node_t **head_inner, node_t **head_outer);
bool all_neighbours_present(graph_t **graph, int v, int w);

#endif
