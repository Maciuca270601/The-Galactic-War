// Copyright 2021 < Copyright Maciuca Alexandru Petru 314CA
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./utils.h"
#include "./func.h"

#define NMAX 31

cdll_t* create_list(int data_size)
{
	// General function that creates a list
	cdll_t *list = malloc(sizeof(cdll_t));
	DIE(!list, "Error while doing the list\n");

	// Sets the initial values of the list
	list->head = NULL;
	list->size = 0;
	list->data_size = data_size;

	return list;
}


void add_nth_node(cdll_t *list, int n, const void *data)
{
	// Creating a new node and giving its data the specific value
	node_t *new_node = malloc(sizeof(node_t));
	DIE(!new_node, "Error while doing new_node\n");

	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "Error while doing new_node->data\n");

	memcpy(new_node->data, data, list->data_size);


	if (list == NULL) {
		fprintf(stderr, "The list has not been created yet\n");
		return;
	}

	if (n < 0) {
		fprintf(stderr, "Position must be a positive index\n");
		return;
	}

	// There is no node so we can add one
	if (list->head == NULL) {
		list->head = new_node;
		new_node->prev = new_node;
		new_node->next = new_node;
		list->size++;
	} else if (n == 0) {
		// Adding on first position in the galaxy
		node_t *tail = list->head;
		new_node->prev = tail->prev;
		new_node->next = tail;
		list->head = new_node;
		new_node->prev->next = new_node;
		tail->prev = new_node;
		list->size++;
	} else if (n >= list->size) {
		// Adding on the last position in the galaxy
		node_t *tail = list->head->prev;
		tail->next = new_node;
		new_node->next = list->head;
		new_node->prev = tail;
		list->head->prev = new_node;
		list->size++;
	} else {
		// Adding in the middle of the galaxy
		node_t *pos = list->head;
		for (int i = 0; i < n - 1; i++) {
			pos = pos->next;
		}

		new_node->next = pos->next;
		new_node->prev = pos;
		pos->next = new_node;
		new_node->next->prev = new_node;
		list->size++;
	}
}


void add_planet(cdll_t *galaxy, specs_t specs, int n)
{
	if (galaxy == NULL) {
		fprintf(stderr, "Galaxy has not been created yet!\n");
	}

	if(n > galaxy->size) {
		printf("Planet out of bounds!\n");
		return;
	}

	// Adding the shields to a planet
	specs.list_of_shields = create_list(sizeof(int));
	int value = 1;

	for (int i = 0; i < specs.shields; i++) {
		add_nth_node(specs.list_of_shields, i, &value);
	}

	// Adding the planet in the galaxy
	add_nth_node(galaxy, n, &specs);
	printf("The planet %s has joined the galaxy.\n", specs.name);
}


void print_shields(node_t *planet)
{
	node_t *shield = ((specs_t *)planet->data)->list_of_shields->head;
	printf("SHIELDS: ");
	for (int j = 0; j < ((specs_t *)planet->data)->shields; j++) {
		printf("%d ", *((int *)shield->data));
		shield = shield->next;
	}
	printf("\n");
}



/*
	In order to simplify the process of showing the right closest planets,
I have divided the show function in 3 main cases:
(1) -> there is only one planet => closest : none
(2) -> there are two planets => closest: current_planet->prev
(3) -> there are more than two planets => closest: curr->prev and curr->next
*/
void show(cdll_t *galaxy, int pos)
{
	if (galaxy->size == 0) {
		printf("Planet out of bounds!\n");
		return;
	}

	node_t *planet = galaxy->head;

	if (pos > galaxy->size -1) {
		printf("Planet out of bounds!\n");
		return;
	}

	// If there is only one planet in the galaxy
	if (galaxy->size == 1) {
		printf("NAME: %s\n", ((specs_t *)planet->data)->name);
		printf("CLOSEST: none\n");
		print_shields(planet);
		printf("KILLED: %d\n", ((specs_t *)planet->data)->kills);
	}

	// If there are two planets in the galaxy
	if (galaxy->size == 2) {
		for (int i = 0; i < pos; i++) {
			planet = planet->next;
		}
		printf("NAME: %s\n", ((specs_t *)planet->data)->name);
		printf("CLOSEST: %s\n", ((specs_t *)planet->prev->data)->name);
		print_shields(planet);
		printf("KILLED: %d\n", ((specs_t *)planet->data)->kills);
	}

	// If there are more than two planets in the galaxy
	if (galaxy->size > 2) {
		for (int i = 0; i < pos; i++) {
			planet = planet->next;
		}
		printf("NAME: %s\n", ((specs_t *)planet->data)->name);
		char name1[NMAX], name2[NMAX];
		int dim1 = strlen(((specs_t *)planet->prev->data)->name) + 1;
		int dim2 = strlen(((specs_t *)planet->next->data)->name) + 1;
		memcpy(name1, ((specs_t *)planet->prev->data)->name, dim1);
		memcpy(name2, ((specs_t *)planet->next->data)->name, dim2);
		printf("CLOSEST: %s and %s\n", name1, name2);
		print_shields(planet);
		printf("KILLED: %d\n", ((specs_t *)planet->data)->kills);
	}
}


