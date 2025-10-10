#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tests.h"
#include "country.h"
#include "array.h"
#include "repository.h"
#include "service.h"
#include "operation_stack.h"

void testCountryFunctions() {
    printf("Testing country functions...\n");

    // Test country creation
    Country* country = createCountry("Romania", "Europe", 19000000);
    assert(country != NULL);
    assert(strcmp(country->name, "Romania") == 0);
    assert(strcmp(country->continent, "Europe") == 0);
    assert(country->population == 19000000);

    // Test getters
    assert(strcmp(get_name(country), "Romania") == 0);
    assert(strcmp(get_continent(country), "Europe") == 0);
    assert(get_population(country) == 19000000);

    // Test validation
    assert(validateCountry("Romania", "Europe", 19000000) == 1);
    assert(validateCountry("", "Europe", 19000000) == 0);
    assert(validateCountry("Romania", "InvalidContinent", 19000000) == 0);
    assert(validateCountry("Romania", "Europe", -1) == 0);

    free(country->name);
    free(country->continent);
    free(country);

    printf("Country functions tests passed!\n");
}

void testDynamicArray() {
    printf("Testing dynamic array functions...\n");

    // Test array creation
    DynamicArray* array = createDynamicArray(2);
    assert(array != NULL);
    assert(array->capacity == 2);
    assert(array->size == 0);
    assert(array->data != NULL);

    // Test adding countries
    Country* country1 = createCountry("Romania", "Europe", 19000000);
    Country* country2 = createCountry("France", "Europe", 67000000);
    Country* country3 = createCountry("Japan", "Asia", 126000000);

    AddCountryToRepo(array, country1);
    assert(array->size == 1);
    assert(strcmp(array->data[0].name, "Romania") == 0);

    AddCountryToRepo(array, country2);
    assert(array->size == 2);
    assert(strcmp(array->data[1].name, "France") == 0);

    // Test resizing
    AddCountryToRepo(array, country3);
    assert(array->size == 3);
    assert(array->capacity == 4);
    assert(strcmp(array->data[2].name, "Japan") == 0);

    free(country1);
    free(country2);
    free(country3);
    DestroyDynamicArray(array);

    printf("Dynamic array tests passed!\n");
}

void testRepository() {
    printf("Testing repository functions...\n");

    DynamicArray* repo = createDynamicArray(5);

    Country* country1 = createCountry("Romania", "Europe", 19000000);
    Country* country2 = createCountry("France", "Europe", 67000000);

    // Add countries to the repository
    AddCountryToRepo(repo, country1);
    assert(repo->size == 1);
    assert(strcmp(repo->data[0].name, "Romania") == 0);

    AddCountryToRepo(repo, country2);
    assert(repo->size == 2);
    assert(strcmp(repo->data[1].name, "France") == 0);

    // Test updating population
    UpdateCountryPopulationRepo(repo, "Romania", 20000000, 0);
    assert(repo->data[0].population == 20000000);

    // Test updating continent
    UpdateCountryContinentRepo(repo, "Romania", "Eastern Europe", 0);
    assert(strcmp(repo->data[0].continent, "Eastern Europe") == 0);

    // Test removing country
    RemoveCountryToRepo(repo, "France", 1);
    assert(repo->size == 1);
    assert(strcmp(repo->data[0].name, "Romania") == 0);

    free(country1);
    free(country2);

    DestroyDynamicArray(repo);

    printf("Repository tests passed!\n");
}

void testService() {
    printf("Testing service functions...\n");

    DynamicArray* repo = createDynamicArray(10);
    Service* service = CreateService(repo);

    // Test generating countries
    generate_countries(service);
    assert(service->repo->size == 10);

    // Test adding a country
    int result = AddCountry(service, "Romania", "Europe", 19000000);
    assert(result == 1);
    assert(service->repo->size == 11);

    // Test removing a country
    result = RemoveCountry(service, "Romania");
    assert(result == 1);
    assert(service->repo->size == 10);

    // Test updating population
    result = AddCountry(service, "TestCountry", "Europe", 5000000);
    assert(result == 1);

    result = UpdateCountryPopulation(service, "TestCountry", 6000000);
    assert(result == 1);

    // Find the country and check its population
    int index = -1;
    for (int i = 0; i < service->repo->size; i++) {
        if (strcmp(get_name(&service->repo->data[i]), "TestCountry") == 0) {
            index = i;
            break;
        }
    }
    assert(index != -1);
    assert(service->repo->data[index].population == 6000000);

    // Test updating continent
    result = UpdateCountryContinent(service, "TestCountry", "Asia");
    assert(result == 1);
    assert(strcmp(service->repo->data[index].continent, "Asia") == 0);

    // Test migration
    result = AddCountry(service, "SourceCountry", "Europe", 10000000);
    assert(result == 1);

    result = AddCountry(service, "DestinationCountry", "Europe", 5000000);
    assert(result == 1);

    result = UpdateMigration(service, "SourceCountry", "DestinationCountry", 1000000);
    assert(result == 1);

    // Find countries and check populations
    int sourceIndex = -1, destIndex = -1;
    for (int i = 0; i < service->repo->size; i++) {
        if (strcmp(get_name(&service->repo->data[i]), "SourceCountry") == 0) {
            sourceIndex = i;
        }
        if (strcmp(get_name(&service->repo->data[i]), "DestinationCountry") == 0) {
            destIndex = i;
        }
    }

    assert(sourceIndex != -1);
    assert(destIndex != -1);
    assert(service->repo->data[sourceIndex].population == 9000000);
    assert(service->repo->data[destIndex].population == 6000000);

    // Test display by name
    DynamicArray* filtered = DisplayByName(service, "Test");
    assert(filtered != NULL);
    assert(filtered->size == 1);
    assert(strcmp(filtered->data[0].name, "TestCountry") == 0);
    DestroyDynamicArray(filtered);

    // Test display by continent
    filtered = DisplayByContinent(service, "Europe", 5000000);
    assert(filtered != NULL);
    DestroyDynamicArray(filtered);

    DestroyService(service);

    printf("Service tests passed!\n");
}

