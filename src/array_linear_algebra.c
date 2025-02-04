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


void* reorder_data(Array* arr, size_t* permutation, size_t* new_shape) {

    // Allocate memory for the reordered data
    size_t dsize = get_dtype_size(arr->dtype);
    void* reordered_data = allocate_data_memory(arr->size * dsize);
    if (!reordered_data) {
        log_error("Failed to allocate memory for reordered data");
        return NULL;
    }

    // Calculate original strides
    size_t* original_strides = calculate_strides(arr->shape, arr->ndim);
    

    for (size_t i = 0; i < arr->ndim; i++) {
        new_shape[i] = original_strides[permutation[i]];
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

        // Calculate the transposed index coordinates
        size_t transposed_index = 0;
        for (size_t j = 0; j < arr->ndim; j++) {
            transposed_index += original_index[permutation[j]] * new_shape[j];
        }

        // Copy the element to the new position in the reordered data
        memcpy((char*)reordered_data + transposed_index * dsize, (char*)arr->data + i * dsize, dsize);
    }

    // Free temporary allocations
    free(original_strides);
    free(new_shape);

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

    size_t* new_shape = allocate_shape_memory(arr->ndim);
    if (!new_shape) {
        return NULL;
    }

    // Permute the shape based on the given permutation
    for (size_t i = 0; i < arr->ndim; i++) {
        new_shape[i] = arr->shape[permutation[i]];
    }

    Array* result = create_array(arr->dtype, arr->ndim, new_shape, arr->data);
    if (!result) {
        free(new_shape);
        return NULL;
    }

    if (arr->ndim > 1) {
        result->data = reorder_data(arr, permutation, new_shape);
        if (!result->data) {
            free_array(result);
            return NULL;
        }
    } else {
        return result;
    }
    return result;
}

int main(int argc, char const *argv[])
{
    // Test case 1: Transpose a 2x3 array
    size_t shape2d[] = {2, 3};
    int data2d[] = {1, 2, 3, 4, 5, 6};
    Array* arr2d = create_array(TYPE_INT, 2, shape2d, data2d);
    Array* result2d = transpose(arr2d, (size_t[]){1, 0});
    print_array(result2d);
    print_shape(result2d->shape, result2d->ndim);

    // Test case 2: Transpose a 2x3x4 array
    size_t shape3d[] = {2, 3, 4};
    int data3d[] = {
        1, 2, 3, 4, 
        5, 6, 7, 8,
        9, 10, 11, 12,

        13, 14, 15, 16,
        17, 18, 19, 20,
        21, 22, 23, 24
    };
    Array* arr3d = create_array(TYPE_INT, 3, shape3d, data3d);

    // Applying a transpose with the permutation {2, 1, 0}
    // This should swap the dimensions, making it a 4x3x2 array
    Array* result3d = transpose(arr3d, (size_t[]){2, 1, 0});
    print_array(result3d);
    print_shape(result3d->shape, result3d->ndim);

    return 0;
}




