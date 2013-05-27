#include <iostream>
#include <time.h>

#include "jps_grid.h"
#include "neighbors.h"
#include "path.h"
#include "display.h"
#include "heap.h"

// Malloc count is used to make sure the program cleans up properly after itself.
// This value gets increased for every "malloc", and reduced for every "free".
int malloc_count = 0;

void test()
{
	int i, width = 100, height = 100, startX=0,startY=0,endX=0,endY=0; // Set the size of the map

	// Prepare the Matrix of Walkable / Not Walkable - Dynamic Size
	bool** matrix;
	matrix = (bool**) malloc(height*sizeof(bool*));
	malloc_count++; // [ Malloc Count ]
	for(int i = 0; i < height; i++) {
		matrix[i] = (bool*)malloc(width*sizeof(bool));
		malloc_count++; // [ Malloc Count ]
	}
	FILE *file = fopen ("map_in.txt", "r");
	char c;
	int n = 0,l = 0;

	if (file == NULL)
		perror("Error reading file");
	else
	{
		do {
			c = getc (file);
			if (c == '-') 
				matrix[l][n] = true;
			else if (c == '#') {
				printf("Start point found at X:%d / Y:%d\n",n,l);
				startY = l;
				startX = n;
				matrix[l][n] = true;
			} else if (c == '*') {
				printf("End point found at X:%d / Y:%d\n",n,l);
				endY = l;
				endX = n;
				matrix[l][n] = true;
			} else if (c == 'X') {
				matrix[l][n] = false;
			}
			n++;
			if(n > width) {
				n = 0;
				l++;
			}
		} while(c != EOF);
		fclose(file);
	}

	struct grid* newgrid; 
	newgrid = &createGrid(width,height,matrix); // Create a new grid

	clock_t c0 = clock();

	struct neighbor_xy_list * path_head = NULL;
	path_head = findPath(newgrid,startX,startY,endX,endY);
	struct neighbor_xy_list * path_pos = path_head;
	struct neighbor_xy_list * path_clean = path_head;

	
	clock_t c1 = clock();
	double runtime_diff_ms = (c1 - c0) * 1000. / CLOCKS_PER_SEC;

	printf("\nPath calculation took %.0fms\n",runtime_diff_ms);
	system("Pause");
	
	// Use displayGrid to display the map without the path
	//displayGrid(newgrid);


	
	path_head = smooth_path(newgrid, path_head);
	path_pos = path_head;
	path_clean = path_head;

	printf("\n\n:Waypoints:\n");
	int count = 0;
	while(path_head != NULL && (path_head != (path_pos = path_pos->left))) {
		printf("Step %d: x:%d y:%d\n",count++,path_pos->x,path_pos->y);
	}
	printf("::Path Smoothened::\n");
	
	printf("\n\n");
	if(path_head != NULL) {
		displaySolution(newgrid,path_head);
	}

	// Cleanup - Free all created arrays.
	neighbor_xy_clean(path_head);
	
	for(i = 0; i < height; i++) {
		free(newgrid->nodes[i]);
		malloc_count--; // [ Malloc Count ]
	}

	free(newgrid->nodes);
	malloc_count--; // [ Malloc Count ]
	
	for(i = 0; i < height; i++) {
		free(matrix[i]);
		malloc_count--; // [ Malloc Count ]
	}
	free(matrix);
	malloc_count--; // [ Malloc Count ]
	
	printf("Malloc count is:%d\n",malloc_count);
	system("Pause");
}

void main()
{
	test();
}