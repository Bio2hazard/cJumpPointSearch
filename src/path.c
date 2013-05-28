#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#include "jps_grid.h"
#include "heap.h"
#include "neighbors.h"
#include "display.h"

/* Activate for a LOT of debug output */
#define DEBUG 0

int euclidean(int dx, int dy)
{
	double distance = sqrt((double)(dx * dx + dy * dy)) * 10.0;
	int idistance = (int)distance;
	if (DEBUG)
		printf("Euclidean engaged! regular:%lf , (int)*10:%d\n", distance, idistance);
	return idistance;
}

int manhattan(int dx, int dy)
{
	int distance = (dx + dy) * 10;
	if (DEBUG)
		printf("Manhattan engaged! regular:%d        , (int)*10:%d\n", (dx + dy), distance);
	return distance;
}

int *_jump(struct grid *gd, int x, int y, int px, int py, struct node *endNode)
{
	int dx = x - px;
	int dy = y - py;
	int *jx, *jy;
	if (DEBUG)
		printf("	_jump attempt: x:%d/y:%d px:%d/py:%d dx:%d/dy:%d\n", x, y, px, py, dx, dy);
	if (!isWalkableAt(gd, x, y)) {
		if (DEBUG)
			printf("	x:%d/y:%d is not walkable. Exiting _jump\n", x, y);
		return NULL;
	} else if (getNodeAt(gd, x, y) == endNode) {
		int *i = (int *) malloc(2 * sizeof(int));
		if (DEBUG)
			printf("	_jump(1) return value: x:%d/y:%d\n", x, y);
		malloc_count++; /* [ Malloc Count ] */
		i[0] = x;
		i[1] = y;
		return i;
	}

	if (dx != 0 && dy != 0) {
		if ((isWalkableAt(gd, (x - dx), (y + dy)) && !isWalkableAt(gd, (x - dx), y)) ||
		    (isWalkableAt(gd, (x + dx), (y - dy)) && !isWalkableAt(gd, x, (y - dy)))) {
			int *i = (int *) malloc(2 * sizeof(int));
			if (DEBUG)
				printf("	_jump(2) return value: x:%d/y:%d\n", x, y);
			malloc_count++;         /* [ Malloc Count ] */
			i[0] = x;
			i[1] = y;
			return i;
		}
	} else {
		if (dx != 0) {
			if ((isWalkableAt(gd, (x + dx), (y + 1)) && !isWalkableAt(gd, x, (y + 1))) ||
			    (isWalkableAt(gd, (x + dx), (y - 1)) && !isWalkableAt(gd, x, (y - 1)))) {
				int *i = (int *) malloc(2 * sizeof(int));
				if (DEBUG)
					printf("	_jump(3) return value: x:%d/y:%d\n", x, y);
				malloc_count++;         /* [ Malloc Count ] */
				i[0] = x;
				i[1] = y;
				return i;
			}
		} else {
			if ((isWalkableAt(gd, (x + 1), (y + dy)) && !isWalkableAt(gd, (x + 1), y)) ||
			    (isWalkableAt(gd, (x - 1), (y + dy)) && !isWalkableAt(gd, (x - 1), y))) {
				int *i = (int *) malloc(2 * sizeof(int));
				if (DEBUG)
					printf("	_jump(4) return value: x:%d/y:%d\n", x, y);
				malloc_count++;         /* [ Malloc Count ] */
				i[0] = x;
				i[1] = y;
				return i;
			}
		}
	}

	if (dx != 0 && dy != 0) {
		if (DEBUG)
			printf("	Recursive _jumping(1) with ( x:%d/y:%d px:%d/py:%d )\n", (x + dx), y, x, y);
		jx = _jump(gd, (x + dx), y, x, y, endNode);

		if (DEBUG)
			printf("	Recursive _jumping(2) with ( x:%d/y:%d px:%d/py:%d )\n", x, (y + dy), x, y);
		jy = _jump(gd, x, (y + dy), x, y, endNode);

		if (jx || jy) {
			int *i;

			if (DEBUG)
				printf("	_jump(5) return value: x:%d/y:%d\n", x, y);

			if (jx) {
				free(jx);
				malloc_count--; /* [ Malloc Count ] */
			}
			if (jy) {
				free(jy);
				malloc_count--; /* [ Malloc Count ] */
			}

			i = (int *) malloc(2 * sizeof(int));
			malloc_count++; /* [ Malloc Count ] */
			i[0] = x;
			i[1] = y;
			return i;
		}
	}

	if (isWalkableAt(gd, (x + dx), y) || isWalkableAt(gd, x, (y + dy))) {
		if (DEBUG)
			printf("	Recursive _jumping(3) with ( x:%d/y:%d px:%d/py:%d )\n", (x + dx), (y + dy), x, y);
		return _jump(gd, (x + dx), (y + dy), x, y, endNode);
	} else {
		if (DEBUG)
			printf("	Returning NULL\n");
		return NULL;
	}
}

