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

    if(argc != REQUIRED_ARGS + 1) {
        sprintf(out_buffer, BAD_ARGS_STR, argv[0]);
        out(out_buffer);
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

    sprintf(out_buffer, "G has minor H: %d", has_minor);
    out(out_buffer);

    // ...
    return EXIT_SUCCESS;
}

void get_tree_decomposition(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k) {
    // given G = (V,E) and k, determine if G has a treewidth bounded by k
    // if yes, also give the composition
    // This comes from Bodlaender 1992. In this original paper, he says to use 'any finite algorithm' for suff. small graphs
    // However, it's not clear why we would do that. The other TW algs are exponential. Stick with the linear one unless there is a problem with small graphs.
    // will need to impl the alg and decide what a tree decomp 'looks' like.
    (*decomposition)->treewidth_bounded = 0;
    out("Determining the tree decomposition of G...");

    if( g_edges <= k * g_vertices - (double)(k*(k+1))/2 ) {
        out("The treewidth is possibly bounded...");
    }
}

int k_disjoint_paths(graph_t **graph, graph_t **minor, int g_vertices, int g_edges, int h_vertices, int h_edges) {
    int treewidth_k;
    tree_decomp_t *decomposition;
    int tw_bounded;

    treewidth_k = h_edges;
    decomposition = malloc(sizeof(tree_decomp_t*));

    get_tree_decomposition(&decomposition, graph, g_vertices, g_edges, treewidth_k);
    tw_bounded = decomposition->treewidth_bounded;


    // the first thing Kawarabayashi et al did was see if G has a bounded tree width
    // in this case, G is bounded by H. Ie: k = E(H) (will need to confirm this)
    return tw_bounded;
}

int graph_has_minor(graph_t **graph, graph_t **minor) {
    bool result = 0;

    int g_vertices = (*graph)->num_vertices;
    int h_vertices = (*minor)->num_vertices;
    int g_edges = (*graph)->num_edges;
    int h_edges = (*minor)->num_edges;

    if(g_edges >= h_edges && g_vertices >= h_vertices) {
        result = k_disjoint_paths(graph, minor, g_vertices, g_edges, h_vertices, h_edges);
    } else {
        out("G failed preprocessing checks against H");
        sprintf(out_buffer, "%d vertices (G) against %d vertices (H). %d edges (G) against %d edges (H).", g_vertices, h_vertices, g_edges, h_edges);
        out(out_buffer);
    }
    return result; // First thing: Simple graph minor check alg. Goes here
}
