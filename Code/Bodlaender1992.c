#include "Bodlaender1992.h"

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

graph_t *get_contracted_graph(graph_t **graph, int g_vertices, edgepair_t **matching, int matching_size) {
    graph_t *contracted_graph = (graph_t*)malloc(sizeof(graph_t*));
    *contracted_graph = **graph;
    
    int contracted[g_vertices];
    int new_degrees[g_vertices];
    memset(contracted, 0, g_vertices);
    memset(new_degrees, 0, g_vertices);
    // Since M is a matching, there is no chance of path contraction
    int i;
    int degree_sum = 0;
    int u_degree = 0;
    edgepair_t* curr_contraction;
    for(i = 0; i < matching_size; i++) {
        curr_contraction = matching[i];
        u_degree = contract_edge(&contracted_graph, curr_contraction->u, curr_contraction->v);
        degree_sum += u_degree;
        contracted[curr_contraction->v] = 1;
        new_degrees[curr_contraction->u] = u_degree;
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
    int total_low_degree_neighbours = 0;
    bool found;

    while(w_adj){
        if( (*graph)->degrees[w_adj->value] < degree_threshold) {
            v_adj = (*graph)->adjacencies[v];
            found = FALSE;

            while(v_adj && !found) {
                if(v_adj->value == w_adj->value) {
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

    (improved_graph)->degrees = init_degrees(g_vertices);
    memcpy( (improved_graph)->degrees, (*graph)->degrees, g_vertices*sizeof(int));

    (improved_graph)->num_vertices = g_vertices;
    (improved_graph)->num_edges = (*graph)->num_edges;

    // go thru the improved graph
    // if there is a pair v w where v w have k+1 common low degree neighbours, add edge vw
    int v;
    int w;
    for(v = 0; v < g_vertices; v++) {
        for(w = 0; w < g_vertices; w++) {
            // if v and w have at least k+1 low degree common neighbours, add a kw edge
            if(common_low_degree_neighbours(graph, w, v, degree_threshold) > k) {
                add_new_node(improved_graph->adjacencies, w, v);
                add_new_node(improved_graph->adjacencies, v, w);
                improved_graph->degrees[w]++;
                improved_graph->degrees[v]++;
            }
        }
    }
    return improved_graph;
}

bool is_i_simplical(graph_t **graph, int v, double degree_threshold) {
    bool low_degree = (*graph)->degrees[v] < degree_threshold;
    bool clique_neighbours = FALSE;
    if(low_degree) {
        // check for low degree neighbour
        node_t *adj = (*graph)->adjacencies[v];
        bool friendly = FALSE;
        while(adj && !friendly) {
            if((*graph)->degrees[adj->value] < degree_threshold) {
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
                if( !all_neighbours_present(graph, v, adj->value)) {
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

node_t *get_i_simplical_vertices(graph_t **improved_graph, double degree_threshold, int k) {
    node_t *i_simp_list_head = NULL;
    node_t *curr = i_simp_list_head;
    int i;
    for(i = 0; i < (*improved_graph)->num_vertices; i++) {
        if(is_i_simplical(improved_graph, i, degree_threshold)) {
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
    int g_vertices = (*improved_graph)->num_vertices;
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
    int new_degrees[g_vertices - removals];
    int curr_vtx = 0;
    int i;
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

int get_matching_repr(int x_vertex, edgepair_t** matching, int matching_size) {
    // if x was folded into a matching, return matching result instead
    // if x was not an endpoint of a matching, return x, otherwise return u of the matching it was a member (v) of
    int index = 0;
    bool found = FALSE;
    while(index < matching_size && !found) {
        if(matching[index]->v == x_vertex) {
            found = TRUE;
        } else {
            index++;
        }
    }

    int result = x_vertex;
    if(found) {
        result = matching[index]->u;
    } 
    return result;
}

void generate_candidate_original_treedecomp(tree_decomp_t **larger_decomposition, tree_decomp_t **tdc, edgepair_t** matching, int matching_size) {
    (*larger_decomposition)->tree_root = (*tdc)->tree_root;

    nested_node_t *curr = (*tdc)->vertex_subsets;
    nested_node_t *curr_new = (*larger_decomposition)->vertex_subsets;

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
}