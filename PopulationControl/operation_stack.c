#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "operation_stack.h"

Operation* createOperation(Country country, char* type) {
    Operation* operation = (Operation*)malloc(sizeof(Operation));
    if (operation == NULL) {
        return NULL;
    }

    operation->country = (Country*)malloc(sizeof(Country));
    if (operation->country == NULL) {
        free(operation);
        return NULL;
    }

    // Deep copy of country
    operation->country->name = (char*)malloc(strlen(country.name) + 1);
    if (operation->country->name == NULL) {
        free(operation->country);
        free(operation);
        return NULL;
    }
    strcpy(operation->country->name, country.name);

    operation->country->continent = (char*)malloc(strlen(country.continent) + 1);
    if (operation->country->continent == NULL) {
        free(operation->country->name);
        free(operation->country);
        free(operation);
        return NULL;
    }
    strcpy(operation->country->continent, country.continent);

    operation->country->population = country.population;

    operation->type = (char*)malloc(strlen(type) + 1);
    if (operation->type == NULL) {
        free(operation->country->name);
        free(operation->country->continent);
        free(operation->country);
        free(operation);
        return NULL;
    }
    strcpy(operation->type, type);

    return operation;
}

void destroyOperation(Operation* operation) {
    if (operation == NULL)
        return;

    if (operation->country) {
        free(operation->country->name);
        free(operation->country->continent);
        free(operation->country);
    }

    free(operation->type);
    free(operation);
}

Operation* copyOperation(Operation* operation) {
    if (operation == NULL)
        return NULL;

    Country country;
    country.name = operation->country->name;
    country.continent = operation->country->continent;
    country.population = operation->country->population;

    return createOperation(country, operation->type);
}

Country* getOperationProduct(Operation* operation) {
    if (operation == NULL)
        return NULL;

    return operation->country;
}

char* getOperationType(Operation* operation) {
    if (operation == NULL)
        return NULL;

    return operation->type;
}

OperationStack* createOperationStack() {
    OperationStack* stack = (OperationStack*)malloc(sizeof(OperationStack));
    if (stack == NULL)
        return NULL;

    stack->capacity = 10;
    stack->size = 0;
    stack->operations = (Operation**)malloc(stack->capacity * sizeof(Operation*));

    if (stack->operations == NULL) {
        free(stack);
        return NULL;
    }

    return stack;
}

void destroyOperationStack(OperationStack* stack) {
    if (stack == NULL)
        return;

    for (int i = 0; i < stack->size; i++)
        destroyOperation(stack->operations[i]);

    free(stack->operations);
    free(stack);
}

void pushOperation(OperationStack* stack, Operation* operation) {
    if (stack == NULL || operation == NULL)
        return;

    if (stack->size >= stack->capacity) {
        stack->capacity *= 2;
        Operation** newOperations = (Operation**)realloc(stack->operations,
            stack->capacity * sizeof(Operation*));
        if (newOperations == NULL)
            return;
        stack->operations = newOperations;
    }

    stack->operations[stack->size++] = copyOperation(operation);
}

Operation* popOperation(OperationStack* stack) {
    if (stack == NULL || stack->size == 0)
        return NULL;

    return stack->operations[--stack->size];
}

void clearOperationStack(OperationStack* stack) {
    if (stack == NULL)
        return;

    for (int i = 0; i < stack->size; i++)
        destroyOperation(stack->operations[i]);

    stack->size = 0;
}