#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jps_grid.h"
#include "neighbors.h"
#include "path.h"
#include "display.h"
#include "heap.h"

/* Malloc count is used to make sure the program cleans up properly after itself. */
/* This value gets increased for every "malloc", and reduced for every "free". */
int malloc_count = 0;

void test()
{
	int i, width = 100, height = 100, startX = 0, startY = 0, endX = 0, endY = 0; /* Set the size of the map */
	FILE *file;
	char c;
	int n, l, count;
	bool **matrix;
	struct grid newgrid;
	struct neighbor_xy_list *path_head = NULL, *path_pos = NULL;
	clock_t c0, c1;
	double runtime_diff_ms;

	/* Prepare the Matrix of Walkable / Not Walkable - Dynamic Size */
	matrix = (bool **) malloc(height * sizeof(bool *));
	malloc_count++; /* [ Malloc Count ] */
	for (i = 0; i < height; i++) {
		matrix[i] = (bool *)malloc(width * sizeof(bool));
		malloc_count++; /* [ Malloc Count ] */
	}

	file = fopen ("map_in.txt", "r");
	n = l = 0;

	if (file == NULL)
		perror("Error reading file");
	else {
		do {
			c = getc (file);
			if (c == '-')
				matrix[l][n] = true;
			else if (c == '#') {
				printf("Start point found at X:%d / Y:%d\n", n, l);
				startY = l;
				startX = n;
				matrix[l][n] = true;
			} else if (c == '*') {
				printf("End point found at X:%d / Y:%d\n", n, l);
				endY = l;
				endX = n;
				matrix[l][n] = true;
			} else if (c == 'X') {
				matrix[l][n] = false;
			}
			n++;
			if (n > width) {
				n = 0;
				l++;
			}
		} while (c != EOF);
		fclose(file);
	}

	newgrid = createGrid(width, height, matrix); /* Create a new grid */

	c0 = clock();

	path_head = findPath(&newgrid, startX, startY, endX, endY);
	path_pos = path_head;

	c1 = clock();
	runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;

	printf("\nPath calculation took %.0fms\n", runtime_diff_ms);
#ifdef _WIN32
	system("Pause");
#endif

	/* Use displayGrid to display the map without the path */
	/* displayGrid(newgrid); */

	path_head = smooth_path(&newgrid, path_head);
	path_pos = path_head;

	printf("\n\n:Waypoints:\n");
	count = 0;
	while (path_head != NULL && (path_head != (path_pos = path_pos->left))) {
		printf("Step %d: x:%d y:%d\n", count++, path_pos->x, path_pos->y);
	}
	printf("::Path Smoothened::\n");

	printf("\n\n");
	if (path_head != NULL) {
		displaySolution(&newgrid, path_head);
	}

	/* Cleanup - Free all created arrays. */
	neighbor_xy_clean(path_head);

	for (i = 0; i < height; i++) {
		free(newgrid.nodes[i]);
		malloc_count--; /* [ Malloc Count ] */
	}

	free(newgrid.nodes);
	malloc_count--; /* [ Malloc Count ] */

	for (i = 0; i < height; i++) {
		free(matrix[i]);
		malloc_count--; /* [ Malloc Count ] */
	}
	free(matrix);
	malloc_count--; /* [ Malloc Count ] */

	printf("Malloc count is:%d\n", malloc_count);
#ifdef _WIN32
	system("Pause");
#endif
}

int main(int argc, char **argv)
{
	test();
	return 0;
}
