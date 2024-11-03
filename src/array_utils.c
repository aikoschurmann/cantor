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
    if (shape == NULL) {
        printf("\033[31mShape: NULL\033[0m\n");
        return;
    }
    
    printf("\033[32mShape:\033[0m (");
    for (size_t i = 0; i < ndim; i++) {
        printf("%zu", shape[i]);
        if (i < ndim - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

void print_array(Array* arr) {
    if (arr == NULL) {
        printf("\033[31mArray: NULL\033[0m\n");  // Red for NULL
        return;
    }

    for (size_t i = 0; i < arr->size; i++) {
        size_t indices[arr->ndim];
        size_t remaining_index = i;
        for (size_t j = arr->ndim; j-- > 0;) {
            indices[j] = remaining_index % arr->shape[j];
            remaining_index /= arr->shape[j];
        }

        void* element = get_element(arr, indices);
        if (element) {
            printf("%d", *(int*)element);
        } else {
            printf("NULL");
        }

        if (i < arr->size - 1) {
            printf(", ");
        }
    }
    printf("\n");
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
    if (!arr_a || !arr_b) {
        fprintf(stderr, "One of the arrays is NULL\n");
        return 0;
    }

    if (arr_a->dtype != arr_b->dtype) {
        fprintf(stderr, "Data types are not equal\n");
        return 0;
    }

    if (arr_a->ndim != arr_b->ndim) {
        fprintf(stderr, "Number of dimensions are not equal\n");
        return 0;
    }

    for (size_t i = 0; i < arr_a->ndim; i++) {
        if (arr_a->shape[i] != arr_b->shape[i]) {
            fprintf(stderr, "Shapes are not equal\n");
            return 0;
        }
    }

    for (size_t i = 0; i < arr_a->size; i++) {
        void* elem_a = (char*)arr_a->data + i * get_dtype_size(arr_a->dtype);
        void* elem_b = (char*)arr_b->data + i * get_dtype_size(arr_b->dtype);

        if (memcmp(elem_a, elem_b, get_dtype_size(arr_a->dtype)) != 0) {
            fprintf(stderr, "Elements are not equal\n");
            return 0;
        }
    }

    return 1;
}