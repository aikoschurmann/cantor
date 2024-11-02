#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>  // For size_t
#include <stdio.h>   // For fprintf
#include <stdlib.h>  // For malloc, free
#include <string.h>  // For memcpy, memset

typedef enum {
    TYPE_INT = 0,
    TYPE_FLOAT = 1,
    TYPE_DOUBLE = 2,
    // Add other types as needed
} DataType;

typedef struct {
    size_t ndim;
    size_t* shape;
    DataType dtype;
    size_t size;
    void* data;
} Array;

typedef struct {
    size_t total_size;
    size_t* shape;
    size_t ndim;
} SizeAndShape;

void print_shape(size_t* shape, size_t ndim);
size_t get_dtype_size(DataType dtype);
Array* create_array(DataType dtype, size_t ndim, size_t *shape, void *data);
void free_array(Array* arr);
void* get_element(Array* arr, size_t* indices);
int set_element(Array* arr, size_t* indices, void* value);
size_t* broadcast_shapes(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB, size_t* result_ndim);
Array* broadcast_arrays(Array* arr_a, Array* arr_b, char operation_symbol);
int are_shapes_equal(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB);
Array* flatten_array(Array* arr);
int arrays_are_equal(Array* arr_a, Array* arr_b);
size_t* map_broadcasted_to_original_indices(size_t* broadcasted_shape, size_t ndim_broadcasted, size_t* original_shape, size_t ndim_original, size_t* broadcasted_indices);

#endif // ARRAY_H
