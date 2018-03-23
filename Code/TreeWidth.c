#include "TreeWidth.h"

void get_tree_decomposition(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k) {
    // given G = (V,E) and k, determine if G has a treewidth bounded by k
    // if yes, give the decomposition
    // This comes from Bodlaender 1992.
    // this seems programatically correct but it is unclear if it is capturing the spirit of the algorithm.


    if(g_vertices <= SMALL_GRAPH_SIZE) {
        get_decomp_brute_force(decomposition, graph, g_vertices, g_edges, k);
        return;
    }

    double degree_threshold = fmax( (k*k + 4*k + 4), ceil( (k*2)/C1 ) );
    int num_friendly_vertices;

    double num_friendly_vertices_threshold = g_vertices/(4*k*k + 12*k + 16);

    (*decomposition)->treewidth_bounded = 0; // Assume unbounded unless the algorithm succeeds
    out("Determining the tree decomposition of G...");

    if(g_edges <= k * g_vertices - (double)(k*(k+1))/2 ) {
        out("The tree width of G is possibly bounded by E(H)");

        num_friendly_vertices = get_total_friendly_vertices(graph, g_vertices, g_edges, degree_threshold);
        if(num_friendly_vertices >= num_friendly_vertices_threshold) {
            out("G has a high number of friendly vertices.");
            // maximal matching
            edgepair_t **matching = (edgepair_t**)malloc(sizeof(edgepair_t)*g_vertices);
            int matching_size = get_maximal_matching(matching, graph, g_vertices); // greedy-style matching

            // contract edges in M from G. so get G . M
            graph_t *contraction = get_contracted_graph(graph, g_vertices, matching, matching_size);
            sprintf(out_buffer, " Found contracted graph of size %d", contraction->num_vertices);
            out(out_buffer);

            // recurse against G'
            tree_decomp_t* tdc = (tree_decomp_t*)malloc(sizeof(tree_decomp_t*));
            get_tree_decomposition(&tdc, &contraction, contraction->num_vertices, contraction->num_edges, k);

            if(tdc->treewidth_bounded) {
                // suppose G' got us k
                // Then we can get a 2k+1 on G
                tree_decomp_t* larger_decomposition = (tree_decomp_t*)malloc(sizeof(tree_decomp_t*));
                generate_candidate_original_treedecomp(&larger_decomposition, &tdc, matching, matching_size);

                // larger_decomp now has a 2k+1 tree decomp
                // finish with the alg in thm 2.10
                out("G has tree width at most 2k+1");
            }
            
        } else {
            // graph improvement
            out("G has a low number of friendly vertices.");
            graph_t *improved = get_improved_graph(graph, g_vertices, k, degree_threshold);

            node_t *i_simplical_vertices_list = get_i_simplical_vertices(&improved, degree_threshold, k); // if we find a k+1 i simp return null (stop condition)
            if(i_simplical_vertices_list && get_ll_size(&i_simplical_vertices_list) >= C2*g_vertices) {
                // drop all i simp vertices from G'
                drop_i_simplical_vertices(&improved, i_simplical_vertices_list);

                // recurse against G'-i
                tree_decomp_t* tdc = (tree_decomp_t*)malloc(sizeof(tree_decomp_t*));
                get_tree_decomposition(&tdc, &improved, improved->num_vertices, improved->num_edges, k);
               
                // last check
                if(tdc->treewidth_bounded) {
                    (*decomposition)->treewidth_bounded = 1;
                }
            }
        }
    }
}

/* to finish alg:
*/


/* now:
- run the alg
- test against sage, get more graphs
- put together slides
- put together report
*/
