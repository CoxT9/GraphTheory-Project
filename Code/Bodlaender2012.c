#include "Bodlaender2012.h"
// Bodlaender 2012: 2^n dynamic programming algorithm for tree width
// This is algorithm 4 of On Exact Algorithms for Treewidth

int found_in_tw_pair(treewidth_pair_t* curr_pair, int v) {
    int result = 0;
    if(curr_pair && curr_pair->set) {
        node_t* curr = curr_pair->set;
        result = ll_search(curr, v);
    }
    return result;
}

int get_treewidth_dp(graph_t **graph) {
    int n = (*graph)->num_vertices;
    int upper_bound = n-1;
    int* clique_vertices;
    int clique_size = max_clique(graph, &clique_vertices);
    int num_reachable_vertices;
    int i;
    int v;
    int r;
    int new_r;
    
    // each candidate has multiple pairs
    // so we have a set of innersets, each inner set is a pair: inner_2set, int
    generic_node_t* candidates[(*graph)->num_vertices];
    candidates[0] = (generic_node_t*)malloc(sizeof(generic_node_t*));

    treewidth_pair_t* start_pair = (treewidth_pair_t*)malloc(sizeof(treewidth_pair_t*));
    start_pair->set = NULL;
    start_pair->width = INT_MIN;

    candidates[0]->data = (void*)start_pair;
    candidates[0]->next = NULL;
    for(i = 1; i <= ((*graph)->num_vertices - clique_size); i++) {
        candidates[i] = NULL;//(generic_node_t*)malloc(sizeof(generic_node_t*));
       // candidates[i]->data = NULL; // no pairs in tw_i
       // candidates[i]->next = NULL;

        generic_node_t* prev_tw = candidates[i-1];
        treewidth_pair_t* prev_pair;
        treewidth_pair_t* new_pair;

        while(prev_tw) {
            prev_pair = (treewidth_pair_t*)(prev_tw->data);
            for(v = 0; v < (*graph)->num_vertices; v++) {
                int found = found_in_tw_pair(prev_pair, v);
                if(!found) {
                    r = prev_pair->width;
                    // Q(S,x) is the set of vertices in V - S - {x} that are reachable from x with paths using vertices in S. DFS
                    num_reachable_vertices = get_total_reachable_vertices(graph, prev_pair, v);

                    new_r = max(r, num_reachable_vertices);
                    if(new_r < upper_bound) {
                        upper_bound = min(upper_bound, (*graph)->num_vertices-i-1);
                        treewidth_pair_t* target_pair = NULL;
                        if(candidates[i] && candidates[i]->data) {
                            target_pair = find_set_in_candidate((*graph)->num_vertices, prev_pair->set, v, candidates[i]);
                        }
                        if(target_pair) { // if there is pair S+x, t in TWi, then replace with S+x, min(t, r')
                            int min_value = min(target_pair->width, new_r);
                            target_pair->width = min_value;
                        } else { // otherwise, insert (S+x, r') into TWi, not i-1
                            // first, get S+x, r' into a new struct
                            new_pair = (treewidth_pair_t*)malloc(sizeof(treewidth_pair_t*));
                            new_pair->width = new_r;

                            node_t* head = (node_t*)malloc(sizeof(node_t*));
                            head->value = v;
                            new_pair->set = head;

                            node_t* curr_dest = head;
                            node_t* curr_src = prev_pair->set;
                            node_t* newnode;
                            while(curr_src) {
                                newnode = (node_t*)malloc(sizeof(node_t*));
                                newnode->value = curr_src->value;
                                curr_dest->next = newnode;

                                curr_src = curr_src->next;
                                curr_dest = curr_dest->next;
                            }
                            if(!candidates[i] || (candidates[i] && !candidates[i]->data)) {
                                candidates[i] = (generic_node_t*)malloc(sizeof(generic_node_t*));
                                candidates[i]->data = new_pair;
                            } else {
                                generic_node_t* new_generic_node = (generic_node_t*)malloc(sizeof(generic_node_t*));
                                new_generic_node->data = (void*)new_pair;
                                new_generic_node->next = candidates[i];
                                candidates[i] = new_generic_node;
                            }
                        }
                    }
                }
            }
            prev_tw = prev_tw->next;
        }
    }
    node_t* vertex_list = NULL;
    int u;
    for(u = 0; u < (*graph)->num_vertices; u++) {
        if(!clique_vertices[u]) {
            node_t* newnode = (node_t*)malloc(sizeof(node_t*));
            newnode->value = u;
            if(!vertex_list) {
                vertex_list = newnode;
            } else {
                vertex_list->next = newnode;
                vertex_list = vertex_list->next;
            }
        }
    } // vertex_list is now V - C

    treewidth_pair_t* final = find_set_in_candidate((*graph)->num_vertices, vertex_list, -1, candidates[(*graph)->num_vertices - clique_size]);
    int result = upper_bound;
    if(final) {
        result = final->width;
    }
    return result;
}

