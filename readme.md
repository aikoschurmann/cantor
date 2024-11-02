# C Array Library

## Overview

This C library provides a dynamic array implementation with support for multidimensional arrays, broadcasting, and basic operations. It is designed to facilitate numerical computations similar to NumPy in Python, focusing on efficient memory management and data manipulation.

## Features

- **Dynamic Array Creation**: Allocate memory for arrays of various data types and dimensions.
- **Element Access and Modification**: Get and set elements in the array using indices.
- **Broadcasting**: Support for broadcasting operations between arrays of different shapes.
- **Memory Management**: Functions for allocating and freeing memory for arrays and their data.
- **Shape Handling**: Functions to handle array shapes and dimensions.

## Data Types

The library supports the following data types:

- `TYPE_INT`: Integer values
- `TYPE_FLOAT`: Floating-point values
- `TYPE_DOUBLE`: Double precision floating-point values

## Functions

### Array Management

- **`Array* create_array(DataType dtype, size_t ndim, size_t *shape, void *data)`**: Creates a new array with the specified data type, number of dimensions, shape, and initial data.
- **`void free_array(Array* arr)`**: Frees the memory allocated for an array.

### Memory Allocation

- **`Array* allocate_array_memory()`**: Allocates memory for an `Array` structure.
- **`size_t* allocate_shape_memory(size_t ndim)`**: Allocates memory for an array shape.
- **`void* allocate_data_memory(size_t data_size)`**: Allocates memory for array data.

### Element Access

- **`void* get_element(Array* arr, size_t* indices)`**: Retrieves an element from the array at the specified indices.
- **`int set_element(Array* arr, size_t* indices, void* value)`**: Sets the value of an element in the array at the specified indices.

### Broadcasting

- **`Array* broadcast_arrays(Array* arr_a, Array* arr_b, char operation_symbol)`**: Performs broadcasting between two arrays based on the specified operation.
- **`size_t* broadcast_shapes(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB, size_t* result_ndim)`**: Calculates the resulting shape after broadcasting two shapes.

### Utility Functions

- **`void print_shape(size_t* shape, size_t ndim)`**: Prints the shape of the array.
- **`int arrays_are_equal(Array* arr_a, Array* arr_b)`**: Compares two arrays for equality.

## Usage Example

```c
#include "array.h"

int main() {
    // Define the shape of the array (2 rows, 3 columns)
    size_t shape[2] = {2, 3};

    // Define the source data array
    int data[6] = {1, 2, 3, 4, 5, 6}; // Source data array

    // Create a 2D integer array directly with data
    Array* arr = create_array(TYPE_INT, 2, shape, data);
    
    // Check if array creation was successful
    if (!arr) {
        fprintf(stderr, "Failed to create array.\n");
        return 1;
    }

    // Print the shape of the array
    printf("Array shape: ");
    print_shape(arr->shape, arr->ndim);

    // Print the array contents after initialization
    printf("Array contents after initialization:\n");
    for (size_t row = 0; row < shape[0]; row++) {
        for (size_t col = 0; col < shape[1]; col++) {
            int* value = (int*)get_element(arr, (size_t[]){row, col});
            printf("%d ", *value);
        }
        printf("\n");
    }

    // display broadcasting 
    size_t shapeA[2] = {1, 4};
    size_t shapeB[2] = {4, 1};
    size_t result_ndim;
    size_t* result_shape = broadcast_shapes(shapeA, 2, shapeB, 2, &result_ndim);
    printf("Broadcasted shape: ");
    print_shape(result_shape, result_ndim);

    free_array(arr);
    free(result_shape);
    return 0;
}
```

## Compilation

```bash
make run
```