void free_the_shields(node_t *planet)
{
	node_t *shield_to_free = ((specs_t *)planet->data)->list_of_shields->head;

	// there is only one shield to free
	if (((specs_t *)planet->data)->shields == 1) {
		free(shield_to_free->data);
		free(shield_to_free);
	} else if (((specs_t *)planet->data)->shields > 1) {
		// there are more shields to be free
		node_t *tail = shield_to_free;
		node_t *final = shield_to_free->prev;
		while (shield_to_free != final) {
			shield_to_free = shield_to_free->next;
			free(tail->data);
			free(tail);
			tail = shield_to_free;
		}
		free(shield_to_free->data);
		free(shield_to_free);
	}

	free(((specs_t *)planet->data)->list_of_shields);
}


void free_the_galaxy(cdll_t **pp_galaxy)
{
	node_t *planet_to_free = (*pp_galaxy)->head;

	// there is only one planet to remove
	if ((*pp_galaxy)->size == 1) {
		free_the_shields(planet_to_free);
		free(((specs_t *)planet_to_free->data));
		free(planet_to_free);
	} else if ((*pp_galaxy)->size > 1) {
		// more planets to free
		node_t *tail = planet_to_free;
		node_t *final = planet_to_free->prev;
		while (planet_to_free != final) {
			planet_to_free = planet_to_free->next;
			free_the_shields(tail);
			free(((specs_t *)tail->data));
			free(tail);
			tail = planet_to_free;
		}

		free_the_shields(planet_to_free);
		free(((specs_t *)planet_to_free->data));
		free(planet_to_free);
	}

	free(*pp_galaxy);
	// pp_galaxy = NULL;
}


node_t* remove_nth_node(cdll_t *list, int n)
{
	node_t *node_to_remove;
	node_t *aux;

	// There is only one node that has to be removed
	if (list->head->next == list->head) {
		list->size--;
		return list->head;
	}
	if (n == 0) {
		// Remove the first node
		node_to_remove = list->head;
		aux = node_to_remove->prev;
		node_to_remove->next->prev = aux;
		aux->next = node_to_remove->next;
		list->head = node_to_remove->next;
		node_to_remove->next = NULL;
		node_to_remove->prev = NULL;
		list->size--;
		return node_to_remove;
	} else if (n == list->size - 1) {
		// Remove the last node
		node_to_remove = list->head->prev;
		node_to_remove->prev->next = list->head;
		list->head->prev = node_to_remove->prev;
		node_to_remove->next = NULL;
		node_to_remove->prev = NULL;
		list->size--;
		return node_to_remove;
	} else {
		// Remove the node from the middle
		node_to_remove = list->head;
		for (int i = 0; i < n; i++) {
			node_to_remove = node_to_remove->next;
		}

		node_to_remove->prev->next = node_to_remove->next;
		node_to_remove->next->prev = node_to_remove->prev;

		node_to_remove->next = NULL;
		node_to_remove->prev = NULL;
		list->size--;
		return node_to_remove;
	}
}


void blackhole(cdll_t *galaxy, int pos)
{
	if (galaxy == NULL) {
		fprintf(stderr, "Galaxy has not been created\n");
		return;
	}

	if (galaxy->head == NULL) {
		printf("Planet out of bounds!\n");
		return;
	}

	if (pos > galaxy->size - 1) {
		printf("Planet out of bounds!\n");
		return;
	}

	// Positioning on the right planet
	node_t *planet_to_remove = galaxy->head;
	for(int i = 0; i < pos; i++) {
		planet_to_remove = planet_to_remove->next;
	}

	// Copying the name of the planet that has to implode before removing it
	char name[NMAX];
	memcpy(name, ((specs_t *)planet_to_remove->data)->name, NMAX);
	planet_to_remove = remove_nth_node(galaxy, pos);

	free_the_shields(planet_to_remove);
	free(((specs_t *)planet_to_remove->data));
	free(planet_to_remove);

	if (galaxy->size == 0) {
		galaxy->head = NULL;
	}

	printf("The planet %s has been eaten by the vortex.\n", name);
}


