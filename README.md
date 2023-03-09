# Breadth First Search: Finding the Shortest Path Through a Maze

## Generate maze

Generate a 50x50 maze, then find the shortest path (if a path exists)

$> ruby path.rb 50 50 "* o12"
$> ./solve_maze maze.txt

## Overview

For this project we will use a breadth first search approach, with a few optimizations, to find the shortest path through a maze.
Since the nodes in the maze are unweighted, algorithms such as Dijkstra's algorithm and A* are not applicable. The maze provided
is a randomly generated field of obstacles (*) and traversable terrain (empty space), with a special character denoting the entrance
and exit of the maze (1 and 2 respectively in this example). An image of a typical maze can be seen below.

An added constraint I added to my implementation was to forgo the use of dynamic memory allocation. 

Next, we will take a look at the main function to get a high-level understanding of the architecture of the program.

## Architecture

Taking a look at the main function, up to line 18 is dedicated to reading in the contents of the text file into a buffer,
along with some error checking to make sure that the file name given as a command line argument (CLA) is a valid file name,
as well as checking if the file was succesfully read into the program.

The next few lines are to parse through the first line of the file to save the configuration of the maze in the
*MazeConfig* struct, and then reading the remainder of the file into a 2D matrix. If you want to look more into the parsing and 
processing of the input file, take a look in the *matrix.c* and *maze_config.c* files.

After all of this parsing and setup is done, the actual implementation of the algorithm and printing out of the solution is just
in 3 lines! 

Of course there is much code behind these 3 lines in the main function, and we will dive into that now.

## The algorithm

Firstly we define a 2D matrix called *nodes* that map to every point in the coordinate plane of the maze. I do this through the use
of variable length arrays (VLAs). The use of variable length arrays is usually not recommended, but I define the maximum dimensions
of the array in the header file, and if they are exceeded in the maze configuration declaration, then the program will return EXIT_FAILURE.
So in this case, the size of the matrix is constrained, so the output is predictable, and it is not cause for concern. This choice is done
to satisfy the restraint of no dynamic memory allocation. 

Moving forward, both the coordinate plane and the matrix containing the maze, along with the maze configuration details are
passed to the bfs function. The variable *dist* will actually be the exit node of the maze. As we will see, it will contain the 
shortest distance from the entrance to the exit, and will also act as the head of a linked list to backtrack through the maze (for
the purposes of displaying the solution). 

Lookin into the *bfs.c* file, we will see all of the functions involved in the search. The actual bfs function calls the *find_start*
function, which combs through the maze looking for the entrance (which will always be in the first row in this example) and
returns its coordinates in a *Point* struct. The entrance node, along with the 2 matrices, configuration information, and a node
pointer are passed to the search function, where the algorithm takes place. The node pointer will play an important role in this
implementation, as it will be used as a temporary store for all of the nodes we visit, update them with relevant information, and
ultimately be returned as a pointer to the exit node which we just discussed. 

The first few lines are just initialization of variables. First, we initialize a FIFO queue which will hold the each node we visit, as
well as each of their neighbors. We then initialize the entrance node *src*, which wraps the entrance coordinates into the
larger *Node* struct. We then load this first value into the queue. Notice that this queue implementation uses the object oriented
programming principle of encapsulation, as the data, as well as the functions that act upon it are contained within the queue struct
(who says you can't do OOP in C?). Next, we initialize a 2D boolean matrix and set all of its members to false with the *set_visit* function,
setting the entrance node to true, as we have already loaded it into the queue. This nodes in this matrix correspond to the coordinates of 
the maze, and will be marked true when they are visited to avoid unnecessary handling of nodes that we have already dealt with. The *row_mod*
and *column_mod* arrays are used in the algorithm to look at the neighbors directly to the left, above, to the right, and below, respectively. 

We now enter into the bulk of our BFS algorithm. The next 24 lines of code essentially encompass the entire action of BFS, which is as follows: 

    1. Dequeue the top node.
    2. Check if it is the target node. If it is, return it. If it is not, continue.
    3. Check each of the neighbors of the top node. If they are valid (in the bounds of the matrix),
    and they have not been visited yet, and, in this specific case, if they are not a boundary
    character (*), add them to the queue
    4. Repeat

By following these steps, we are guaranteed to find the shortest path in an unweighted graph. In BFS, the first time we encounter the target node, 
the exit node in our case, we will have traversed the minimum *depth* or *levels*. This is all due to the functionality of the FIFO queue. By its
very nature, we will analyze all nodes at a given distance from the start node before moving on to a further distance, or the next level. For
instance, when we analyze the entrance node, we will enqueue its only neighbor, the node directly below it (our maze has a single entrance and exit), which
is a distance of 1 away from the start node. The next iteration of the search, we will enqueue the neighbors (maximum of 4) of the node directly below the 
entrance, each a distance of 2 away from the start node. After that, we will sequentially dequeue all of the neighbors we just added, and even though we will
enqueue the neighbors of each of those nodes, we will examine them in the order that they were added, i.e. we will examine each node of a given *depth* before
moving on to the nodes that are 1 step deeper than the ones we are currently looking at. By doing this, we ensure that once we find the target node,
whatever *depth* we are currently at is the shortest distance away from the starting point. Their may be other nodes at the same level that also reach the 
target node, but our algorithm terminate after the first node that can access the exit is found.

Also, to again reference the *curr_node* pointer, it acts as the temporary reference of every node that we load into the queue. Looking at line 64, it is used
to be the previous node of each of its neighbors, and its distance from the source is also incremented by one, maintaining the reality that any neighbor to a
node (besides the one that it has marked as its previous node, which will not be considered anyway because it has already been marked true in the visited boolean
matrix) is 1 more level deeper than it is. Notice also that the last line of the search function sets *curr_node* to NULL. This is because if we have checked
every valid node and have not found the exit node, their is no possible path from entrance to exit. The maze generator will sometimes produce a maze that is not
solvable like the maze shown below where there is an obstacle directly in front of the entrance node. Since I passed the *curr_node* variable to this function,
I change its value to NULL in the case where their is no solution, but I could just as easily reconfigure this function to return NULL and slightly change the
bfs function to accommodate this. Furthermore, since each neihbor node added to the queue is equipped with the memory address of its predecessor, when we find the 
exit node, we can traverse back to the entry node as mentioned previously. This allows us to display the path from start to finish (or finish to start) by modifying
the maze matrix values that lie along the shortest path to *o* as seen in the image at the start of this section.

## Summary 

This project has covered many topics besides BFS such as matrix manipulation, the queue and linked list data structures, utilizing stack memory, and how to use pointers
effectively. There are many ways to solve this problem, and this is configuration is the one that I decided to implement. Although this is a fun problem to figure out
in its own right, the knowledge gained from it could be used in project involving GPS, or even image/video processing software for a vehicle to traverse a space in the 
shortest distance possible. 
