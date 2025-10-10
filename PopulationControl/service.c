#include "service.h"
#include "repository.h"
#include "country.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

Service* CreateService(DynamicArray* repo) {
    Service* s = (Service*)malloc(sizeof(Service));
    if (!s) return NULL;

    s->repo = repo;
    s->undoStack = createOperationStack();
    s->redoStack = createOperationStack();
    return s;
}

void DestroyService(Service* s) {
    if (!s) return;

    DestroyDynamicArray(s->repo);
    destroyOperationStack(s->undoStack);
    destroyOperationStack(s->redoStack);
    free(s);
}

void generate_countries(Service* s) {
    if (!s) return;

    Country* country;

    country = createCountry("France", "Europe", 67000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("Brazil", "South America", 213000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("Japan", "Asia", 126000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("Canada", "North America", 38000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("Nigeria", "Africa", 206000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("Australia", "Australia", 25000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("India", "Asia", 1391000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("Germany", "Europe", 83000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("Argentina", "South America", 45000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);

    country = createCountry("South Africa", "Africa", 59000000);
    AddCountryToRepo(s->repo, country);
    freeCountry(country);
}

int AddCountry(Service* s, char* name, char* continent, long population) {
    if (!s || !name || !continent) return 0;

    if (!validateCountry(name, continent, population)) {
        return 0;
    }

    for (int i = 0; i < s->repo->size; i++) {
        if (strcmp(get_name(&s->repo->data[i]), name) == 0) {
            return 0; 
        }
    }

    Country* newCountry = createCountry(name, continent, population);
    if (!newCountry) return 0;

    clearOperationStack(s->redoStack);

    Country opCountry;
    opCountry.name = _strdup(name);
    opCountry.continent = _strdup(continent);
    opCountry.population = population;

    Operation* op = createOperation(opCountry, "add");
    pushOperation(s->undoStack, op);
    destroyOperation(op);

    free(opCountry.name);
    free(opCountry.continent);

    AddCountryToRepo(s->repo, newCountry);
    freeCountry(newCountry);

    return 1;
}

int RemoveCountry(Service* s, char* name) {
    if (!s || !name) return 0;

    int index = -1;
    for (int i = 0; i < s->repo->size; i++) {
        if (strcmp(get_name(&s->repo->data[i]), name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1)
        return 0;

    clearOperationStack(s->redoStack);

    Country tempCountry;
    tempCountry.name = _strdup(s->repo->data[index].name);
    tempCountry.continent = _strdup(s->repo->data[index].continent);
    tempCountry.population = s->repo->data[index].population;

    Operation* op = createOperation(tempCountry, "remove");
    pushOperation(s->undoStack, op);
    destroyOperation(op);

    free(tempCountry.name);
    free(tempCountry.continent);

    RemoveCountryToRepo(s->repo, name, index);
    return 1;
}

int UpdateCountryPopulation(Service* s, char* name, int population) {
    if (!s || !name || population <= 0 || population > 8000000000) return 0;

    int index = -1;
    for (int i = 0; i < s->repo->size; i++) {
        if (strcmp(get_name(&s->repo->data[i]), name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1)
        return 0;

    clearOperationStack(s->redoStack);

    Country tempCountry;
    tempCountry.name = _strdup(s->repo->data[index].name);
    tempCountry.continent = _strdup(s->repo->data[index].continent);
    tempCountry.population = s->repo->data[index].population;

    Operation* op = createOperation(tempCountry, "update_population");
    pushOperation(s->undoStack, op);
    destroyOperation(op);

    free(tempCountry.name);
    free(tempCountry.continent);

    UpdateCountryPopulationRepo(s->repo, name, population, index);
    return 1;
}

int UpdateCountryContinent(Service* s, char* name, char* continent) {
    if (!s || !name || !continent) return 0;

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

    int index = -1;
    for (int i = 0; i < s->repo->size; i++) {
        if (strcmp(get_name(&s->repo->data[i]), name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1)
        return 0;

    clearOperationStack(s->redoStack);

    // Create a deep copy of the country for the operation
    Country tempCountry;
    tempCountry.name = _strdup(s->repo->data[index].name);
    tempCountry.continent = _strdup(s->repo->data[index].continent);
    tempCountry.population = s->repo->data[index].population;

    Operation* op = createOperation(tempCountry, "update_continent");
    pushOperation(s->undoStack, op);
    destroyOperation(op);

    free(tempCountry.name);
    free(tempCountry.continent);

    UpdateCountryContinentRepo(s->repo, name, continent, index);
    return 1;
}

int UpdateMigration(Service* s, char* emigrants, char* immigrants, long population) {
    if (!s || !emigrants || !immigrants || population <= 0) return 0;

    int index1 = -1, index2 = -1;
    for (int i = 0; i < s->repo->size; i++) {
        if (strcmp(get_name(&s->repo->data[i]), emigrants) == 0) {
            index1 = i;
        }
        if (strcmp(get_name(&s->repo->data[i]), immigrants) == 0) {
            index2 = i;
        }
    }
    if (index1 == -1 || index2 == -1)
        return 0;

    if (s->repo->data[index1].population < population)
        return 0;

    clearOperationStack(s->redoStack);

    Country tempCountry1;
    tempCountry1.name = _strdup(s->repo->data[index1].name);
    tempCountry1.continent = _strdup(s->repo->data[index1].continent);
    tempCountry1.population = s->repo->data[index1].population;

    Country tempCountry2;
    tempCountry2.name = _strdup(s->repo->data[index2].name);
    tempCountry2.continent = _strdup(s->repo->data[index2].continent);
    tempCountry2.population = s->repo->data[index2].population;

    Operation* op1 = createOperation(tempCountry1, "update_migration_from");
    pushOperation(s->undoStack, op1);
    destroyOperation(op1);

    Operation* op2 = createOperation(tempCountry2, "update_migration_to");
    pushOperation(s->undoStack, op2);
    destroyOperation(op2);

    free(tempCountry1.name);
    free(tempCountry1.continent);
    free(tempCountry2.name);
    free(tempCountry2.continent);

    long newPopSource = s->repo->data[index1].population - population;
    long newPopDest = s->repo->data[index2].population + population;

    UpdateCountryPopulationRepo(s->repo, emigrants, newPopSource, index1);
    UpdateCountryPopulationRepo(s->repo, immigrants, newPopDest, index2);
    return 1;
}

int matchesPartialName(const char* fullName, const char* partialName) {
    if (!fullName || !partialName) return 0;
    if (strlen(partialName) == 0) return 1; 

    char* lowerFullName = (char*)malloc(strlen(fullName) + 1);
    char* lowerPartialName = (char*)malloc(strlen(partialName) + 1);

    if (!lowerFullName || !lowerPartialName) {
        free(lowerFullName);
        free(lowerPartialName);
        return 0;
    }

    for (int i = 0; fullName[i]; i++) {
        lowerFullName[i] = tolower(fullName[i]);
    }
    lowerFullName[strlen(fullName)] = '\0';

    for (int i = 0; partialName[i]; i++) {
        lowerPartialName[i] = tolower(partialName[i]);
    }
    lowerPartialName[strlen(partialName)] = '\0';

    int result = strstr(lowerFullName, lowerPartialName) != NULL;

    free(lowerFullName);
    free(lowerPartialName);

    return result;
}

int compareByName(const void* a, const void* b) {
    Country* countryA = (Country*)a;
    Country* countryB = (Country*)b;
    return strcmp(countryA->name, countryB->name);
}

int compareByPopulation(const void* a, const void* b) {
    Country* countryA = (Country*)a;
    Country* countryB = (Country*)b;
    return (countryB->population - countryA->population);
}

DynamicArray* DisplayByName(Service* s, char* str) {
    if (!s || !str) return NULL;

    DynamicArray* country_array = createDynamicArray(s->repo->capacity);
    if (!country_array) return NULL;

    for (int i = 0; i < s->repo->size; i++) {
        if (matchesPartialName(get_name(&s->repo->data[i]), str)) {
            AddCountryToRepo(country_array, &s->repo->data[i]);
        }
    }

    if (country_array->size == 0) {
        DestroyDynamicArray(country_array);
        return NULL;
    }

    qsort(country_array->data, country_array->size, sizeof(Country), compareByName);
    return country_array;
}

DynamicArray* DisplayByContinent(Service* s, char* str, long population) {
    if (!s || !str) return NULL;

    DynamicArray* continent_array = createDynamicArray(s->repo->capacity);
    if (!continent_array) return NULL;

    for (int i = 0; i < s->repo->size; i++) {
        if (matchesPartialName(get_continent(&s->repo->data[i]), str) &&
            get_population(&s->repo->data[i]) > population) {
            AddCountryToRepo(continent_array, &s->repo->data[i]);
        }
    }

    if (continent_array->size == 0) {
        DestroyDynamicArray(continent_array);
        return NULL;
    }

    qsort(continent_array->data, continent_array->size, sizeof(Country), compareByPopulation);
    return continent_array;
}

void print_countries(Service* s) {
    if (!s) return;
    print_all(s->repo);
}

int Undo(Service* s) {
    if (!s || s->undoStack->size == 0) {
        return 0;
    }

    Operation* op = popOperation(s->undoStack);
    if (op == NULL) {
        return 0;
    }

    Country* country = getOperationProduct(op);
    char* type = getOperationType(op);

    if (strcmp(type, "add") == 0) {
        // Undo add = remove
        int index = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            pushOperation(s->redoStack, op);

            RemoveCountryToRepo(s->repo, get_name(country), index);
        }
    }
    else if (strcmp(type, "remove") == 0) {
        // Undo remove = add
        pushOperation(s->redoStack, op);

        AddCountryToRepo(s->repo, country);
    }
    else if (strcmp(type, "update_population") == 0) {
        // Undo update population = restore old population
        int index = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            Country tempCountry;
            tempCountry.name = _strdup(s->repo->data[index].name);
            tempCountry.continent = _strdup(s->repo->data[index].continent);
            tempCountry.population = s->repo->data[index].population;

            Operation* redoOp = createOperation(tempCountry, "update_population");
            pushOperation(s->redoStack, redoOp);
            destroyOperation(redoOp);
            free(tempCountry.name);
            free(tempCountry.continent);

            UpdateCountryPopulationRepo(s->repo, get_name(country), get_population(country), index);
        }
    }
    else if (strcmp(type, "update_continent") == 0) {
        // Undo update continent = restore old continent
        int index = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            Country tempCountry;
            tempCountry.name = _strdup(s->repo->data[index].name);
            tempCountry.continent = _strdup(s->repo->data[index].continent);
            tempCountry.population = s->repo->data[index].population;

            Operation* redoOp = createOperation(tempCountry, "update_continent");
            pushOperation(s->redoStack, redoOp);
            destroyOperation(redoOp);

            free(tempCountry.name);
            free(tempCountry.continent);

            UpdateCountryContinentRepo(s->repo, get_name(country), get_continent(country), index);
        }
    }
    else if (strcmp(type, "update_migration_to") == 0) {
        if (s->undoStack->size == 0) {
            pushOperation(s->undoStack, op);
            destroyOperation(op);
            return 0;
        }

        Operation* op2 = popOperation(s->undoStack);
        if (op2 == NULL || strcmp(getOperationType(op2), "update_migration_from") != 0) {
            pushOperation(s->undoStack, op2);
            pushOperation(s->undoStack, op);
            destroyOperation(op);
            if (op2) destroyOperation(op2);
            return 0;
        }

        Country* country2 = getOperationProduct(op2);

        int index1 = -1, index2 = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country2)) == 0) {
                index1 = i;
            }
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index2 = i;
            }
        }

        if (index1 != -1 && index2 != -1) {
            Country tempCountry1;
            tempCountry1.name = _strdup(s->repo->data[index1].name);
            tempCountry1.continent = _strdup(s->repo->data[index1].continent);
            tempCountry1.population = s->repo->data[index1].population;

            Country tempCountry2;
            tempCountry2.name = _strdup(s->repo->data[index2].name);
            tempCountry2.continent = _strdup(s->repo->data[index2].continent);
            tempCountry2.population = s->repo->data[index2].population;

            Operation* redoOp1 = createOperation(tempCountry1, "update_migration_from");
            pushOperation(s->redoStack, redoOp1);
            destroyOperation(redoOp1);

            Operation* redoOp2 = createOperation(tempCountry2, "update_migration_to");
            pushOperation(s->redoStack, redoOp2);
            destroyOperation(redoOp2);

            free(tempCountry1.name);
            free(tempCountry1.continent);
            free(tempCountry2.name);
            free(tempCountry2.continent);

            UpdateCountryPopulationRepo(s->repo, get_name(country2), get_population(country2), index1);
            UpdateCountryPopulationRepo(s->repo, get_name(country), get_population(country), index2);
        }

        destroyOperation(op2);
    }
    else if (strcmp(type, "update_migration_from") == 0) {
        pushOperation(s->undoStack, op);
        return 0;
    }

    destroyOperation(op);
    return 1;
}

int Redo(Service* s) {
    if (!s || s->redoStack->size == 0) {
        return 0;
    }

    Operation* op = popOperation(s->redoStack);
    if (op == NULL) {
        return 0;
    }

    Country* country = getOperationProduct(op);
    char* type = getOperationType(op);

    if (strcmp(type, "add") == 0) {
        // Redo add = add
        pushOperation(s->undoStack, op);

        AddCountryToRepo(s->repo, country);
    }
    else if (strcmp(type, "remove") == 0) {
        // Redo remove = remove
        int index = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            pushOperation(s->undoStack, op);

            RemoveCountryToRepo(s->repo, get_name(country), index);
        }
    }
    else if (strcmp(type, "update_population") == 0) {
        // Redo update population = update to new population
        int index = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            Country tempCountry;
            tempCountry.name = _strdup(s->repo->data[index].name);
            tempCountry.continent = _strdup(s->repo->data[index].continent);
            tempCountry.population = s->repo->data[index].population;

            Operation* undoOp = createOperation(tempCountry, "update_population");
            pushOperation(s->undoStack, undoOp);
            destroyOperation(undoOp);

            free(tempCountry.name);
            free(tempCountry.continent);

            UpdateCountryPopulationRepo(s->repo, get_name(country), get_population(country), index);
        }
    }
    else if (strcmp(type, "update_continent") == 0) {
        int index = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            Country tempCountry;
            tempCountry.name = _strdup(s->repo->data[index].name);
            tempCountry.continent = _strdup(s->repo->data[index].continent);
            tempCountry.population = s->repo->data[index].population;

            Operation* undoOp = createOperation(tempCountry, "update_continent");
            pushOperation(s->undoStack, undoOp);
            destroyOperation(undoOp);

            free(tempCountry.name);
            free(tempCountry.continent);

            UpdateCountryContinentRepo(s->repo, get_name(country), get_continent(country), index);
        }
    }
    else if (strcmp(type, "update_migration_to") == 0) {
        if (s->redoStack->size == 0) {
            pushOperation(s->redoStack, op);
            destroyOperation(op);
            return 0;
        }

        Operation* op2 = popOperation(s->redoStack);
        if (op2 == NULL || strcmp(getOperationType(op2), "update_migration_from") != 0) {
            pushOperation(s->redoStack, op2);
            pushOperation(s->redoStack, op);
            destroyOperation(op);
            if (op2) destroyOperation(op2);
            return 0;
        }

        Country* country2 = getOperationProduct(op2);

        int index1 = -1, index2 = -1;
        for (int i = 0; i < s->repo->size; i++) {
            if (strcmp(get_name(&s->repo->data[i]), get_name(country2)) == 0) {
                index1 = i;
            }
            if (strcmp(get_name(&s->repo->data[i]), get_name(country)) == 0) {
                index2 = i;
            }
        }

        if (index1 != -1 && index2 != -1) {
            Country tempCountry1;
            tempCountry1.name = _strdup(s->repo->data[index1].name);
            tempCountry1.continent = _strdup(s->repo->data[index1].continent);
            tempCountry1.population = s->repo->data[index1].population;

            Country tempCountry2;
            tempCountry2.name = _strdup(s->repo->data[index2].name);
            tempCountry2.continent = _strdup(s->repo->data[index2].continent);
            tempCountry2.population = s->repo->data[index2].population;

            Operation* undoOp1 = createOperation(tempCountry1, "update_migration_from");
            pushOperation(s->undoStack, undoOp1);
            destroyOperation(undoOp1);

            Operation* undoOp2 = createOperation(tempCountry2, "update_migration_to");
            pushOperation(s->undoStack, undoOp2);
            destroyOperation(undoOp2);

            free(tempCountry1.name);
            free(tempCountry1.continent);
            free(tempCountry2.name);
            free(tempCountry2.continent);

            UpdateCountryPopulationRepo(s->repo, get_name(country2), get_population(country2), index1);
            UpdateCountryPopulationRepo(s->repo, get_name(country), get_population(country), index2);
        }

        destroyOperation(op2);
    }
    else if (strcmp(type, "update_migration_from") == 0) {
        pushOperation(s->redoStack, op);
        return 0;
    }

    destroyOperation(op);
    return 1;
}