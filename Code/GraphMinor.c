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

int k_disjoint_paths(graph_t **graph, graph_t **minor, int g_vertices, int g_edges, int h_vertices, int h_edges) {
    // Launch k dpp between G and H.
    // Need to find the 'general dynamic programming argument'
    // Also need a basic implemenation of k dpp

    // first pass: for each u in H, is there an unvisited v in G with same degree as u?
    bool paths_exist = 1;
    bool found;
    int u;
    int v;

    int visited[g_vertices];
    int i;
    for(i = 0; i < g_vertices; i++) {
        visited[i] = 0;
    }

    for(u = 0; u < h_vertices && paths_exist; u++) {
        // for each u in H, does there exist an unvisited v in G with same degree as u?
        // this may not work, but a good starting point
        found = 0;
        v = 0;
        while(v < g_vertices && !found) {
            if(!visited[v]) {
                visited[v] = 1;
                if(get_degree(graph, v, g_vertices) >= get_degree(minor, u, h_vertices)) { // get degree of v
                    found = 1;
                }
            }
            v++;
        }
        if(!found) {
            paths_exist = 0;
        }
    }
    // first, do the simple "unvisited degree match"

    // pick a node of H
    // BFS out of the node, collect all the outgoing paths
    // does there exist a node in G with that number of outgoing paths?
    // shuffling: may have to backtrack
    // then have to connect it all

    // plan:
    // 1: outgoing path match. is there an unvisited vertex v with i outgoing paths?
    // 2: backtrack: if no unvisited vertex v exists, pop the stack and find a different option somewhere and try that (this may be where DP comes into play)
    // 3: path joining. Apply the constraint that some src's are other's dests. 
    // Then we'll do advanced disjoint paths, tree width for example
    return paths_exist;
}

int graph_has_minor(graph_t **graph, graph_t **minor) {
    bool result = 0;

    int g_vertices = (*graph)->num_vertices;
    int h_vertices = (*minor)->num_vertices;
    int g_edges = (*graph)->num_edges;
    int h_edges = (*minor)->num_edges;

    char *result_str = malloc(sizeof(char*));

    if(g_edges >= h_edges && g_vertices >= h_vertices) {
        result = k_disjoint_paths(graph, minor, g_vertices, g_edges, h_vertices, h_edges);
    } else {
        out("G failed preprocessing checks against H\n");
        sprintf(result_str, "%d vertices (G) against %d vertices (H). %d edges (G) against %d edges (H).\n", g_vertices, h_vertices, g_edges, h_edges);
        out(result_str);
    }
    return result; // First thing: Simple graph minor check alg. Goes here
}
