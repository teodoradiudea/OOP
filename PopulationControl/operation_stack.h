#pragma once
#include "country.h"

typedef struct {
    Country* country;
    char* type;
} Operation;

typedef struct {
    Operation** operations;
    int size;
    int capacity;
} OperationStack;

// Creates a new operation
Operation* createOperation(Country country, char* type);

// Destroys an operation and deallocates its memory
void destroyOperation(Operation* operation);

// Creates a deep copy of an operation
Operation* copyOperation(Operation* operation);

// Gets the product from an operation
Country* getOperationProduct(Operation* operation);

// Gets the type of an operation
char* getOperationType(Operation* operation);

// Creates a new operation stack
OperationStack* createOperationStack();

// Destroys an operation stack and deallocates its memory
void destroyOperationStack(OperationStack* stack);

// Pushes an operation onto the stack
void pushOperation(OperationStack* stack, Operation* operation);

// Pops an operation from the stack
Operation* popOperation(OperationStack* stack);

// Clears all operations from the stack
void clearOperationStack(OperationStack* stack);
