#C Jump Point Search Implementation
==================

This is a C implementation of the Jump Point Search pathfinding algorithm. The code is based on qiao's excellent JavaScript JPS implementation, which can be found at https://github.com/qiao/PathFinding.js

I'm by no means a expert at C, but I needed a C implementation of the JPS algorithm, none existed at the time and so I decided to take matters in my own hands.

When executed, this program reads in a file called map_in.txt where a - represents a walkable tile, # represents the start, * represents the end and X represents a unwalkable tile. It will then calculate the path, smoothen it and output each coordinate. It will then print a visual representation of the path in the console window.

It really is meant to be implemented and used in other prjects and as such mostly serves to debug and benchmark the algorithm.

For more information on the JPS algorithm check out http://harablog.wordpress.com/2011/09/07/jump-point-search/

Author: Felix Kastner (felix@chapterfain.com)

Creation date: 2012

License: MIT