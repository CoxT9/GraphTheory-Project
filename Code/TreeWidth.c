#include "TreeWidth.h"

const double c1 = 0.5;

int get_total_friendly_vertices(graph_t **graph, int g_vertices, int g_edges, double degree_threshold) {
    int total_friendly_vertices = 0;
    int v;
    int u;
    bool found_neighbour = FALSE;
    for(v = 0; v < g_vertices; v++) {
        if((*graph)->degrees[v] < degree_threshold) {
            u = 0;
            while(u < g_vertices-1 && !found_neighbour) {
                if(u != v) {
                    found_neighbour = (*graph)->degrees[u] < degree_threshold;
                }
                u++;
            }
            if(found_neighbour) {
                // v is a friendly vertex
                total_friendly_vertices++;
                found_neighbour = FALSE;
            }
        }
    }
    return total_friendly_vertices;
}

int get_maximal_matching(edgepair_t **matching, graph_t **graph, int num_vertices) {
    // fill up a very large array then copy memory over
    // the advantage of an array over a linked list is that while the ll will be faster to build, the array is indexible which we need
    // if it becomes the case that indexing into the matching is not necessary, this may (should) change
    // the approach: iterate over all vertices and check if there is an available pair. runs in O(E)
    node_t *curr_node;
    bool found_edge;
    int size = 0;
    int i;
    int visited[num_vertices];
    int curr_neighbour;
    for(i = 0; i < num_vertices; i++) {
        visited[i] = 0;
    }

    for(i = 0; i < num_vertices; i++) {
        if(!visited[i]) {

            // find an unvisited neighbour. if one is found, add a new pair and mark that neighbour visited
            found_edge = FALSE;
            curr_node = (*graph)->adjacencies[i];
            while(curr_node && !found_edge) {
                curr_neighbour = curr_node->vertex_id;
                if(!visited[curr_neighbour]) {
                    visited[curr_neighbour] = 1;
                    found_edge = TRUE;

                    matching[size] = (edgepair_t*)malloc(sizeof(edgepair_t*));
                    matching[size]->u = i;
                    matching[size]->v = curr_neighbour;
                    size++;
                }
                curr_node = curr_node->next;
            }
        }
    }
    edgepair_t **result = (edgepair_t**)malloc(sizeof(edgepair_t*)*size);
    memcpy( result, matching, size*sizeof(edgepair_t*));
    matching = result; // there might be some pointer-array gotchas here
    return size;
}

graph_t *get_contracted_graph(graph_t **graph, int g_vertices, edgepair_t **matching, int matching_size) {
    // this is what's next
    graph_t *contracted_graph = (graph_t*)malloc(sizeof(graph_t*));
    *contracted_graph = **graph;
    return contracted_graph;
}

void get_tree_decomposition(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k) {
    // given G = (V,E) and k, determine if G has a treewidth bounded by k
    // if yes, also give the composition
    // This comes from Bodlaender 1992. In this original paper, he says to use 'any finite algorithm' for suff. small graphs
    // However, it's not clear why we would do that. The other TW algs are exponential. Stick with the linear one unless there is a problem with small graphs.
    // will need to impl the alg and decide what a tree decomp 'looks' like.
    // this seems programatically correct but it is unclear if it is capturing the spirit of the algorithm.
    // example: the tree width of K5 is not bounded by E(K4). This seems correct but need to find an example of something actually running
    // it is still not a 100% guarantee that k is E(H). A lot of these numbers in fact seem odd. Sample data will be very helpful here
    // a conceptual gap here is that the algorithm apparently yields tree decompositions. need to determine how this is the case.
    // const double c2 = 0.5;

    double degree_threshold = fmax( (k*k + 4*k + 4), ceil( (k*2)/c1 ) );
    int num_friendly_vertices;

    double num_friendly_vertices_threshold = g_vertices/(4*k*k + 12*k + 16);

    (*decomposition)->treewidth_bounded = 0;
    out("Determining the tree decomposition of G...");

    if(g_edges <= k * g_vertices - (double)(k*(k+1))/2 ) {
        out("The tree width of G is possibly bounded by E(H)");

        num_friendly_vertices = get_total_friendly_vertices(graph, g_vertices, g_edges, degree_threshold);
        if(num_friendly_vertices >= num_friendly_vertices_threshold) {
            // maximal matching
            edgepair_t **matching = (edgepair_t**)malloc(sizeof(edgepair_t)*g_vertices);
            int matching_size = get_maximal_matching(matching, graph, g_vertices); // greedy-style matching
            
            // contract edges in M from G. so get G . M
            graph_t *contraction = get_contracted_graph(graph, g_vertices, matching, matching_size);
            sprintf(out_buffer, "%d\n", contraction->num_vertices);
            out(out_buffer);

            out("G has a high number of friendly vertices.");           
        } else {
            // graph improvement
            out("G has a low number of friendly vertices.");
        }
    }
}

// The scope of this project has been greatly reduced.
// Next steps:
/*
- read the entire paper in detail
- finish off the algorithm, make sure it gives the decomposition too
- test its correctness and performance against sage
- put together slides and report. done
*/