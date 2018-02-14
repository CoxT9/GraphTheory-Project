#ifndef _PROJECT_UTIL_H_
#define _PROJECT_UTIL_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INPUT_LENGTH 2048

typedef struct graph {
    int** data;
    int n;
} graph_t;

typedef enum { FALSE, TRUE } bool;

time_t timer;
struct tm* timeinfo;

void out(char *data);
void output_graph(graph_t **graph);
void generate_graph(graph_t **graph, char *path_to_graph);
int **init_graph(int num_vertices);

#endif
