#include "ProjectUtil.h"

void out(char *output) {
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
        output
    );
}

void output_graph(graph_t **graph) {
    // Take graph and write to stdout without extra logging
    int i;
    int n = (*graph)->num_vertices;
    node_t* curr_node;
    for(i = 0; i < n; i++) {
        printf("Vertex %d: ", i);
        curr_node = (*graph)->adjacencies[i];
        while(curr_node) {
            printf("%d ", curr_node->value);
            curr_node = curr_node->next;
        }
        printf("\n");
    }
    out("End of graph output\n");
}

void generate_graph(graph_t **graph, char *path_to_graph) {
    // Open file, parse into format and make up the graph
    // Line 0: Name (if not int)
    // Line 1: N
    // N lines: adjacencies
    // 0 alone is eof
    int num_vertices = 0;
    int num_edges = 0;
    int value;
    int curr_vertex;

    node_t** adjacencies;
    int *degrees;
    char *tok;

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
                    sprintf(out_buffer, "Name of graph is: %s", input);
                    out(out_buffer);
                } else {
                    num_vertices = atoi(input);
                    adjacencies = init_adjacencies(num_vertices);
                    degrees = init_degrees(num_vertices);
                    graph_set = TRUE;
                }
                started = TRUE;
            } else {
                if(!graph_set) {
                    num_vertices = atoi(input);
                    adjacencies = init_adjacencies(num_vertices);
                    degrees = init_degrees(num_vertices);
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
                                // This code assumes that the graph is 'fully' described. (ie: gives all edges uv and vu)
                                add_new_node(adjacencies, curr_vertex, value-1);
                                degrees[curr_vertex]++;
                                num_edges++;
                            }
                            tok = strtok(NULL, " ");
                        }
                    }
                }
            }
        }
    }
    (*graph)->adjacencies = init_adjacencies(num_vertices);
    memcpy( (*graph)->adjacencies, adjacencies, num_vertices*sizeof(node_t*));

    (*graph)->degrees = init_degrees(num_vertices);
    memcpy( (*graph)->degrees, degrees, num_vertices*sizeof(int));

    (*graph)->num_vertices = num_vertices;
    (*graph)->num_edges = num_edges;
}

node_t **init_adjacencies(int num_vertices) {
    int i;
    node_t **adjacencies;
    adjacencies = (node_t **)malloc(num_vertices * sizeof(node_t*));
    if(adjacencies) {
        for(i = 0; i < num_vertices; i++) {
            adjacencies[i] = NULL;
        }
    }
    return adjacencies;
}

int *init_degrees(int num_vertices) {
    int i;
    int *degrees = (int *)malloc(sizeof(int) * num_vertices);
    if(degrees) {
        for(i = 0; i < num_vertices; i++) {
            degrees[i] = 0;
        }
    }
    return degrees;
}

void add_new_node(node_t **adjacencies, int src, int dest) {
    node_t *curr_node= (node_t*)malloc(sizeof(node_t*));
    node_t *new_node = (node_t*)malloc(sizeof(node_t*));
    new_node->value = dest;
    new_node->next = NULL;
    if(adjacencies[src]) {
        curr_node = adjacencies[src]; 
        while(curr_node->next) {
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
    } else {
        adjacencies[src] = new_node;
    }
}

void get_ll_size(node_t **head) {
    node_t *curr = *head;
    int size = 0;
    while(curr) {
        size++;
        curr = curr->next;
    }
    return size;
}

bool all_neighbours_present(graph_t **graph, int v, int w) {
    bool all_present = FALSE;
    if((*graph)->degrees[v] <= (*graph)->degrees[w]) {
        all_present = all_nodes_present(&((*graph)->adjacencies[v]), &((*graph)->adjacencies[w]));
    }
    return all_present;
}

bool all_nodes_present(node_t **head_inner, node_t **head_outer) {
    bool all_present = TRUE;
    bool present;
    curr_inner = *head_inner;
    int u;
    int v;

    while(curr_inner && all_present) {
        u = curr_inner->value;
        present = FALSE;
        curr_outer = *head_outer;
        while(curr_outer && !present) {
            if(u == curr_outer->value) {
                present = TRUE;
            } else {
                curr_outer = curr_outer->next;
            }
        }
        all_present = present;
    }
    return all_present;
}
