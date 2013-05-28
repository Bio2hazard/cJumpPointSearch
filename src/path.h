#ifndef __included_path_h
#define __included_path_h

#include "jps_grid.h"
#include "heap.h"

/* Check path.cpp for the option of turning on debug output! */

/* for counting mallocs: */
extern int malloc_count;

/* The 2 Algorithms used */
int euclidean(int dx, int dy);
int manhattan(int dx, int dy);

/* Jumps recursively into one direction until a suitable successor has been found. */
int *_jump(struct grid *gd, int x, int y, int px, int py, struct node *endNode);

/* Finds successor nodes worth investigating */
void _identifySuccessors(struct grid *gd, struct node *activeNode, struct open_list *current, struct node *endNode);

/* Is used once the goal has been reached - creates a list of all nodes passed to reach the goal */
struct neighbor_xy_list *backtrace(struct node *activeNode);

/* Computes the path. Returns a list of X/Y coordinates that form the path. */
struct neighbor_xy_list *findPath(struct grid *gd, int startX, int startY, int endX, int endY);

/* Smoothens the path */
struct neighbor_xy_list *smooth_path(struct grid *gd, struct neighbor_xy_list *head);

#endif
