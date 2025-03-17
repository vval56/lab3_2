#ifndef FUNCTIONS2_H
#define FUNCTIONS2_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

typedef struct time_of_excursion {
    char start_excursion[11];
    int amount;
} time_of_excursion;

typedef struct Excursion {
    char *name;
    double cost;
    time_of_excursion time;
} Excursion;

double check_double();
void start_excursion_check(char *date);
void input_excursion(Excursion *tour);
void print_excursion(const Excursion *tour);
void delete_excursion(Excursion **tours, int *count, const char *name);
void find_excursion(const Excursion *tours, int count, const char *name);
void structure_task(Excursion **tours, int *count);

#endif
