#include "ProjectUtil.h"

void out(char *data) {
    // printf data with timestamp
    time(&timer);
    timeinfo = localtime(&timer);

    printf(
        "(%d-%d-%d %d:%d:%d) %s\n",
        timeinfo->tm_mday,
        timeinfo->tm_mon+1,
        timeinfo->tm_year+1900,
        timeinfo->tm_hour,
        timeinfo->tm_min,
        timeinfo->tm_sec,
        data
    );
}

void output_graph(graph_t **graph) {
    // Take graph and write to stdout without extra logging
    int i;
    int j;
    int value;
    int n = (*graph)->num_vertices;
    for(i = 0; i < n; i++) {
        printf("Vertex %d: ", i);
        for(j = 0; j < n; j++) {
            value = (*graph)->data[i][j];
            if(value) {
                printf("%d", j);
                if(j < n -1) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    out("End of graph output\n");
}

void generate_graph(graph_t **graph, char *path_to_graph) {
    // Open file, parse into format and make up the graph
    // Line 0: Name (if not int)
    // Line 1: N
    // N lines: adjancies
    // 0 alone is eof

    int num_vertices = 0;
    int num_edges = 0;
    int value;
    int curr_vertex;

    int** data;
    char *tok;
    char name_str[INPUT_LENGTH];

    bool graph_set = FALSE;
    bool complete = FALSE;
    bool started = FALSE;

    int line = 0;
    FILE *graph_input = fopen(path_to_graph, "r");
    char input[INPUT_LENGTH];

    if(graph_input) {
        while(!complete && fgets(input, INPUT_LENGTH, graph_input)) {
            if(!started) {
                // 0th line. Did graph have a name?
                if(!isdigit(input[0])) {
                    // Graph name:
                    sprintf(name_str, "Name of graph is: %s", input);
                    out(name_str);
                } else {
                    num_vertices = atoi(input);
                    data = init_graph(num_vertices);
                    graph_set = TRUE;
                }
                started = TRUE;
            } else {
                if(!graph_set) {
                    num_vertices = atoi(input);
                    data = init_graph(num_vertices);
                    graph_set = TRUE;
                } else {
                    // nth line. also careful of huge lines.
                    tok = strtok(input, " ");
                    if(!strcmp(tok, "0")) {
                        complete = TRUE;
                    } else {
                        while(tok) {
                            value = atoi(tok);
                            if(value < 0) {
                                curr_vertex = (value*-1)-1;
                                line++;
                            } else if(value > 0) {
                                printf("%d %d %d\n", curr_vertex, value, num_vertices);
                                data[curr_vertex][value-1] = 1;
                                data[value-1][curr_vertex] = 1;
                                num_edges++;
                            }
                            tok = strtok(NULL, " ");
                        }
                    }
                }
            }
        }
    }
    (*graph)->data = data;
    (*graph)->num_vertices = num_vertices;
    (*graph)->num_edges = num_edges;
}

int **init_graph(int num_vertices) {
    int i;
    int **data;

    data = (int **)malloc(num_vertices * sizeof(int*));
    if(data) {
        for(i = 0; i < num_vertices; i++) {
            data[i] = (int *)malloc(sizeof(int) * num_vertices);
        }
    }
    return data;
}