void testOperationStack() {
    printf("Testing operation stack functions...\n");

    // Test creating operation stack
    OperationStack* stack = createOperationStack();
    assert(stack != NULL);
    assert(stack->size == 0);
    assert(stack->capacity == 10);

    // Test creating and pushing operations
    Country country1 = { "TestCountry", "Europe", 5000000 };
    country1.name = _strdup("TestCountry");
    country1.continent = _strdup("Europe");

    Operation* op1 = createOperation(country1, "add");
    assert(op1 != NULL);

    pushOperation(stack, op1);
    assert(stack->size == 1);

    // Test popping operations
    Operation* poppedOp = popOperation(stack);
    assert(poppedOp != NULL);
    assert(strcmp(getOperationType(poppedOp), "add") == 0);
    assert(strcmp(getOperationProduct(poppedOp)->name, "TestCountry") == 0);

    // Test clearing stack
    Country country2 = { "AnotherCountry", "Asia", 10000000 };
    country2.name = _strdup("AnotherCountry");
    country2.continent = _strdup("Asia");

    Operation* op2 = createOperation(country2, "remove");
    pushOperation(stack, op2);
    assert(stack->size == 1);

    clearOperationStack(stack);
    assert(stack->size == 0);

    free(country1.name);
    free(country1.continent);
    free(country2.name);
    free(country2.continent);
    destroyOperation(op1);
    destroyOperation(op2);
    destroyOperationStack(stack);

    printf("Operation stack tests passed!\n");
}

void testUndoRedo() {
    printf("Testing undo and redo functionality...\n");

    DynamicArray* repo = createDynamicArray(10);
    Service* service = CreateService(repo);

    // Test undo with empty stack
    int result = Undo(service);
    assert(result == 0); 

    // Test redo with empty stack
    result = Redo(service);
    assert(result == 0); 

    // Add a country and test undo
    result = AddCountry(service, "TestCountry", "Europe", 5000000);
    assert(result == 1);
    assert(service->repo->size == 1);

    result = Undo(service);
    assert(result == 1); 
    assert(service->repo->size == 0); 

    // Test redo
    result = Redo(service);
    assert(result == 1);
    assert(service->repo->size == 1);

    result = UpdateCountryPopulation(service, "TestCountry", 6000000);
    assert(result == 1);

    int index = -1;
    for (int i = 0; i < service->repo->size; i++) {
        if (strcmp(get_name(&service->repo->data[i]), "TestCountry") == 0) {
            index = i;
            break;
        }
    }
    assert(index != -1);
    assert(service->repo->data[index].population == 6000000);

    result = Undo(service);
    assert(result == 1);
    assert(service->repo->data[index].population == 5000000);

    // Test redo after undo
    result = Redo(service);
    assert(result == 1);
    assert(service->repo->data[index].population == 6000000);

    // Test migration and undo
    result = AddCountry(service, "SourceCountry", "Europe", 10000000);
    assert(result == 1);

    result = AddCountry(service, "DestinationCountry", "Europe", 5000000);
    assert(result == 1);

    result = UpdateMigration(service, "SourceCountry", "DestinationCountry", 1000000);
    assert(result == 1);

    // Find countries and check populations
    int sourceIndex = -1, destIndex = -1;
    for (int i = 0; i < service->repo->size; i++) {
        if (strcmp(get_name(&service->repo->data[i]), "SourceCountry") == 0) {
            sourceIndex = i;
        }
        if (strcmp(get_name(&service->repo->data[i]), "DestinationCountry") == 0) {
            destIndex = i;
        }
    }

    assert(sourceIndex != -1);
    assert(destIndex != -1);
    assert(service->repo->data[sourceIndex].population == 9000000);
    assert(service->repo->data[destIndex].population == 6000000);

    // Undo migration
    result = Undo(service);
    assert(result == 1);
    assert(service->repo->data[sourceIndex].population == 10000000);
    assert(service->repo->data[destIndex].population == 5000000);

    DestroyService(service);

    printf("Undo and redo tests passed!\n");
}

void runAllTests() {
    printf("Running all tests...\n");

    testCountryFunctions();
    testDynamicArray();
    testOperationStack();
    testRepository();
    testService();
    testUndoRedo();

    printf("All tests completed successfully!\n");
}