treewidth_pair_t* find_set_in_candidate(int size, node_t* head, int new_vtx, generic_node_t* candidate_list) {
    // set new_vtx -1 if we are using the end case, otherwise append
    if(!head || !candidate_list || !candidate_list->data) {
        return NULL;
    }

    if(new_vtx >= 0) {
        node_t* newnode = (node_t*)malloc(sizeof(node_t*));
        newnode->value = new_vtx;
        newnode->next = head;
        head = newnode;
    }

    treewidth_pair_t* result = NULL;
    // see if every int in the linked list (starts at head) can be found in one of the candidate_list's sets
    // return the set that has these values, or NULL
    generic_node_t* curr_tw = candidate_list;

    while(curr_tw && !result) {
        // if all present of head in curr_tw->set, set result to curr_tw

        treewidth_pair_t* curr_pair = (treewidth_pair_t*)(curr_tw->data);
        int present = all_present(size, curr_pair->set, head);
        if(present) {
            result = curr_pair;
        } else {
            curr_tw = curr_tw->next;
        }
    }
    return result;
}

bool all_present(int n, node_t* head1, node_t* head2) {
    // make sure all entries of head1 are in head2
    // we want to make sure that the linked lists have the same entries
    // we can do this in O(n)
    // 1: make 0'd out array of size N
    // walk h1 and ++ every element
    // walk h2 and -- every element
    // walk the array. if all entries are 0: true (otherwise, a 1 indicates h1 and not h2, -1 indicates h2 and not h1)
    int present[n];
    memset(present, 0, n*sizeof(int));

    node_t* curr = head1;
    while(curr) {
        present[curr->value]++;
        curr = curr->next;
    }
    curr = head2;
    while(curr) {
        present[curr->value]--;
        curr = curr->next;
    }
    bool same_entries = TRUE;
    int i = 0;
    while(i < n && same_entries) {
        same_entries = present[i] == 0;
        i++;
    }
    return same_entries;
}

int get_total_reachable_vertices(graph_t **graph, treewidth_pair_t* tw_pair, int start) {
    // Q(S,x) is the set of vertices in V - S - {x} that are reachable from x with paths using vertices in S. DFS
    int total_reachable = 0;
    int u;
    int v;
    bool found = FALSE;
    node_t* adj;

    int inner[(*graph)->num_vertices];
    memset(inner, 0, sizeof(int)*(*graph)->num_vertices);

    int visited[(*graph)->num_vertices];
    memset(visited, 0, sizeof(int)*(*graph)->num_vertices);

    int stack[(*graph)->num_vertices];
    memset(stack, 0, sizeof(int)*(*graph)->num_vertices);

    int top = -1;

    node_t* curr = tw_pair->set;
    while(curr) {
        inner[curr->value] = 1;
        curr = curr->next;
    }

    stack[++top] = start;
    visited[start] = 1;
    while(top >= 0) { // DFS for the 'finger' vertices
        u = stack[top];
        v = -1;
        adj = (*graph)->adjacencies[u];
        found = FALSE;
        while(!found && adj) {
            if(!visited[adj->value]) {
                v = adj->value;
                found = TRUE;
            } else {
                adj = adj->next;
            }
        }
        if(found) {
            visited[v] = 1;
            // if v is a finger, incr, else push
            if(!inner[v]) {
                total_reachable++;
            } else {
                stack[++top] = v;
            }
        } else {
            top--;
        }
    }

    return total_reachable;
}

void max_clique_recurse(graph_t **graph, int *curr_clique, int *curr_size, int *clique_vertices, int *max_size, int depth) {
    int i;
    int connected;
    if(depth == (*graph)->num_vertices) {
        memcpy(clique_vertices, curr_clique, (*graph)->num_vertices*sizeof(int));
        *max_size = *curr_size;
    } else {
        connected = 1;
        for(i = 0; i < depth && connected; i++) {
            if(curr_clique[i] && !has_neighbour(graph, depth, i)) {
                connected = 0;
            }
        }

        if(connected) {
            curr_clique[depth] = 1;
            (*curr_size)++;
            max_clique_recurse(graph, curr_clique, curr_size, clique_vertices, max_size, depth+1);
            (*curr_size)--;
        }

        if(*curr_size + (*graph)->num_vertices - depth > *max_size) {
            curr_clique[depth] = 0;
            max_clique_recurse(graph, curr_clique, curr_size, clique_vertices, max_size, depth+1);
        }
    }
}

int max_clique(graph_t **graph, int** clique_vertices) {
    // credit to Martin Broadhurst
    int max_size = 0;
    int curr_size = 0;
    int* curr_clique = malloc((*graph)->num_vertices*sizeof(int));
    *clique_vertices = malloc((*graph)->num_vertices*sizeof(int));

    max_clique_recurse(graph, curr_clique, &curr_size, *clique_vertices, &max_size, 0);
    return max_size;
}

void get_decomp_brute_force(tree_decomp_t **decomposition, graph_t **graph, int g_vertices, int g_edges, int k) {
    out("G is small enough to permit brute-force evaluation.");
    int tw = get_treewidth_dp(graph);
    sprintf(out_buffer, "Exponential Time Dynamic Programming Yielded a Tree-Width Result of %d\n", tw);
    out(out_buffer);
    (*decomposition)->treewidth_bounded = tw <= k; // bounded: G has a tw upper bound of k
}
