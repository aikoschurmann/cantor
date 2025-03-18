# C NDArray Library

## Overview

This C library provides a dynamic ndarray implementation with support for multidimensional arrays, broadcasting, and basic operations. It is designed to facilitate numerical computations similar to NumPy in Python, focusing on efficient memory management and data manipulation.

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
```

## Compilation

```bash
make run
```

