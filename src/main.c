#include <stdio.h>
#include "array.h"

int main() {
    // Create a 2x3 array with integer data
    size_t shape1[2] = {2, 3};
    int data[6] = {1, 2, 3, 4, 5, 6};

    // Create a 3x2 array with integer data
    size_t shape2[2] = {1, 3};
    int data2[3] = {1, 2, 3};

    // type, amount of values in shape (ndim), the shape, the data
    Array* arr1 = create_array(TYPE_INT, 2, shape1, data);
    Array* arr2 = create_array(TYPE_INT, 2, shape2, data2);
    if (!arr1 || !arr2) {
        fprintf(stderr, "Failed to create array.\n");
        return 1;
    }
    // Print array shape and contents
    printf("Array shape: ");
    print_shape(arr1->shape, arr1->ndim);
    printf("Array contents:\n");
    print_array(arr1);

    // Demonstrate broadcasting arrays
    Array* result = broadcast_arrays(arr1, arr2, '+');
    printf("Broadcasted array:\n");
    print_array(result);

    // transpose the array
    size_t permutation[2] = {1,0};
    Array* transposed = transpose(result, permutation);
    printf("Transposed array:\n");
    print_array(transposed);

    free_array(arr1);
    free_array(arr2);
    return 0;
}