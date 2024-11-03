#include "array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Array* allocate_array_memory() {
    Array* arr = malloc(sizeof(Array));
    if (!arr) {
        fprintf(stderr, "Failed to allocate array memory");
        return NULL;
    }
    return arr;
}

size_t* allocate_shape_memory(size_t ndim) {
    size_t *shape = calloc(ndim, sizeof(size_t));
    if (!shape) {
        fprintf(stderr, "Failed to allocate shape memory");
        return NULL;
    }
    return shape;
}

void* allocate_data_memory(size_t data_size) {
    void *data = calloc(data_size, 1);
    if (!data) {
        fprintf(stderr, "Failed to allocate memory for data");
        return NULL;
    }
    return data;
}

void free_array(Array* arr) {
    if (arr) {
        free(arr->shape);
        free(arr->data);
        free(arr);
    }
}