void remove_shield(cdll_t *galaxy, int pos, int pos2)
{
	if (galaxy == NULL) {
		fprintf(stderr, "Galaxy has not been created\n");
		return;
	}

	if (galaxy->head == NULL) {
		printf("Planet out of bounds!\n");
		return;
	}

	if (pos > galaxy->size - 1) {
		printf("Planet out of bounds!\n");
		return;
	}

	// Positioning on the right planet
	node_t *planet = galaxy->head;
	for (int i = 0; i < pos; i++) {
		planet = planet->next;
	}

	// Positioning on the right shield
	if (pos2 >= ((specs_t *)planet->data)->shields) {
		printf("Shield out of bounds!\n");
		return;
	}

	if (((specs_t *)planet->data)->shields <= 4) {
		printf("A planet cannot have less than 4 shields!\n");
		return;
	}

	// Both removing the shield and freeing its memory
	node_t *shield_to_remove;
	cdll_t *list_of_shields = ((specs_t *)planet->data)->list_of_shields;
	shield_to_remove = remove_nth_node(list_of_shields, pos2);
	free(shield_to_remove->data);
	free(shield_to_remove);
	((specs_t *)planet->data)->shields--;
}


void upg_shield(cdll_t *galaxy, int pos, int pos2, int upgrade)
{
	if (galaxy->size == 0) {
		printf("Planet out of bounds!\n");
		return;
	}

	if (pos > galaxy->size - 1) {
		printf("Planet out of bounds!\n");
		return;
	}

	// Positioning on the right planet
	node_t *planet = galaxy->head;
	for (int i = 0; i < pos; i++) {
		planet = planet->next;
	}

	if (pos2 >= ((specs_t *)planet->data)->shields) {
		printf("Shield out of bounds!\n");
		return;
	}

	// Positioning on the right shield
	node_t *shield = ((specs_t *)planet->data)->list_of_shields->head;
	for (int i = 0; i < pos2; i++) {
		shield = shield->next;
	}

	*(int *)shield->data = *(int *)shield->data + upgrade;
}


void expand(cdll_t *galaxy, int pos, int value)
{
	if (galaxy->size == 0) {
		printf("Planet out of bounds!\n");
		return;
	}

	if (pos > galaxy->size - 1) {
		printf("Planet out of bounds!\n");
		return;
	}

	// Positioning on the right planet
	node_t *planet = galaxy->head;
	for (int i = 0; i < pos; i++) {
		planet = planet->next;
	}

	// Creating a new shield with the upgrade value
	node_t *new_shield = malloc(sizeof(node_t));
	DIE(!new_shield, "Error while doing new_shield\n");
	new_shield->data = malloc(sizeof(int));
	DIE(!new_shield->data, "Error while doing new_shield->data\n");
	*(int *)new_shield->data = value;

	// Positioning the new shield at the end of the list of shields
	node_t *tail = ((specs_t *)planet->data)->list_of_shields->head->prev;
	tail->next = new_shield;
	new_shield->next = ((specs_t *)planet->data)->list_of_shields->head;
	new_shield->prev = tail;
	((specs_t *)planet->data)->list_of_shields->head->prev = new_shield;
	((specs_t *)planet->data)->list_of_shields->size++;
	((specs_t *)planet->data)->shields++;
}


/*
	First of all, we have to position the planet1 and then planet2
will take planet1->next. We position the shield1 of planet1 and shield2
of planet2 respectively at shields->size/4 and 3*shields->size/4.
	In order for a planet to implode it has to have one shield with a negative
value before the end of the function. For this there are three if statements
that carefully check all cases in order to remove the right planet and free its
memory. 
*/
void collide(cdll_t *galaxy, int pos, int pos2)
{
	if (galaxy == NULL) {
		fprintf(stderr, "Galaxy has not been created\n");
		return;
	}

	if (galaxy->head == NULL) {
		printf("Planet out of bounds!\n");
		return;
	}

	if (pos > galaxy->size - 1 || pos2 > galaxy->size - 1) {
		printf("Planet out of bounds!\n");
		return;
	}

	node_t *planet_to_remove;
	node_t *planet1 = galaxy->head;
	for (int i = 0; i < pos; i++) {
		planet1 = planet1->next;
	}
	node_t *planet2 = planet1->next;

	node_t *shield1 = ((specs_t *)planet1->data)->list_of_shields->head;
	node_t *shield2 = ((specs_t *)planet2->data)->list_of_shields->head;

	for (int i = 0; i < ((specs_t *)planet1->data)->shields / 4; i++) {
		shield1 = shield1->next;
	}

	for (int i = 0; i < (((specs_t *)planet2->data)->shields / 4) * 3; i++) {
		shield2 = shield2->next;
	}

	*(int *)shield1->data = *(int *)shield1->data - 1;
	*(int *)shield2->data = *(int *)shield2->data - 1;

	if (*(int *)shield1->data != -1 && *(int *)shield2->data == -1) {
		printf("The planet %s has imploded.\n", ((specs_t *)planet2->data)->name);
		planet_to_remove = remove_nth_node(galaxy, pos2);
		free_the_shields(planet_to_remove);
		free(((specs_t *)planet_to_remove->data));
		free(planet_to_remove);
		((specs_t *)planet1->data)->kills = ((specs_t *)planet1->data)->kills + 1;
	} else if (*(int *)shield1->data == -1 && *(int *)shield2->data != -1) {
		printf("The planet %s has imploded.\n", ((specs_t *)planet1->data)->name);
		planet_to_remove = remove_nth_node(galaxy, pos);
		free_the_shields(planet_to_remove);
		free(((specs_t *)planet_to_remove->data));
		free(planet_to_remove);
		((specs_t *)planet2->data)->kills = ((specs_t *)planet2->data)->kills + 1;
	} else if (*(int *)shield1->data == -1 && *(int *)shield2->data == -1) {
		printf("The planet %s has imploded.\n", ((specs_t *)planet1->data)->name);
		printf("The planet %s has imploded.\n", ((specs_t *)planet2->data)->name);
		planet_to_remove = remove_nth_node(galaxy, pos);
		free_the_shields(planet_to_remove);
		free(((specs_t *)planet_to_remove->data));
		free(planet_to_remove);
		if (pos2 == 0) {
			planet_to_remove = remove_nth_node(galaxy, pos2);
			free_the_shields(planet_to_remove);
			free(((specs_t *)planet_to_remove->data));
			free(planet_to_remove);
		} else {
			planet_to_remove = remove_nth_node(galaxy, pos2 - 1);
			free_the_shields(planet_to_remove);
			free(((specs_t *)planet_to_remove->data));
			free(planet_to_remove);
		}
	}
}


