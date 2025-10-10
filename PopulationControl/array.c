#include "array.h"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

DynamicArray* createDynamicArray(int capacity) {
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!arr) return NULL;

    arr->size = 0;
    arr->capacity = capacity;
    arr->data = (Country*)malloc(sizeof(Country) * arr->capacity);
    if (!arr->data) {
        free(arr);
        return NULL;
    }
    return arr;
}

void DestroyDynamicArray(DynamicArray* arr) {
    if (!arr) return;
    for (int i = 0; i < arr->size; i++) {
        free(arr->data[i].name);
        free(arr->data[i].continent);
    }
   
    free(arr->data);
    free(arr);
}

void resize(DynamicArray* arr) {
    if (!arr) return;

    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        Country* newData = (Country*)realloc(arr->data, sizeof(Country) * arr->capacity);
        if (newData) {
            arr->data = newData;
        }
    }
}