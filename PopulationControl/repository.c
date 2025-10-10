#include "array.h"
#define _CRT_SECURE_NO_WARNINGS
#include "repository.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void AddCountryToRepo(DynamicArray* repo, Country* country) {
    if (!repo || !country) return;
    resize(repo);

    if (repo->size >= repo->capacity) {
        return;
    }

    repo->data[repo->size].name = (char*)malloc(strlen(country->name) + 1);
    if (!repo->data[repo->size].name) {
        return;
    }
    strcpy(repo->data[repo->size].name, country->name);

    repo->data[repo->size].continent = (char*)malloc(strlen(country->continent) + 1);
    if (!repo->data[repo->size].continent) {
        free(repo->data[repo->size].name);
        return;
    }
    strcpy(repo->data[repo->size].continent, country->continent);

    repo->data[repo->size].population = country->population;
    repo->size++;
}

void RemoveCountryToRepo(DynamicArray* repo, char* name, int index) {
    if (!repo || index < 0 || index >= repo->size) return;

    free(repo->data[index].name);
    free(repo->data[index].continent);

    if (index < repo->size - 1) {
        repo->data[index] = repo->data[repo->size - 1];

        repo->data[repo->size - 1].name = NULL;
        repo->data[repo->size - 1].continent = NULL;
    }

    repo->size--;
}

void UpdateCountryPopulationRepo(DynamicArray* repo, char* name, int population, int index) {
    if (!repo || index < 0 || index >= repo->size) return;
    repo->data[index].population = population;
}

void UpdateCountryContinentRepo(DynamicArray* repo, char* name, char* continent, int index) {
    if (!repo || index < 0 || index >= repo->size || !continent) return;

    free(repo->data[index].continent);

    repo->data[index].continent = (char*)malloc(strlen(continent) + 1);
    if (!repo->data[index].continent) return;

    strcpy(repo->data[index].continent, continent);
}

void print_all(DynamicArray* countries_array) {
    if (!countries_array) return;

    printf("\nList of countries:\n");
    printf("--------------------------------------------------\n");
    printf("%-20s %-15s %-15s\n", "Name", "Continent", "Population");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < countries_array->size; i++) {
        printf("%-20s %-15s %-15ld\n",
            get_name(&countries_array->data[i]),
            get_continent(&countries_array->data[i]),
            get_population(&countries_array->data[i]));
    }
    printf("--------------------------------------------------\n");
}