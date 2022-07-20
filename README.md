# The Galactic War

April 2021

<https://ocw.cs.pub.ro/courses/sd-ca/teme/tema1-2021>

Student: Maciuca Alexandru-Petru 314CA

Libraries used for the implementation:
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./utils.h"
#include "./func.h"


## Implementation

	This is an idea of implementing the galaxy using circular doubly 
linked lists where the galaxy is a huge linked list and the planets 
are its nodes. 
	In the same spirit each planet has its own lists of shields 
where every shield is a node of that specific linked list.

### Structure

	Avoiding refurbished amounts of code blocks has been an 
important goal of the project so the structures that are being used are
as generical as possible. 
		There are three main structures :
(1)cdll_t -> stores a head of the list(a node), the number of nodes(int),
and the data_size that could differ from node to node.(for shields it is 
a int and for planets it is a structure with specific info about them)
(2)node_t -> stores two nodes(next and prev) to help navigate through the 
list and its data which is a void pointer for generical use.
(3)specs_t -> a structure that stores important info about the planet such as
name, the list of shields, the number of shields and kills.

	The functions that are being used are mainly generical circular
doubly linked list type of functions such as create_list, add_nth_node,
remove_nth_node and free_list.
	The collide function is using the above functions. Firstly, it checks
if the command is a valid one which means that the command could be applied to
the galaxy at that specific moment and then it does some mathematical checks in
order to decide if a planet should be removed or not. This is being done with
the help of the remove_nth_function. 
	If the rotate function is a valid one then a small mathematical
adjustment is being done in order to reduce time. 
The index is being devided by the number of shields so the "for loop" 
does the minimum amount of iterations possible. 
When the wanted node is being reached, the head will just point to it.
	Upgrade just modifies the value stored in a shield.
	
#### Difficulties
	
	For the complex tests, I had a problem with the collide function.
If the first given index was of the last planet and the second one was of
the first then my assumption that index2 = index1 + 1 was wrong so I ended up
with segmentation fault. In order to solve this I had to add a "if" type of 
decision to treat this specific case.
	The upgrade function was also a bit tricky at first sight because
I did a mistake in the add_planet function and ended up with all shields having
the same address. Furthermore, modifying a value resulted and modifying all 
of them but eventfully I did solved it.


