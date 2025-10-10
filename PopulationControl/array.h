#pragma once
#include "country.h"

typedef struct dynamic_array {
    Country* data;
    int size;
    int capacity;
} DynamicArray;

DynamicArray* createDynamicArray(int capacity);
void DestroyDynamicArray(DynamicArray* arr);
void resize(DynamicArray* arr);
