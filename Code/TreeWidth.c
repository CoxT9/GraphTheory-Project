#include "TreeWidth.h"

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
                curr_neighbour = curr_node->value;
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

int contract_edge(graph_t **graph, int u, int v, int *new_degrees) {
    int degree = (*graph)->degrees[u] + (*graph)->degrees[v];
    node_t* contract_dest = (*graph)->adjacencies[u]; // contract into u
    node_t* contract_src = (*graph)->adjacencies[v]; // contract from v

    node_t* curr = contract_dest;
    while(curr->next) {
        curr = curr->next;
    }
    curr->next = contract_src;

    int present[(*graph)->num_vertices];
    memset(present, 0, (*graph)->num_vertices);
    node_t* curr = contract_dest;
    node_t* prev = NULL;

    while(curr) {
        if(present[curr->value]) {
            prev->next = curr->next;
            degree--;
        } else {
            present[curr->value] = 1;
            prev = curr;
        }
        curr = curr->next;
    }
    return degree;
}

graph_t *get_contracted_graph(graph_t **graph, int g_vertices, edgepair_t **matching, int matching_size) {
    graph_t *contracted_graph = (graph_t*)malloc(sizeof(graph_t*));
    *contracted_graph = **graph;
    
    int contracted[g_vertices];
    int new_degrees[g_vertices]
    memset(contracted, 0, g_vertices);
    memset(new_degrees, 0, g_vertices);
    // Since M is a matching, there is no chance of path contraction
    int i;
    int edge;
    int degree_sum = 0;
    edgepair_t* curr_contraction;
    for(i = 0; i < matching_size; i++) {
        curr_contraction = matching[i];
        degree_sum += contract_edge(&contracted_graph, curr_contraction->u, curr_contraction->v, new_degrees);
        contracted[curr_contraction->v] = 1;
    }

    // now copy over non-contracted edges into new list and make the new list the adj of the contracted graph
    node_t* new_adj[g_vertices - matching_size];
    int curr_vtx = 0;
    for(i = 0; i < g_vertices; i++) {
        if(!contracted[i]) {
            new_adj[curr_vtx] = contracted_graph->adjacencies[i];
            curr_vtx++;
        }
    }

    contracted_graph->adjacencies = new_adj;
    contracted_graph->num_vertices = g_vertices - matching_size;
    contracted_graph->degrees = new_degrees;
    contracted_graph->num_edges = degree_sum/2;

    return contracted_graph;
}

bool common_low_degree_neighbours(graph_t **graph, int w, int v, double degree_threshold) {
    node_t *w_adj = (*graph)->adjacencies[w];
    node_t *v_adj;
    total_low_degree_neighbours = 0;
    bool found;

    while(w_adj) {
        if( (*graph)->degrees[w_adj.value] < degree_threshold) {
            v_adj = (*graph)->adjacencies[v];
            found = FALSE;

            while(v_adj && !found) {
                if(v_adj.value = w_adj.value) {
                    found = TRUE;
                    total_low_degree_neighbours++;
                } else {
                    v_adj = v_adj->next;
                }
            }
        }
        w_adj = w_adj->next;
    }
    return total_low_degree_neighbours;
}

graph_t *get_improved_graph(graph_t **graph, int g_vertices, int k, double degree_threshold) {
    // if there is a pair v w where v w have k+1 common low degree neighbours, add edge vw
    graph_t* improved_graph = (graph_t*)malloc(sizeof(graph_t*));

    (improved_graph)->adjacencies = init_adjacencies(g_vertices);
    memcpy( (improved_graph)->adjacencies, (*graph)->adjacencies, g_vertices*sizeof(node_t*));   

    (improved_graph)->degrees = init_degrees(num_vertices);
    memcpy( (improved_graph)->degrees, (*improved_graph)->degrees, num_vertices*sizeof(int));

    (*graph)->num_vertices = g_vertices;
    (*graph)->num_edges = g_edges;

    // go thru the improved graph
    // if there is a pair v w where v w have k+1 common low degree neighbours, add edge vw
    int v;
    int w;
    for(v = 0; v < g_vertices; v++) {
        for(w = 0; w < g_vertices; w++) {
            // if v and w have at least k+1 low degree common neighbours, add a kw edge
            if(common_low_degree_neighbours(graph, w, v, degree_threshold) > k) {
                add_new_node(&improved_graph, w, v);
                add_new_node(&improved_graph, w, v);
                improved_graph->degrees[w]++;
                improved_graph->degrees[v]++;
            }
        }
    }
    return improved_graph;
}

void get_decomp_brute_force(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k) {
    out("Graph is small enough to permit brute-force evaluation.");
    (*decomposition)->treewidth_bounded = 0;
}

