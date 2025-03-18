#include "array.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Calculate the broadcasted shape from two input shapes.
 *
 * The shapes are processed from the rightmost (least significant) dimension.
 * For each dimension, the broadcasted size is the maximum of the two dimensions,
 * as long as they are compatible (i.e. equal or one of them is 1).
 *
 * @param shapeA First shape array.
 * @param ndimA Number of dimensions of shapeA.
 * @param shapeB Second shape array.
 * @param ndimB Number of dimensions of shapeB.
 * @param result_ndim Pointer to store the resulting number of dimensions.
 * @return The broadcasted shape or NULL if the shapes are not compatible.
 */
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

    // Loop over dimensions from the rightmost side (least significant dimension)
    for (size_t i = 0; i < max_rank; i++) {
        size_t dimA, dimB;
        get_dim_value(shapeA, ndimA, i, &dimA);
        get_dim_value(shapeB, ndimB, i, &dimB);

        if (!are_dims_compatible(dimA, dimB)) {
            free(result_shape);
            log_error("Shapes are not broadcastable");
            return NULL; 
        }
        // The broadcasted dimension is the maximum of the two (since one may be 1)
        result_shape[max_rank - 1 - i] = (dimA > dimB) ? dimA : dimB; 
    }

    *result_ndim = max_rank; 
    return result_shape; 
}

/**
 * Retrieve a dimension value from a shape array.
 *
 * The function returns dimensions in reverse order (from the end)
 * so that the rightmost dimension is processed first.
 *
 * @param shape The shape array.
 * @param ndim Number of dimensions.
 * @param index The index counting from the right.
 * @param dim_value Pointer to store the dimension value.
 */
void get_dim_value(size_t* shape, size_t ndim, size_t index, size_t* dim_value) {
    if (index < ndim) {
        *dim_value = shape[ndim - 1 - index]; // Access from the end
    } else {
        *dim_value = 1; // Out of bounds, treat as 1 (for broadcasting)
    }
}

/**
 * Check if two dimensions are compatible for broadcasting.
 *
 * Dimensions are compatible if they are equal or if one of them is 1.
 *
 * @param dimA Dimension from the first shape.
 * @param dimB Dimension from the second shape.
 * @return Non-zero if compatible, zero otherwise.
 */
int are_dims_compatible(size_t dimA, size_t dimB) {
    return (dimA == dimB) || (dimA == 1) || (dimB == 1);
}

/**
 * Map indices from a broadcasted shape back to the corresponding indices
 * in the original array.
 *
 * Given the broadcasted indices (for the broadcasted shape) and the original shape,
 * this function calculates the indices in the original array. For dimensions where the
 * original array has size 1, the index is set to 0.
 *
 * @param broadcasted_shape The shape after broadcasting.
 * @param ndim_broadcasted Number of dimensions in the broadcasted shape.
 * @param original_shape The original shape of the array.
 * @param ndim_original Number of dimensions in the original shape.
 * @param broadcasted_indices The indices in the broadcasted array.
 * @return A newly allocated array of indices corresponding to the original array.
 */
size_t* map_broadcasted_to_original_indices(
    size_t* broadcasted_shape, size_t ndim_broadcasted,
    size_t* original_shape, size_t ndim_original,
    size_t* broadcasted_indices
) {
    #if DEBUG_MODE
        if (!broadcasted_shape || !original_shape || !broadcasted_indices) {
            log_error("One of the inputs is NULL");
            return NULL;
        }
    #endif

    size_t* original_indices = allocate_shape_memory(ndim_original);
    if (!original_indices) {
        return NULL;
    }

    size_t shape_offset = ndim_broadcasted - ndim_original;
    // Loop over each dimension of the original shape
    for (size_t i = 0; i < ndim_original; i++) {
        // Access the corresponding dimension in the broadcasted shape
        size_t broadcast_dim = broadcasted_shape[i + shape_offset];
        size_t original_dim = original_shape[i];
        size_t broadcast_index = broadcasted_indices[i + shape_offset];

        // Validate that the broadcast index is within the bounds of the broadcasted dimension
        if (broadcast_index >= broadcast_dim) {
            free(original_indices);
            log_error("Broadcast index out of bounds for the current dimension");
            return NULL;
        }
        
        // If the original dimension matches the broadcasted dimension, use the index.
        // If the original dimension is 1, it is broadcasted so use index 0.
        if (original_dim == broadcast_dim) {
            original_indices[i] = broadcast_index;
        } else if (original_dim == 1) {
            original_indices[i] = 0;
        } else {
            free(original_indices);
            log_error("Shapes are incompatible");
            return NULL;
        }
    }

    return original_indices;
}

/**
 * Fast broadcasting for arrays with identical shapes.
 *
 * When two arrays have the same shape, no index remapping is needed.
 * This function simply applies the operation element-wise.
 *
 * @param arr_a First input array.
 * @param arr_b Second input array.
 * @param operation_symbol The operation to apply.
 * @return A new array containing the results.
 */
Array* broadcast_arrays_fast(Array* arr_a, Array* arr_b, char operation_symbol) {
    size_t data_size = arr_a->size * get_dtype_size(arr_a->dtype);
    Array* result = create_array(arr_a->dtype, arr_a->ndim, arr_a->shape, NULL);
    if (!result) {
        return NULL;
    }
   


    // Loop through each element in the arrays
    for (size_t i = 0; i < arr_a->size; i++) {
        void* elem_a = (char*)arr_a->data + i * get_dtype_size(arr_a->dtype);
        void* elem_b = (char*)arr_b->data + i * get_dtype_size(arr_b->dtype);
        void* elem_res = (char*)result->data + i * get_dtype_size(result->dtype);

        apply_operation(operation_symbol, elem_res, elem_a, elem_b, arr_a->dtype);
    }
    return result;
}

/**
 * Broadcast two arrays and apply an operation on each element.
 *
 * This function computes the broadcasted shape of the two arrays, remaps
 * the indices appropriately, and then applies the operation element-wise.
 *
 * @param arr_a First input array.
 * @param arr_b Second input array.
 * @param operation_symbol The operation to apply.
 * @return A new array containing the results or NULL on error.
 */
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
    
    // If the shapes are identical, use the fast broadcast method
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
        free(result_shape);
        return NULL;
    }
    free(result_shape); // Assume create_array makes a copy if needed

    // Allocate temporary memory to hold the broadcasted indices
    size_t* indices = allocate_shape_memory(result_ndim);
    if (!indices) {
        free_array(result);
        return NULL;
    }

    // Iterate over every element in the result array
    for (size_t i = 0; i < result->size; i++) {
        size_t remaining_index = i;
        // Compute multi-dimensional indices for the result array
        for (size_t j = result_ndim; j-- > 0;) {
            indices[j] = remaining_index % result->shape[j];
            remaining_index /= result->shape[j];
        }

        // Map broadcasted indices to indices in the original arrays
        size_t* indices_a = map_broadcasted_to_original_indices(result->shape, result_ndim, arr_a->shape, arr_a->ndim, indices);
        size_t* indices_b = map_broadcasted_to_original_indices(result->shape, result_ndim, arr_b->shape, arr_b->ndim, indices);

        if (!indices_a || !indices_b) {
            free_array(result);
            free(indices);
            free(indices_a);
            free(indices_b);
            return NULL;
        }

        // Retrieve element pointers using the computed indices
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