void rotate(cdll_t *galaxy, int pos, int value, int sem)
{
	if (galaxy == NULL) {
		fprintf(stderr, "Galaxy has not been created\n");
		return;
	}

	if (galaxy->head == NULL) {
		printf("Planet out of bounds!\n");
		return;
	}

	if (pos > galaxy->size - 1) {
		printf("Planet out of bounds!\n");
		return;
	}

	// Positioning on the right planet
	node_t *planet = galaxy->head;
	for (int i = 0; i < pos; i++) {
		planet = planet->next;
	}
	value = value % (((specs_t *)planet->data)->shields);
	node_t *shield = ((specs_t *)planet->data)->list_of_shields->head;

	// Sem == 1 (ceasornic) and Sem == 0 (trigonometric)
	if (sem == 1) {
		for (int i = 0; i < value; i++) {
			shield = shield->prev;
		}
		((specs_t *)planet->data)->list_of_shields->head = shield;
	} else if (sem == 0) {
		for (int i = 0; i < value; i++) {
			shield = shield->next;
		}
		((specs_t *)planet->data)->list_of_shields->head = shield;
	}
}

// Console ( implemented close to the one given in the lab skel )
int main(void) {
	specs_t specs;
	int pos, pos2, upgrade, nr_commands, sem, value;
	char sens;
	cdll_t *galaxy = create_list(sizeof(specs_t));

	scanf("%d", &nr_commands);

	while(nr_commands > 0) {
		char command[16];

		scanf("%s", command);

		if (strcmp(command, "ADD") == 0) {
			scanf("%s%d%d", specs.name, &pos, &(specs.shields));
			specs.kills = 0;
			add_planet(galaxy, specs, pos);
			nr_commands--;
		}
		if (strcmp(command, "RMV") == 0) {
			scanf("%d%d", &pos, &pos2);
			remove_shield(galaxy, pos, pos2);
			nr_commands--;
		}
		if (strcmp(command, "SHW") == 0) {
			scanf("%d", &pos);
			show(galaxy, pos);
			nr_commands--;
		}
		if (strcmp(command, "BLH") == 0) {
			scanf("%d", &pos);
			blackhole(galaxy, pos);
			nr_commands--;
		}
		if (strcmp(command, "UPG") == 0) {
			scanf("%d%d%d", &pos, &pos2, &upgrade);
			upg_shield(galaxy, pos, pos2, upgrade);
			nr_commands--;
		}
		if (strcmp(command, "EXP") == 0) {
			scanf("%d", &pos);
			scanf("%d", &value);
			expand(galaxy, pos, value);
			nr_commands--;
		}
		if (strcmp(command, "COL") == 0) {
			scanf("%d%d", &pos, &pos2);
			collide(galaxy, pos, pos2);
			nr_commands--;
		}
		if (strcmp(command, "ROT") == 0) {
			scanf("%d %c%d", &pos, &sens, &value);
			if(sens == 'c') {
				sem = 1;
				rotate(galaxy, pos, value, sem);
			} else if (sens == 't') {
				sem = 0;
				rotate(galaxy, pos, value, sem);
			} else {
				printf("Not a valid direction!\n");
			}
			nr_commands--;
		}
	}
	free_the_galaxy(&galaxy);
	return 0;
}