void _identifySuccessors(struct grid *gd, struct node *activeNode, struct open_list *current, struct node *endNode)
{
	int endX = endNode->x;
	int endY = endNode->y;
	int *jumpPoint;
	struct neighbor_xy_list *neighbors_head = _findNeighbors(gd, activeNode);
	struct neighbor_xy_list *neighbors_current = neighbors_head;
	while (neighbors_head != (neighbors_current = neighbors_current->right)) {
		if (DEBUG) {
			if (isWalkableAt(gd, neighbors_current->x, neighbors_current->y))
				printf("Neighbor x:%d/y:%d is walkable!\n", neighbors_current->x, neighbors_current->y);
			else
				printf("Neighbor x:%d/y:%d is NOT walkable!\n", neighbors_current->x, neighbors_current->y);
		}

		jumpPoint = _jump(gd, neighbors_current->x, neighbors_current->y, activeNode->x, activeNode->y, endNode);
		if (DEBUG)
			printf("Jump point not set!\n\n");
		if (jumpPoint != NULL) {
			int jx, jy, d, ng;
			struct node *jumpNode;
			if (DEBUG)
				printf("Jump point set!\n\n");
			jx = jumpPoint[0];
			jy = jumpPoint[1];

			free(jumpPoint);
			malloc_count--; /* [ Malloc Count ] */

			jumpNode = getNodeAt(gd, jx, jy);
			if (jumpNode->closed) {
				continue;
			}

			d = euclidean(abs(jx - activeNode->x), abs(jy - activeNode->y));
			ng = activeNode->g + d;
			if (!jumpNode->opened || ng < jumpNode->g) {
				jumpNode->g = ng;
				if (!jumpNode->h)
					jumpNode->h = manhattan(abs(jx - endX), abs(jy - endY));
				/* jumpNode->h = jumpNode->h || manhattan(abs(jx - endX), abs(jy - endY)); // ASK FIDELIS !! */
				jumpNode->f = jumpNode->g + jumpNode->h;
				if (DEBUG)
					printf("Node g:%d h:%d f:%d\n", jumpNode->g, jumpNode->h, jumpNode->f);
				jumpNode->parent = activeNode;

				if (!jumpNode->opened) {
					current = ol_insert_right(current, jumpNode);
					jumpNode->opened = true;
				} else {
					ol_listsort(current->right);
				}
			}
		}
	}
	neighbor_xy_clean(neighbors_head);
}

struct neighbor_xy_list *backtrace(struct node *activeNode)
{
	struct neighbor_xy_list *head = neighbor_xy_new();
	struct neighbor_xy_list *current = head;
	current = neighbor_xy_insert_right(current, activeNode->x, activeNode->y);
	while (activeNode->parent != NULL) {
		activeNode = activeNode->parent;
		current = neighbor_xy_insert_right(current, activeNode->x, activeNode->y);
	}
	return head;
}