bool is_i_simplical(graph_t **graph, int v, double degree_threshold) {
    bool low_degree = (*improved_graph)->degrees[v] < degree_threshold;
    bool clique_neighbours = FALSE;
    if(low_degree) {
        // check for low degree neighbour
        node_t *adj = (*graph)->adjacencies[v];
        bool friendly = FALSE;
        while(adj && !friendly) {
            if((*graph)->degrees[adj.value] < degree_threshold) {
                friendly = TRUE;
            } else {
                adj = adj->next;
            }
        }

        if(friendly) {
            // check for clique neighbour
            clique_neighbours = TRUE;
            while(clique_neighbours && adj) {
                node_t *adj = (*graph)->adjacencies[v];
                if( !all_neighbours_present(graph, v, adj->value) {
                    clique_neighbours = FALSE;
                } else {
                    adj = adj->next;
                }
            }
            return clique_neighbours;
        }
    }
    return FALSE;
}

node_t *get_i_simplical_vertices(graph_t **improved_graph, double degree_threshold) {
    node_t *i_simp_list_head = NULL;
    node_t *curr = i_simp_list_head;
    int i;
    for(i = 0; i < (*improved_graph)->num_vertices; i++) {
        if(is_i_simplical(improved_graph, i)) {
            if((*improved_graph)->degrees[i] > k) {
                return NULL;
            }

            curr = (node_t*)malloc(sizeof(node_t*));
            curr->value = i;
            curr = curr->next; // may be wrong
        }
    }
    // find all the vertices in G_i that fit the i-simplical criteria
    return i_simp_list_head;
}

void drop_i_simplical_vertices(graph_t **improved_graph, node_t* i_simplical_vertices_list_head) {
    // for each v in isimp, remove v from g, adjust g as needed
    int g_vertices = (*improved_graph)->num_vertices
    int removals = 0;
    int deleted[g_vertices];
    memset(deleted, 0, g_vertices);

    int delete;
    node_t *curr = i_simplical_vertices_list_head;
    node_t *nbr;
    node_t *adj;
    while(curr) {
        removals++;
        delete = curr->value;
        deleted[curr->value] = 1;
        (*improved_graph)->num_edges -= (*improved_graph)->degrees[delete];
        nbr = (*improved_graph)->adjacencies[delete];
        while(nbr) {
            // for each neighbour, decrement degree and remove delete from adjacencies
            adj = (*improved_graph)->adjacencies[nbr->value];
            node_t* prev = NULL;
            while(adj) {
                if(adj->value == delete){
                    prev->next = adj->next;
                    adj = adj->next;
                } else {
                    prev = adj;
                    adj = adj->next;
                }
            }
            (*improved_graph)->degrees[nbr->value]--;
            nbr = nbr->next;
        }
    }

    node_t* new_adj[g_vertices - removals];
    int new_degrees[g_vertices - removals]
    int curr_vtx = 0;
    for(i = 0; i < g_vertices; i++) {
        if(!deleted[i]) {
            new_adj[curr_vtx] = (*improved_graph)->adjacencies[i];
            new_degrees[curr_vtx] = (*improved_graph)->degrees[i];
            curr_vtx++;
        }
    }

    (*improved_graph)->num_vertices -= removals;
    (*improved_graph)->adjacencies = new_adj;
    (*improved_graph)->degrees = new_degrees;
}

void get_tree_decomposition(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k) {
    // given G = (V,E) and k, determine if G has a treewidth bounded by k
    // if yes, give the decomposition
    // This comes from Bodlaender 1992.
    // this seems programatically correct but it is unclear if it is capturing the spirit of the algorithm.

    if(g_vertices < SMALL_GRAPH_SIZE) {
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
            get_tree_decomposition(&tw, &contraction, contraction->num_vertices, contraction->num_edges, k);

            if(tdc->treewidth_bounded) {
                // suppose G' got us k
                // Then we can get a 2k+1 on G
                tree_decomp_t* larger_decomposition = (tree_decomp_t*)malloc(sizeof(tree_decomp_t*));
                larger_decomposition->tree_root = tdc->tree_root;

                nested_node_t *curr = tdc->vertex_subsets;
                nested_node_t *curr_new = larger_decomposition->vertex_subsets;

                node_t *curr_xi_item;
                node_t *curr_yi_item;

                int x_vertex;
                int transformed_vertex;
                // ll transform and copy into new tree decomp

                while(curr) {
                    curr_new = (nested_node_t*)malloc(sizeof(nested_node_t*));

                    curr_xi_item = curr->value;
                    while(curr_xi_item) {
                        curr_yi_item = (node_t*)malloc(sizeof(node_t*));

                        if(!curr_new->value) {
                            curr_new->value = curr_yi_item;
                        }

                        x_vertex = curr_xi_item->value;
                        transformed_vertex = get_matching_repr(x_vertex, matching, matching_size);

                        curr_yi_item->value = transformed_vertex;
                        curr_xi_item = curr_xi_item->next;
                        curr_yi_item = curr_yi_item->next;
                    }
                    curr = curr->next;
                    curr_new = curr_new->next;
                }

                // finish with the alg in thm 2.10
            }
            
        } else {
            // graph improvement
            out("G has a low number of friendly vertices.");
            graph_t *improved = get_improved_graph(graph, g_vertices, k, degree_threshold);

            node_t *i_simplical_vertices_list = get_i_simplical_vertices(&improved); // if we find a k+1 i simp return null (stop condition)
            if(i_simplical_vertices_list && get_ll_size(&i_simplical_vertices_list) >= int(C2*g_vertices)) {
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
- finish high friendly vtx case
    - put last step into method
    - impl matching repr function
    - clarify alg from thm 2.10
- impl brute force tree decomp algorithm
*/


/* now:
- finish the alg
- run the alg
- test against sage, get more graphs
- put together slides
- put together report
*/
