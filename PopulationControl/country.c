#include "country.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

Country* createCountry(char* name, char* continent, long pop) {
    Country* country = (Country*)malloc(sizeof(Country));
    if (!country) return NULL;

    country->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    if (!country->name) {
        free(country);
        return NULL;
    }

    country->continent = (char*)malloc(sizeof(char) * (strlen(continent) + 1));
    if (!country->continent) {
        free(country->name);
        free(country);
        return NULL;
    }

    strcpy(country->name, name);
    strcpy(country->continent, continent);
    country->population = pop;
    return country;
}
void freeCountry(Country* country) {
    if (!country) return;
    free(country->name);
    free(country->continent);
    free(country);
}

int validateCountry(char* name, char* continent, long population) {
    if (name == NULL || strlen(name) == 0) {
        return 0;
    }

    if (strlen(name) > 50) {
        return 0;
    }

    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && !isspace(name[i]) && name[i] != '-' && name[i] != '\'') {
            return 0;
        }
    }

    if (continent == NULL || strlen(continent) == 0) {
        return 0;
    }

    const char* validContinents[] = { "Africa", "Asia", "Europe", "North America", "South America", "Australia", "Antarctica", "Oceania" };
    int validContinent = 0;

    for (int i = 0; i < 8; i++) {
        if (strcmp(continent, validContinents[i]) == 0) {
            validContinent = 1;
            break;
        }
    }

    if (!validContinent) {
        return 0;
    }

    if (population <= 0 || population > 8000000000) {
        return 0;
    }
    return 1;
}

void destroyCountry(Country* country) {
    if (country == NULL) return;
    free(country->name);
    free(country->continent);
    // Note: Don't free country itself here, as it might be part of an array
}

char* get_name(Country* country) {
    return country->name;
}

char* get_continent(Country* country) {
    return country->continent;
}

long get_population(Country* country) {
    return country->population;
}