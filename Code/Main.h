#ifndef _GRAPH_MINOR_H_
#define _GRAPH_MINOR_H_

#include "ProjectUtil.h"
#include "TreeWidth.h"

#define REQUIRED_ARGS 1
#define BAD_ARGS_STR "Usage: %s <Path-To-Graph>"

int graph_has_bounded_tw(graph_t **graph);
#endif
