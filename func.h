// Copyright 2021 < Copyright Maciuca Alexandru Petru 314CA
#ifndef FUNC_H_
#define FUNC_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./utils.h"

#define NMAX 31

typedef struct node_t node_t;
struct node_t
{
	node_t *prev;
	node_t *next;
	void *data;
};

typedef struct cdll_t cdll_t;
struct cdll_t
{
	node_t* head;
    int size;
    int data_size;
};

typedef struct specs_t specs_t;
struct specs_t
{
	char name[NMAX];
	int shields;
	int kills;
	cdll_t *list_of_shields;
};


cdll_t* create_list(int data_size);


void add_shield(node_t *planet, cdll_t *list_of_shields, specs_t specs);


void add_planet(cdll_t *galaxy, specs_t specs, int n);


void print_shields(node_t *planet);


void show(cdll_t *galaxy, int pos);


void free_the_shields(node_t *planet);


void free_the_galaxy(cdll_t **pp_galaxy);


node_t* remove_nth_node(cdll_t *list, int n);


void blackhole(cdll_t *galaxy, int pos);


void remove_shield(cdll_t *galaxy, int pos, int pos2);


void upg_shield(cdll_t *galaxy, int pos, int pos2, int upgrade);


void expand(cdll_t *galaxy, int pos, int value);


void collide(cdll_t *galaxy, int pos, int pos2);


void rotate(cdll_t *galaxy, int pos, int upgrade, int sem);

#endif  // FUNC_H_
