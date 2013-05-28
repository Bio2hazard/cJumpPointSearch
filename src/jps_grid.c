#include <malloc.h>

#include "jps_grid.h"

struct neighbor_list *new_neighbor_list()
{
	struct neighbor_list *newlist = (struct neighbor_list *) malloc(sizeof(struct neighbor_list));
	malloc_count++; /* [ Malloc Count ] */
	newlist->right = newlist;
	newlist->left = newlist;
	newlist->neighbor_node = NULL;
	return newlist;
}

void clean_neighbor_list(struct neighbor_list *head)
{
	if (head != NULL) {
		struct neighbor_list *pos = head;
		struct neighbor_list *tmp = head;
		do {
			tmp = pos->right;
			free(pos);
			malloc_count--; /* [ Malloc Count ] */
			pos = tmp;
		} while (pos != head);
	}
}

struct neighbor_list *insert_right(struct neighbor_list *list, struct node *data)
{
	struct neighbor_list *newlist = (struct neighbor_list *) malloc(sizeof(struct neighbor_list));
	malloc_count++;         /* [ Malloc Count ] */
	newlist->neighbor_node = data;
	newlist->left = list;
	newlist->right = list->right;
	list->right = newlist;
	newlist->right->left = newlist;
	return newlist;
}

struct node createNode( int x, int y, bool walkable)
{
	struct node nd;
	nd.x = x;
	nd.y = y;
	nd.f = 0;
	nd.g = 0;
	nd.h = 0;
	nd.opened = false;
	nd.closed = false;
	nd.parent = NULL;
	nd.walkable = walkable ? true : false;
	return nd;
}

struct grid createGrid( int width, int height, bool **matrix)
{
	struct grid gd;
	gd.width = width;
	gd.height = height;
	gd.nodes = _buildNodes(width, height, matrix);
	return gd;
}

struct node **_buildNodes( int width, int height, bool **matrix)
{
	int i, j;
	struct node **nodes;
	nodes = (struct node **) malloc(height * sizeof(struct node *));
	malloc_count++; /* [ Malloc Count ] */

	for (i = 0; i < height; i++) {
		nodes[i] = (struct node *) malloc(width * sizeof(struct node));
		malloc_count++; /* [ Malloc Count ] */
		for (j = 0; j < width; ++j) {
			nodes[i][j] = createNode(j, i, matrix[i][j]);
		}
	}
	return nodes;
}

struct node *getNodeAt(struct grid *gd, int x, int y)
{
	return &gd->nodes[y][x];
}

bool isWalkableAt(struct grid *gd, int x, int y)
{
	return isInside(gd, x, y) && gd->nodes[y][x].walkable;
}

bool isInside(struct grid *gd, int x, int y)
{
	return (x >= 0 && x < gd->width) && (y >= 0 && y < gd->height);
}

void setWalkableAt(struct grid *gd, int x, int y, bool walkable)
{
	gd->nodes[y][x].walkable = walkable;
}

struct neighbor_list *getNeighbors(struct grid *gd, struct node *nd)
{
	int x = nd->x;
	int y = nd->y;

	struct neighbor_list *head = new_neighbor_list();
	struct neighbor_list *current = head;

	bool d0 = false;
	bool d1 = false;
	bool d2 = false;
	bool d3 = false;

	/* UP */
	if (isWalkableAt(gd, x, y - 1)) {
		current = insert_right(current, &gd->nodes[y - 1][x]);
		d0 = d1 = true;
	}

	/* RIGHT */
	if (isWalkableAt(gd, x + 1, y)) {
		current = insert_right(current, &gd->nodes[y][x + 1]);
		d1 = d2 = true;
	}

	/* DOWN */
	if (isWalkableAt(gd, x, y + 1)) {
		current = insert_right(current, &gd->nodes[y + 1][x]);
		d2 = d3 = true;
	}

	/* LEFT */
	if (isWalkableAt(gd, x - 1, y)) {
		current = insert_right(current, &gd->nodes[y][x - 1]);
		d3 = d0 = true;
	}

	/* UP + LEFT */
	if (d0 && isWalkableAt(gd, x - 1, y - 1)) {
		current = insert_right(current, &gd->nodes[y - 1][x - 1]);
	}

	/* UP + RIGHT */
	if (d1 && isWalkableAt(gd, x + 1, y - 1)) {
		current = insert_right(current, &gd->nodes[y - 1][x + 1]);
	}

	/* DOWN + RIGHT */
	if (d2 && isWalkableAt(gd, x + 1, y + 1)) {
		current = insert_right(current, &gd->nodes[y + 1][x + 1]);
	}

	/* DOWN + LEFT */
	if (d3 && isWalkableAt(gd, x - 1, y + 1)) {
		current = insert_right(current, &gd->nodes[y + 1][x - 1]);
	}

	return head;
}
