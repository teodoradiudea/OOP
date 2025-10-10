#pragma once
#include "array.h"

void AddCountryToRepo(DynamicArray* r, Country* country);
void RemoveCountryToRepo(DynamicArray* r, char* name, int index);
void UpdateCountryPopulationRepo(DynamicArray* repo, char* name, int population, int index);
void UpdateCountryContinentRepo(DynamicArray* repo, char* name, char* continent, int index);
void print_all(DynamicArray* arr);