struct neighbor_xy_list *findPath(struct grid *gd, int startX, int startY, int endX, int endY)
{
	struct open_list *head = ol_new();
	struct open_list *current = head;
	struct node *startNode = getNodeAt(gd, startX, startY);
	struct node *endNode = getNodeAt(gd, endX, endY);
	struct node *activeNode;
	int counter = 0;

	/* Initialize the start node */
	startNode->h = 0;
	startNode->g = 0;
	startNode->f = 0;
	startNode->parent = NULL;

	current = ol_insert_right(current, startNode);

	startNode->opened = true;

	head = ol_listsort(head);
	current = head->left;

	while (head != current) { /* List is empty when current marker rests on head */
		if (DEBUG)
			printf("Cycle %d\n", counter);

		activeNode = current->list_node;
		current = ol_del_free(current);
		activeNode->closed = true;

		if (activeNode == endNode) {
			struct neighbor_xy_list *goal;
			ol_clean(head);
			goal = backtrace(activeNode);
			return goal;
		}

		/* Begin identifying successors... */
		_identifySuccessors(gd, activeNode, current, endNode);
		head = ol_listsort(head);               /* Instead of sorting it everytime a item is added, I will sort when I need to grab the lowest value. */
		current = head->right;

		if (DEBUG) {
			listOpenList(head);
			printf("Currently active node:\n");
			displayNodeInfo(current->list_node);
		}

		counter++;
		if (counter >= 5000) {
			ol_clean(head);
			printf("\n----------\nLimit reached\n----------\n");
			return NULL;
		}
	}
	printf("\n----------\nReturning NULL because head = current\n----------\n");
	return NULL;
}

struct neighbor_xy_list *smooth_path(struct grid *gd, struct neighbor_xy_list *head)
{
	struct neighbor_xy_list *pos = head->left;
	int xi, yi, dx, dy;
	while (head != NULL && (head != (pos = pos->left))) {
		xi = pos->x;
		yi = pos->y;
		dx = xi - pos->right->x;
		dy = yi - pos->right->y;
		if (dx == 1 && dy == -1) { /* Up & Right */
			if (!isWalkableAt(gd, xi, yi + 1)) {
				pos = neighbor_xy_insert_right(pos, pos->x - 1, pos->y);
			} else if (!isWalkableAt(gd, xi - 1, yi)) {
				pos = neighbor_xy_insert_right(pos, pos->x, pos->y + 1);
			}
		} else if (dx == 1 && dy == 1) {    /* Down & Right */
			if (!isWalkableAt(gd, xi - 1, yi)) {
				pos = neighbor_xy_insert_right(pos, pos->x, pos->y - 1);
			} else if (!isWalkableAt(gd, xi, yi - 1)) {
				pos = neighbor_xy_insert_right(pos, pos->x - 1, pos->y);
			}
		} else if (dx == -1 && dy == 1) {    /* Down & Left */
			if (!isWalkableAt(gd, xi, yi - 1)) {
				pos = neighbor_xy_insert_right(pos, pos->x + 1, pos->y);
			} else if (!isWalkableAt(gd, xi + 1, yi)) {
				pos = neighbor_xy_insert_right(pos, pos->x, pos->y - 1);
			}
		} else if (dx == -1 && dy == -1) {    /* Up & Left */
			if (!isWalkableAt(gd, xi + 1, yi)) {
				pos = neighbor_xy_insert_right(pos, pos->x, pos->y + 1);
			} else if (!isWalkableAt(gd, xi, yi + 1)) {
				pos = neighbor_xy_insert_right(pos, pos->x + 1, pos->y);
			}
		} else if (abs(dx) > 1 || abs(dy) > 1) {
			int incrX = dx / max(abs(dx), 1);
			int incrY = dy / max(abs(dy), 1);
			pos = neighbor_xy_insert_right(pos, pos->right->x + incrX, pos->right->y + incrY);
		}
	}
	return head;
}
