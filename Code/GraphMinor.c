/* Given a graph G and a fixed graph H, does G have a minor H?

1. Basic G and H input
2. Basic Implementation and compare with Sage
3. Bring complexity down to O(nm)
4. Metrics (find more data too)

This file exclusively includes code for the graph minor algorithm itself.
Other code (ie: input parsing, outging, etc) can be found in a util source.
Note that the graphs are 0-indexed. 
Another file may be added for graph-theoretical utilities.
*/
#include "GraphMinor.h"

int main(int argc, char** argv) {
    graph_t *graph = malloc(sizeof(graph_t*));
    graph_t *minor = malloc(sizeof(graph_t*));
    int has_minor = 0; 
    int args_str_len = strlen(BAD_ARGS_STR) + 100;

    if(argc != REQUIRED_ARGS + 1) {
        char *bad_args = malloc(sizeof(char*)*args_str_len);
        sprintf(bad_args, BAD_ARGS_STR, argv[0]);
        out(bad_args);
        return EXIT_SUCCESS;
    }

    out("Launching Fixed Graph Minor Program...");

    generate_graph(&graph, argv[GRAPH_INDEX]);
    generate_graph(&minor, argv[MINOR_INDEX]);

    out("Writing G to stdout...");
    output_graph(&graph);

    out("Writing H to stdout...");
    output_graph(&minor);

    out("Launching Minor Detection Algorithm...");
    has_minor = graph_has_minor(&graph, &minor);

    char *result_str = malloc(sizeof(char*));
    sprintf(result_str, "G has minor H: %d", has_minor);
    out(result_str);

    // ...
    return EXIT_SUCCESS;
}

int k_disjoint_paths(graph_t **graph, graph_t **minor) {
    // Launch k dpp between G and H.
    // Need to find the 'general dynamic programming argument'
    // Also need a basic implemenation of k dpp, and a clear analogy between the two problems
    return 0;
}

int graph_has_minor(graph_t **graph, graph_t **minor) {
    bool result = 0;

    int g_vertices = (*graph)->num_vertices;
    int h_vertices = (*minor)->num_vertices;
    int g_edges = (*graph)->num_edges;
    int h_edges = (*minor)->num_edges;

    char *result_str = malloc(sizeof(char*));

    if(g_edges >= h_edges && g_vertices >= h_vertices) {
        result = k_disjoint_paths(graph, minor);
    } else {
        out("G failed preprocessing checks against H\n");
        sprintf(result_str, "%d vertices (G) against %d vertices (H). %d edges (G) against %d edges (H).\n", g_vertices, h_vertices, g_edges, h_edges);
        out(result_str);
    }
    return result; // First thing: Simple graph minor check alg. Goes here
}
