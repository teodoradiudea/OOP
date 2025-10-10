#pragma once

typedef struct {
    char* name;
    char* continent;
    long population;
} Country;

Country* createCountry(char* n, char* ct, long p);
void freeCountry(Country* country);
int validateCountry(char* name, char* continent, long population);
void destroyCountry(Country* country);
char* get_name(Country* country);
char* get_continent(Country* country);
long get_population(Country* country);