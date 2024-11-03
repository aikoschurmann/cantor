#include "array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int validate_indices(Array* arr, size_t* indices) {
    for (size_t i = 0; i < arr->ndim; i++) {
        if (indices[i] >= arr->shape[i]) {
            return 0; // Invalid indices
        }
    }
    return 1; // Valid indices
}

size_t calculate_offset(Array* arr, size_t* indices) {
    size_t offset = 0;
    size_t stride = 1;

    for (size_t i = arr->ndim; i > 0; i--) {
        size_t dim_index = i - 1;
        offset += indices[dim_index] * stride;
        stride *= arr->shape[dim_index];
    }
    return offset;
}

void* get_element(Array* arr, size_t* indices) {
    if (arr->ndim <= 0 || !validate_indices(arr, indices)) {
        return NULL;
    }
    size_t offset = calculate_offset(arr, indices);
    return (char*)arr->data + offset * get_dtype_size(arr->dtype);
}

int set_element(Array* arr, size_t* indices, void* value) {
    void* element = get_element(arr, indices);
    if (element) {
        memcpy(element, value, get_dtype_size(arr->dtype));
        return 1; // Success
    }
    return 0; // Failure
}

