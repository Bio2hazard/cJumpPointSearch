#include <stdlib.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#include "jps_grid.h"
#include "neighbors.h"
#include "display.h"

struct neighbor_xy_list *neighbor_xy_new()
{
	struct neighbor_xy_list *newlist = (struct neighbor_xy_list *) malloc(sizeof(struct neighbor_xy_list));
	malloc_count++; /* [ Malloc Count ] */
	newlist->right = newlist;
	newlist->left = newlist;
	newlist->x = 0;
	newlist->y = 0;
	return newlist;
}

void neighbor_xy_clean(struct neighbor_xy_list *head)
{
	if (head != NULL) {
		struct neighbor_xy_list *pos = head;
		struct neighbor_xy_list *tmp = head;
		do {
			tmp = pos->right;
			free(pos);
			malloc_count--; /* [ Malloc Count ] */
			pos = tmp;
		} while (pos != head);
	}
}

struct neighbor_xy_list *neighbor_xy_insert_right(struct neighbor_xy_list *list, int x, int y)
{
	struct neighbor_xy_list *newlist = (struct neighbor_xy_list *) malloc(sizeof(struct neighbor_xy_list));
	malloc_count++;         /* [ Malloc Count ] */
	newlist->x = x;
	newlist->y = y;
	newlist->left = list;
	newlist->right = list->right;
	list->right = newlist;
	newlist->right->left = newlist;
	return newlist;
}

struct neighbor_xy_list *_findNeighbors(struct grid *gd, struct node *activeNode)
{
	struct node *parent = activeNode->parent;
	int x = activeNode->x;
	int y = activeNode->y;
	int px, py, dx, dy;

	struct neighbor_xy_list *head = neighbor_xy_new();
	struct neighbor_xy_list *current = head;

	struct node *neighborNode;
	struct neighbor_list *neighborNodes_head;
	struct neighbor_list *neighborNodes_current;

	if (parent) {
		px = parent->x;
		py = parent->y;

		dx = (x - px) / max(abs(x - px), 1);
		dy = (y - py) / max(abs(y - py), 1);

		/* Diagonals */
		if (dx != 0 && dy != 0) {
			if (isWalkableAt(gd, x, (y + dy))) {
				current = neighbor_xy_insert_right(current, x, (y + dy));
			}
			if (isWalkableAt(gd, (x + dx), y)) {
				current = neighbor_xy_insert_right(current, (x + dx), y);
			}
			if (isWalkableAt(gd, x, (y + dy)) || isWalkableAt(gd, (x + dx), y)) {
				current = neighbor_xy_insert_right(current, (x + dx), (y + dy));
			}
			if (!isWalkableAt(gd, (x - dx), y) && isWalkableAt(gd, x, (y + dy))) {
				current = neighbor_xy_insert_right(current, (x - dx), (y + dy));
			}
			if (!isWalkableAt(gd, x, (y - dy)) && isWalkableAt(gd, (x + dx), y)) {
				current = neighbor_xy_insert_right(current, (x + dx), (y - dy));
			}

			/* Horizontal / Vertical */
		} else {
			if (dx == 0) {
				if (isWalkableAt(gd, x, (y + dy))) {
					if (isWalkableAt(gd, x, (y + dy))) {
						current = neighbor_xy_insert_right(current, x, (y + dy));
					}
					if (!isWalkableAt(gd, (x + 1), y)) {
						current = neighbor_xy_insert_right(current, (x + 1), (y + dy));
					}
					if (!isWalkableAt(gd, (x - 1), y)) {
						current = neighbor_xy_insert_right(current, (x - 1), (y + dy));
					}
				}
			} else {
				if (isWalkableAt(gd, (x + dx), y)) {
					if (isWalkableAt(gd, (x + dx), y)) {
						current = neighbor_xy_insert_right(current, (x + dx), y);
					}
					if (!isWalkableAt(gd, x, (y + 1))) {
						current = neighbor_xy_insert_right(current, (x + dx), (y + 1));
					}
					if (!isWalkableAt(gd, x, (y - 1))) {
						current = neighbor_xy_insert_right(current, (x + dx), (y - 1));
					}
				}
			}
		}
	} else {
		neighborNodes_head = getNeighbors(gd, activeNode);
		neighborNodes_current = neighborNodes_head;
		while (neighborNodes_head != (neighborNodes_current = neighborNodes_current->right)) {
			neighborNode = neighborNodes_current->neighbor_node;
			current = neighbor_xy_insert_right(current, neighborNode->x, neighborNode->y);
		}
		clean_neighbor_list(neighborNodes_head);
	}

	return head;
}
