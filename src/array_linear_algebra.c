#include "array.h"


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

    if (arr->ndim == 1) {
        return result;
    }

}

//int main(int argc, char const *argv[])
//{
//    // Test case 1: Transpose a 2x3 array
//    size_t shape[] = {6};
//    int data[] = {1, 2, 3, 4, 5, 6};
//    Array* arr = create_array(TYPE_INT, 1, shape, data);
//    Array* result = transpose(arr, NULL);
//    //print_array(result);
//    return 0;
//}



