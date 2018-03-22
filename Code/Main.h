#ifndef _GRAPH_MINOR_H_
#define _GRAPH_MINOR_H_

#include "ProjectUtil.h"
#include "TreeWidth.h"

#define REQUIRED_ARGS 2
#define BAD_ARGS_STR "Usage: %s <Path-To-Graph> <K>"

int graph_has_bounded_tw(graph_t **graph, int treewidth_k);
#endif
