# Breadth First Search: Finding the Shortest Path Through a Maze

## Generate maze

Generate a 50x50 maze, then find the shortest path (if a path exists)

------------------------------ On MacOS (should work on Linux, but not yet tested)

$> ruby path.rb 50 50 "* o12"

$> ./solve_maze maze.txt

------------------------------ On Windows

$> ruby path.rb 50 50 "* o12"

$>.\solve_maze.exe maze.txt

## Overview

For this project we will use a breadth first search approach, with a few optimizations, to find the shortest path through a maze.
Since the nodes in the maze are unweighted, algorithms such as Dijkstra's algorithm and A* are not applicable. The maze provided
is a randomly generated field of obstacles (*) and traversable terrain (empty space), with a special character denoting the entrance
and exit of the maze (1 and 2 respectively in this example). The format of the maze is determined by the user when calling the ruby script from a terminal in the fomat as seen above. An image of a typical maze can be seen below.

![Screen Shot 2023-03-08 at 10 59 08 PM](https://user-images.githubusercontent.com/73136662/223913609-076f97bd-2c18-4a3d-b120-1a6702403261.png)

An added constraint I added to my implementation was to forgo the use of dynamic memory allocation. 

Next, we will take a look at the main function to get a high-level understanding of the architecture of the program.

## Architecture

![Screen Shot 2023-03-08 at 6 32 58 PM](https://user-images.githubusercontent.com/73136662/223913926-adbc37d0-f0fb-4ee5-9f68-9486787eefb7.png)

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

![Screen Shot 2023-03-08 at 9 35 39 PM](https://user-images.githubusercontent.com/73136662/223913706-49508ad6-6a94-46ff-940a-be2503e13a69.png)

Firstly we define a 2D matrix called *nodes* that map to every point in the coordinate plane of the maze. I do this through the use
of variable length arrays (VLAs). The use of variable length arrays is usually not recommended, but I define the maximum dimensions
of the array in the header file, and if they are exceeded in the maze configuration declaration, then the program will return EXIT_FAILURE.
So in this case, the size of the matrix is constrained, so the output is predictable, and it is not cause for concern. This choice is done
to satisfy the restraint of no dynamic memory allocation. 

Moving forward, both the coordinate plane and the matrix containing the maze, along with the maze configuration details are
passed to the bfs function. The variable *dist* will actually be the exit node of the maze. As we will see, it will contain the 
shortest distance from the entrance to the exit, and will also act as the head of a linked list to backtrack through the maze (for
the purposes of displaying the solution). 

![Screen Shot 2023-03-09 at 12 01 02 AM](https://user-images.githubusercontent.com/73136662/223924441-94f012e2-05a5-4a05-9520-e2d11d7dae95.png)


Lookin into the *bfs.c* file, we will see all of the functions involved in the search. The bfs function calls the *find_start*
function, which combs through the maze looking for the entrance (which will always be in the first row in this example) and
returns its coordinates in a *Point* struct. The entrance node, along with the 2 matrices, configuration information, and a node
pointer are passed to the *search* function, where the algorithm takes place. The node pointer will play an important role in this
implementation, as it will be used as a temporary store for all of the nodes we visit, update them with relevant information, and
ultimately be returned as a pointer to the exit node which we just discussed. 

![Screen Shot 2023-03-09 at 12 03 13 AM](https://user-images.githubusercontent.com/73136662/223925262-92f36698-b539-49fd-a6ab-0cfef880332a.png)

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

By following these steps (shown in code below), we are guaranteed to find the shortest path in an unweighted graph. In BFS, the first time we encounter the target node, 
the exit node in our case, we will have traversed the minimum *depth* or *levels* (not to be confused with depth first search [(DFS)](https://en.wikipedia.org/wiki/Depth-first_search)). This is all due to the functionality of the FIFO queue. By its
very nature, we will analyze all nodes at a given distance from the start node before moving on to a further distance, or the next level. For
instance, when we analyze the entrance node, we will enqueue its only neighbor, the node directly below it (our maze has a single entrance and exit), which
is a distance of 1 away from the start node. The next iteration of the search, we will enqueue the neighbors of the node directly below the 
entrance, each a distance of 2 away from the start node. After that, we will sequentially dequeue all of the neighbors we just added, and even though we will
enqueue the neighbors of each of those nodes, we will examine them in the order that they were added, i.e. we will examine each node of a given *depth* before
moving on to the nodes that are 1 step deeper than the ones we are currently looking at. You can see a pictoral representation of this below.

![Screen Shot 2023-03-10 at 1 36 36 PM](https://user-images.githubusercontent.com/73136662/224397525-e7fcbfee-be3a-428b-bb7b-96b8f250a030.png)

The first insertion into the queue, N1, is the node directly below the entrance. It is immediatly dequeued, and its 3 neighbors (N2<sub>1</sub>, N2<sub>2</sub>, N2<sub>3</sub>) are added to the queue at the *head*, which we will see is analogous to the "back of the line" in a FIFIO queue. In this example you can think of N1 as "node or neighbor at distance 1 from the start node", and the subscipted nodes denote the number of nodes at the specified depth. If you have not noticed already, when adding a nodes neighbors to the queue, we will only add a maximum of 3 nodes, as at least one of them are a node that we have previously visisted (i.e. the current node was previously the neighbor of another), and we do not add duplicates to the queue. Nonetheless, as we move to the next round of pushing and popping, we also move on to the next *depth* of the search.

As you can see, the nodes added after N2<sub>3</sub> are N3 nodes, or nodes a distance of 3 away from the start node. These 3 are the neighbors of N2<sub>1</sub>, which has just been dequeued, showing how the FIFO queue (First In First Out) gets its name. This brings us to the final round shown at the bottom of the image. Be sure to note that the nodes in the queue have been shifted. Out of the frame, N2<sub>2</sub> has been dequeued, and there are now 3 empty spaces in the front of the queue (reference the sub script numbers to confirm this for yourself). In this round, notice that the 3 neighbors of N2<sub>2</sub> that were added (N3<sub>4</sub>, N3<sub>5</sub>, N3<sub>6</sub>) are at the same level as the nodes added by N2<sub>1</sub>. This demonstrates that not only will the nodes added to the queue be dealt with in the order they were added, but also that each *depth* will be completely explored before moving on to the next level. 

By iterating through this process, we ensure that once we find the target node,
whatever *depth* we are currently at is the shortest distance away from the starting point. Their may be other nodes at the same level that also reach the 
target node, but our algorithm terminate after the first node that can access the exit is found.

![Screen Shot 2023-03-09 at 12 05 49 AM](https://user-images.githubusercontent.com/73136662/223925691-5c7883c9-ae1e-4bf8-b971-e5fe0628f171.png)

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

## Complexity

This implementation has a time and space complexity of O(m*n) where m and n are the dimensions of the graph (the 2D matrix representation of the coordinates of the maze): (m*n) representing the sum total of all of the nodes. This could also be written as O(V) where V is the number of vertices (nodes). Through our use of a queue and our auxiliary *visited* matrix, we ensure that in the worse case, we will visit every node (V) only once, so we only need to reserve enough space to hold the information of every node. 

## Summary 

This project has covered many topics besides BFS such as matrix manipulation, the queue and linked list data structures, utilizing stack memory, and how to use pointers
effectively. There are many ways to solve this problem, and this is configuration is the one that I decided to implement. Although this is a fun problem to figure out
in its own right, the knowledge gained from it could be used in project involving GPS, networking applications, or even image/video processing software for a vehicle to traverse a space in the 
shortest distance possible. 
