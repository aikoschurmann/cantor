#include "array.h"


size_t* calculate_strides(const size_t* shape, size_t ndim) {
    size_t* strides = malloc(ndim * sizeof(size_t));
    if (!strides) {
        log_error("Failed to allocate memory for strides");
        return NULL;
    }

    strides[ndim - 1] = 1;
    for (int i = ndim - 2; i >= 0; i--) {
        strides[i] = strides[i + 1] * shape[i + 1];
    }

    return strides;
}


int is_valid_permutation(const size_t* perm, size_t ndim) {
    int seen[ndim];
    for (size_t i = 0; i < ndim; i++) {
        seen[i] = 0;
    }

    for (size_t i = 0; i < ndim; i++) {
        if (perm[i] >= ndim || seen[perm[i]]) {
            return 0; // Out of bounds or duplicate index
        }
        seen[perm[i]] = 1;
    }
    return 1;
}



void* reorder_data(Array* arr, size_t* permutation, size_t* new_shape) {
    size_t dsize = get_dtype_size(arr->dtype);
    void* reordered_data = allocate_data_memory(arr->size * dsize);
    if (!reordered_data) {
        log_error("Failed to allocate memory for reordered data");
        return NULL;
    }

    // Compute new strides based on the transposed shape
    size_t* new_strides = calculate_strides(new_shape, arr->ndim);
    if (!new_strides) {
        return NULL;
    }

    // Map each element to the new position
    for (size_t i = 0; i < arr->size; i++) {
        // Calculate the original index coordinates
        size_t original_index[arr->ndim];
        size_t idx = i;
        for (int j = arr->ndim - 1; j >= 0; j--) {
            original_index[j] = idx % arr->shape[j];
            idx /= arr->shape[j];
        }

        // Calculate the transposed linear index using new strides
        size_t transposed_index = 0;
        for (size_t j = 0; j < arr->ndim; j++) {
            transposed_index += original_index[permutation[j]] * new_strides[j];
        }

        // Copy the element to the new position in the reordered data
        memcpy((char*)reordered_data + transposed_index * dsize,
               (char*)arr->data + i * dsize, dsize);
    }

    free(new_strides);
    return reordered_data;
}

Array* transpose(Array* arr, size_t* permutation) {
    #if DEBUG_MODE
        if (!arr) {
            log_error("Array is NULL");
            return NULL;
        }
        if (!permutation) {
            log_error("Permutation is NULL");
            return NULL;
        }
        if (arr->ndim < 1) {
            log_error("Array has less than 1 dimension");
            return NULL;
        }
    #endif

    // Allocate memory for the new shape (dimensions after transpose)
    size_t* new_shape = allocate_shape_memory(arr->ndim);
    if (!new_shape) {
        return NULL;
    }

    if (!is_valid_permutation(permutation, arr->ndim)) {
        log_error("Invalid permutation");
        return NULL;
    }

    // Permute the shape based on the given permutation
    for (size_t i = 0; i < arr->ndim; i++) {
        new_shape[i] = arr->shape[permutation[i]];
    }

    // Create the result array using the new shape
    Array* result = create_array(arr->dtype, arr->ndim, new_shape, arr->data);
    if (!result) {
        free(new_shape);
        return NULL;
    }

    // Only reorder data if more than one dimension exists
    if (arr->ndim > 1) {
        result->data = reorder_data(arr, permutation, new_shape);
        if (!result->data) {
            free_array(result);
            return NULL;
        }
    }
    return result;
}










