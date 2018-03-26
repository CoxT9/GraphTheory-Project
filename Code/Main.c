/* Given a graph G, does G have a bounded tree width? If so, give the (minimum) tree decomposition of G

NOW:
- Gather a suite of results
Presentation:
- proofs
- algorithms
- the process
- there is definitely 20 minutes here. Look at how cool the algorithms are!
- report: two implementations and compare. One worked one didnt. Why? Tradeoffs?
*/

#include "Main.h"

int main(int argc, char** argv) {
    graph_t *graph = malloc(sizeof(graph_t*));
    int tree_width_bounded = 0; 

    if(argc != REQUIRED_ARGS + 1) {
        printf("%d\n", argc);
        sprintf(out_buffer, BAD_ARGS_STR, argv[0]);
        out(out_buffer);
        return EXIT_SUCCESS;
    }

    int tw_k = atoi(argv[2]);
    sprintf(out_buffer, "Parameter K set to %d\n", tw_k);
    out(out_buffer);

    int reflective = atoi(argv[3]);

    out("Launching Tree Decomposition program...");
    generate_graph(&graph, argv[1], reflective);

    out("Writing G to stdout...");
    output_graph(&graph);

    out("Launching Tree Decomposition Detection Algorithm...");
    tree_width_bounded = graph_has_bounded_tw(&graph, tw_k);

    sprintf(out_buffer, "G has bounded tree width: %d", tree_width_bounded);
    // output the tree decomposition here
    out(out_buffer);

    // ...
    return EXIT_SUCCESS;
}

int graph_has_bounded_tw(graph_t **graph, int treewidth_k) {
    int g_vertices = (*graph)->num_vertices;
    int g_edges = (*graph)->num_edges;

    tree_decomp_t *decomposition;
    int tw_bounded;

    decomposition = (tree_decomp_t*)malloc(sizeof(tree_decomp_t*));

    get_tree_decomposition(&decomposition, graph, g_vertices, g_edges, treewidth_k);
    tw_bounded = decomposition->treewidth_bounded;

    return tw_bounded;
}
