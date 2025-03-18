#include "array.h"
#include <stdio.h>

size_t get_dtype_size(DataType dtype) {
    switch (dtype) {
        case TYPE_INT:
            return sizeof(int);
        case TYPE_FLOAT:
            return sizeof(float);
        case TYPE_DOUBLE:
            return sizeof(double);
        default:
            return 0;
    }
}



void print_shape(size_t* shape, size_t ndim) {
    if (!shape) {
        printf("NULL\n");
        return;
    }

    printf("(");
    if (ndim > 0) {
        printf("%zu", shape[0]);  // Print the first element
        for (size_t i = 1; i < ndim; i++) {
            printf(", %zu", shape[i]);  // Print remaining elements with a leading comma
        }
    }
    printf(")\n");
}



void print_array(Array* arr) {
    if (!arr) {
        printf("\033[31mArray: NULL\033[0m\n");  // Red for NULL
        return;
    }

    size_t ndim = arr->ndim;
    size_t *shape = arr->shape;

    for (size_t i = 0; i < arr->size; i++) {
        size_t indices[ndim];
        size_t remaining_index = i;

        // Compute the multi-dimensional indices
        for (size_t j = ndim; j-- > 0;) {
            indices[j] = remaining_index % shape[j];
            remaining_index /= shape[j];
        }

        // Fetch and print the element
        void *element = get_element(arr, indices);
        if (element) {
            printf("%d", *(int*)element);
        } else {
            printf("NULL");
        }

        // Determine when to add newlines based on the last dimension
        if ((i + 1) % shape[ndim - 1] == 0) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
}


int are_shapes_equal(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB) {
    if (ndimA != ndimB) {
        return 0; 
    }

    for (size_t i = 0; i < ndimA; i++) {
        if (shapeA[i] != shapeB[i]) {
            return 0; 
        }
    }

    return 1; 
}

int arrays_are_equal(Array* arr_a, Array* arr_b) {
    #if DEBUG_MODE
        if (!arr_a || !arr_b) {
            log_error("One of the arrays is NULL");
            return 0;
        }
    #endif

    if (arr_a->dtype != arr_b->dtype) {
        log_error("Data types are not equal");
        return 0;
    }

    if (arr_a->ndim != arr_b->ndim) {
        log_error("Number of dimensions are not equal");
        return 0;
    }

    for (size_t i = 0; i < arr_a->ndim; i++) {
        if (arr_a->shape[i] != arr_b->shape[i]) {
            log_error("Shapes are not equal");
            return 0;
        }
    }

    for (size_t i = 0; i < arr_a->size; i++) {
        void* elem_a = (char*)arr_a->data + i * get_dtype_size(arr_a->dtype);
        void* elem_b = (char*)arr_b->data + i * get_dtype_size(arr_b->dtype);

        if (memcmp(elem_a, elem_b, get_dtype_size(arr_a->dtype)) != 0) {
            log_error("Elements are not equal");
            return 0;
        }
    }

    return 1;
}