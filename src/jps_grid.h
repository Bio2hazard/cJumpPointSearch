#ifndef __included_jps_grid_h
#define __included_jps_grid_h

#include <stdbool.h>

extern int malloc_count;

/* Contains all relevant information for a position in the grid */
struct node {
	int x;
	int y;
	bool walkable;
	int g;
	int h;
	int f;
	bool opened;
	bool closed;
	struct node *parent;
};

/* Forms the grid */
struct grid {
	int width, height;
	struct node **nodes;
};

/* All neighbors of a node - in node form */
struct neighbor_list {
	struct neighbor_list *left;
	struct neighbor_list *right;
	struct node *neighbor_node;
};

/* New List */
struct neighbor_list *new_neighbor_list();

/* Clean list */
void clean_neighbor_list(struct neighbor_list *head);

/* Add Entry */
struct neighbor_list *insert_right(struct neighbor_list *list, struct node *data);

/* New Node */
struct node createNode(int x, int y, bool walkable);

/* Create the grid based on with, height and a matrix */
struct grid createGrid(int width, int height, bool **matrix);

/* Build the 2D node grid from the matrix */
struct node **_buildNodes(int width, int height, bool **matrix);

/* Return a pointer to a node, identified by the x and y coordinates */
struct node *getNodeAt(struct grid *gd, int x, int y);

/* Return True / False whether a node is walkable or not ( automatically performs isInside check ) */
bool isWalkableAt(struct grid *gd, int x, int y);

/* Return True / False whether a node is inside the grid or not */
bool isInside(struct grid *gd, int x, int y);

/* Allows you to manually set a cell to walkable TRUE / FALSE. Generally not used. */
void setWalkableAt(struct grid *gd, int x, int y, bool walkable);

/* Return a list of neighbors in node-format */
struct neighbor_list *getNeighbors(struct grid *gd, struct node *nd);

#endif
