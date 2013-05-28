#include <stdio.h>
#include <stdbool.h>

#include "neighbors.h"
#include "jps_grid.h"
#include "heap.h"

void displaySolution(struct grid *gd, struct neighbor_xy_list *path)
{
	int i, j;
	bool found = false;
	struct neighbor_xy_list *path_pos = path;
	for (i = 0; i < gd->height; i++) {
		for (j = 0; j < gd->width; ++j) {
			if (gd->nodes[i][j].walkable) {
				while (path != (path_pos = path_pos->left)) {
					if (path_pos->y == i && path_pos->x == j) {
						printf("o");
						found = true;
					}
				}
				if (!found)
					printf(".");
			} else
				printf("#");

			found = false;
		}
		printf("\n");
	}
}

void displayGrid(struct grid *gd)
{
	int i, j;
	for (i = 0; i < gd->height; i++) {
		for (j = 0; j < gd->width; ++j) {
			if (gd->nodes[i][j].walkable)
				printf(".");
			else
				printf("#");
		}
		printf("\n");
	}
}

void displayNodeInfo(struct node *n)
{
	printf("x: %i ", n->x);
	printf("\ny: %i ", n->y);
	printf("\nf: %i ", n->f);
	if (n->walkable)
		printf("\nwalkable: yes\n\n");
	else
		printf("\nwalkable: no\n\n");
}

void listNeighbors(struct neighbor_list *list)
{
	struct neighbor_list *head = list;
	struct neighbor_list *current = list;
	while (head != (current = current->right)) {
		displayNodeInfo(current->neighbor_node);
	}
}

void listOpenList(struct open_list *list)
{
	struct open_list *head = list;
	struct open_list *current = list;
	while (head != (current = current->right)) {
		displayNodeInfo(current->list_node);
	}
}
