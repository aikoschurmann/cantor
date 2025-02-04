#include "array.h"
#include <stdlib.h>
#include <stdio.h>

size_t* broadcast_shapes(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB, size_t* result_ndim) {
    #if DEBUG_MODE
        if (!shapeA || !shapeB) {
            log_error("One of the inputs is NULL");
            return NULL;
        }

        if (ndimA == 0 || ndimB == 0) {
            log_error("Number of dimensions is 0");
            return NULL; 
        }
    #endif

    size_t max_rank = (ndimA > ndimB) ? ndimA : ndimB;
    size_t* result_shape = allocate_shape_memory(max_rank);
    if (!result_shape) {
        return NULL;
    }

    for (size_t i = 0; i < max_rank; i++) {
        size_t dimA, dimB;
        get_dim_value(shapeA, ndimA, i, &dimA);
        get_dim_value(shapeB, ndimB, i, &dimB);

        if (!are_dims_compatible(dimA, dimB)) {
            free(result_shape);
            log_error("Shapes are not broadcastable");
            return NULL; 
        }

        result_shape[max_rank - 1 - i] = (dimA > dimB) ? dimA : dimB; 
    }

    *result_ndim = max_rank; 
    return result_shape; 
}

void get_dim_value(size_t* shape, size_t ndim, size_t index, size_t* dim_value) {
    if (index < ndim) {
        *dim_value = shape[ndim - 1 - index]; // Access from the end
    } else {
        *dim_value = 1; // Out of bounds, treat as 1
    }
}

int are_dims_compatible(size_t dimA, size_t dimB) {
    return (dimA == dimB) || (dimA == 1) || (dimB == 1);
}

size_t* map_broadcasted_to_original_indices(
    size_t* broadcasted_shape, size_t ndim_broadcasted,
    size_t* original_shape, size_t ndim_original,
    size_t* broadcasted_indices
) {
    //check if any of the inputs are NULL
    #if DEBUG_MODE
        if (!broadcasted_shape || !original_shape || !broadcasted_indices) {
            log_error("One of the inputs is NULL");
            return NULL;  // One of the inputs is NULL
        }
    #endif

    // Allocate memory for original indices
    size_t* original_indices = allocate_shape_memory(ndim_original);
    if (!original_indices) {
        return NULL;  // Memory allocation failed
    }

    size_t shape_offset = ndim_broadcasted - ndim_original;

    // Check for valid shape offset
    if (shape_offset < 0) {
        free(original_indices);
        log_error("Invalid shape offset");
        return NULL;  // Invalid offset
    }

    for (size_t i = 0; i < ndim_original; i++) {
        // Check if we're accessing valid indices
        if (i + shape_offset >= ndim_broadcasted) {
            free(original_indices);
            log_error("Accessing out of bounds in broadcasted_shape");
            return NULL;  // Accessing out of bounds in broadcasted_shape
        }
        if (i >= ndim_original) {
            free(original_indices);
            log_error("Accessing out of bounds in original_shape");
            return NULL;  // Accessing out of bounds in original_shape
        }

        size_t broadcast_dim = broadcasted_shape[i + shape_offset];
        size_t original_dim = original_shape[i];
        size_t broadcast_index = broadcasted_indices[i + shape_offset];

        // Ensure the index is valid for the broadcasted shape
        if (broadcast_index >= ndim_broadcasted) {
            free(original_indices);
            log_error("Accessing out of bounds in broadcasted_indices");
            return NULL;  // Accessing out of bounds in broadcasted_indices
        }
        
        if (original_dim == broadcast_dim) {
            original_indices[i] = broadcast_index;
        } else if (original_dim == 1) {
            original_indices[i] = 0;  // Use the first index
        } else {
            free(original_indices);  // Free memory to prevent a leak
            log_error("Shapes are incompatible");
            return NULL;  // Shapes are incompatible
        }
    }

    return original_indices;
}

Array* broadcast_arrays_fast(Array* arr_a, Array* arr_b, char operation_symbol) {
    size_t data_size = arr_a->size * get_dtype_size(arr_a->dtype);
    Array* result = create_array(arr_a->dtype, arr_a->ndim, arr_a->shape, NULL);
        
    if (!result) {
        return NULL;
    }

    for (size_t i = 0; i < arr_a->size; i++) {
        void* elem_a = (char*)arr_a->data + i * get_dtype_size(arr_a->dtype);
        void* elem_b = (char*)arr_b->data + i * get_dtype_size(arr_b->dtype);
        void* elem_res = (char*)result->data + i * get_dtype_size(result->dtype);

        apply_operation(operation_symbol, elem_res, elem_a, elem_b, arr_a->dtype);
    }
    return result;
}

Array* broadcast_arrays(Array* arr_a, Array* arr_b, char operation_symbol) {
    #if DEBUG_MODE
        if (!arr_a || !arr_b) {
            log_error("One of the arrays is NULL");
            return NULL;
        }

        if (arr_a->ndim == 0 || arr_b->ndim == 0) {
            log_error("Empty array detected. Broadcasting is not possible.");
            return NULL;
        }

        if (arr_a->dtype != arr_b->dtype) {
            log_error("Data types are not equal");
            return NULL;
        }
    #endif
    
    if (are_shapes_equal(arr_a->shape, arr_a->ndim, arr_b->shape, arr_b->ndim)) {
        return broadcast_arrays_fast(arr_a, arr_b, operation_symbol);
    }

    size_t result_ndim;
    size_t* result_shape = broadcast_shapes(arr_a->shape, arr_a->ndim, arr_b->shape, arr_b->ndim, &result_ndim);
    
    if (!result_shape) {
        return NULL;
    }

    Array* result = create_array(arr_a->dtype, result_ndim, result_shape, NULL);
    if (!result) {
        return NULL;
    }

    size_t* indices = allocate_shape_memory(result_ndim);
    if (!indices) {
        free_array(result);
        return NULL;
    }

    for (size_t i = 0; i < result->size; i++) {
        size_t remaining_index = i;
        for (size_t j = result_ndim; j-- > 0;) {
            indices[j] = remaining_index % result->shape[j];
            remaining_index /= result->shape[j];
        }

        size_t* indices_a = map_broadcasted_to_original_indices(result->shape, result_ndim, arr_a->shape, arr_a->ndim, indices);
        size_t* indices_b = map_broadcasted_to_original_indices(result->shape, result_ndim, arr_b->shape, arr_b->ndim, indices);

    
        if (!indices_a || !indices_b) {
            free_array(result);
            free(indices);
            free(indices_a);
            free(indices_b);
            return NULL;
        }

        void* elem_a = get_element(arr_a, indices_a);
        void* elem_b = get_element(arr_b, indices_b);
        void* elem_res = get_element(result, indices);

        apply_operation(operation_symbol, elem_res, elem_a, elem_b, arr_a->dtype);

        free(indices_a);
        free(indices_b);
    }

    free(indices);
    return result;
}