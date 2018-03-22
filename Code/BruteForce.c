#include "BruteForce.h"

void get_decomp_brute_force(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k) {
    out("Graph is small enough to permit brute-force evaluation.");
    (*decomposition)->treewidth_bounded = 0;

    graph_t* graph_copy = (graph_t*)malloc(sizeof(graph_t*));

    (graph_copy)->adjacencies = init_adjacencies(g_vertices);
    memcpy( (graph_copy)->adjacencies, (*graph)->adjacencies, g_vertices*sizeof(node_t*));   

    (graph_copy)->degrees = init_degrees(g_vertices);
    memcpy( (graph_copy)->degrees, (*graph)->degrees, g_vertices*sizeof(int));

    graph_copy->num_vertices = (*graph)->num_vertices;
    graph_copy->num_edges = (*graph)->num_edges;

    // See if the lower bound of the treewidth is higher than k
    int lb = 0;
    int removed_vertices = 0;
    int u; int v;

    int min_degree = g_vertices;
    int i;
    int new_degree;
    int contracted[g_vertices];
    memset(contracted, 0, sizeof(int)*g_vertices);

    while(removed_vertices < g_vertices) {
        min_degree = g_vertices;
        for(i = 0; i < g_vertices; i++) {
            if(!contracted[i] && graph_copy->degrees[i] <= min_degree) {
                min_degree = graph_copy->degrees[i];
                v = i;
            }
        }
        // we have v. find the minimum degree neighbour of v (call it u)
        node_t* curr_adj = graph_copy->adjacencies[v];
        min_degree = g_vertices;
        while(curr_adj) {
            if(curr_adj->value != v && !contracted[curr_adj->value] && graph_copy->degrees[curr_adj->value] <= min_degree) {
                min_degree = graph_copy->degrees[curr_adj->value];
                u = curr_adj->value;
            }
            curr_adj = curr_adj->next;
        }
        // now contract u and v
        new_degree = contract_edge(&graph_copy, v, u);
        removed_vertices++;
        contracted[u] = 1;
        graph_copy->degrees[v] = new_degree;
        lb = max(lb, graph_copy->degrees[v]);
    }

    // Gogate and Detchter Branch + Bound
    (*decomposition)->treewidth_bounded = lb <= k;
}
