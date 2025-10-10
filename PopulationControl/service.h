#pragma once
#include "array.h"
#include "operation_stack.h"

typedef struct {
    DynamicArray* repo;
    OperationStack* undoStack;
    OperationStack* redoStack;
} Service;

Service* CreateService(DynamicArray* repo);
void DestroyService(Service* s);
void generate_countries(Service* s);
void print_countries(Service* s);

// CRUD Operations
int AddCountry(Service* s, char* name, char* continent, long population);
int UpdateCountryPopulation(Service* s, char* name, int population);
int UpdateCountryContinent(Service* s, char* name, char* continent);
int RemoveCountry(Service* s, char* name);

// Special Operations
int UpdateMigration(Service* s, char* emigrants, char* immigrants, long population);

// Filter Operations
DynamicArray* DisplayByName(Service* s, char* partialName);
DynamicArray* DisplayByContinent(Service* s, char* continent, long minPopulation);

// Undo/Redo Operations
int Undo(Service* s);
int Redo(Service* s);
