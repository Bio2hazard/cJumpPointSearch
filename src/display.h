#ifndef __included_display_h
#define __included_display_h

#include "jps_grid.h"
#include "neighbors.h"
#include "heap.h"

/* Draws the grid / map */
void displayGrid(struct grid *gd);

/* Displays rudimentary information about a given node */
void displayNodeInfo(struct node *n);

/* Draws the grid / map with the computed path */
void displaySolution(struct grid *gd, struct neighbor_xy_list *path);

/* Lists information about all walkable nodes adjecent to a node */
void listNeighbors(struct neighbor_list *list);

/* Lists all entry in the open list ( contains eligible nodes for inspection ) */
void listOpenList(struct open_list *list);

#endif
