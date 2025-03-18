# C NDArray Library

## Overview

This C library provides a dynamic ndarray implementation with support for multidimensional arrays, broadcasting, and basic operations. It is designed to facilitate numerical computations similar to NumPy in Python, focusing on efficient memory management and data manipulation.

## Features

- **Dynamic Array Creation**: Allocate memory for arrays of various data types and dimensions.
- **Element Access and Modification**: Get and set elements in the array using indices.
- **Broadcasting**: Support for broadcasting operations between arrays of different shapes.
- **Linear Algebra Opperations**: Support for linear algebra operations such as transposing using permutations

## Data Types

The library supports the following data types:

- `TYPE_INT`: Integer values
- `TYPE_FLOAT`: Floating-point values
- `TYPE_DOUBLE`: Double precision floating-point values

## Functions

### Array Management

- **`Array* create_array(DataType dtype, size_t ndim, size_t *shape, void *data)`**: Creates a new array with the specified data type, number of dimensions, shape, and initial data.
- **`void free_array(Array* arr)`**: Frees the memory allocated for an array.


### Element Access

- **`void* get_element(Array* arr, size_t* indices)`**: Retrieves an element from the array at the specified indices.
- **`int set_element(Array* arr, size_t* indices, void* value)`**: Sets the value of an element in the array at the specified indices.

### Broadcasting

- **`Array* broadcast_arrays(Array* arr_a, Array* arr_b, char operation_symbol)`**: Performs broadcasting between two arrays based on the specified operation.
- **`size_t* broadcast_shapes(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB, size_t* result_ndim)`**: Calculates the resulting shape after broadcasting two shapes.

### Utility Functions

- **`void print_shape(size_t* shape, size_t ndim)`**: Prints the shape of the array.
- **`int arrays_are_equal(Array* arr_a, Array* arr_b)`**: Compares two arrays for equality.

### Linear Algebra

- **`Array* transpose(Array* arr, size_t* permutation);`**: Transposes the array given a permutation.

## Usage Example

```c
    #include <stdio.h>
    #include "array.h"

    int main() {
        // Sales data: Stores x Products (2 stores, 3 products)
        size_t sales_shape[2] = {2, 3};
        int sales_data[6] = {10, 15, 20,  // Store 1 (products 1, 2, 3)
                             5,  8,  12}; // Store 2 (products 1, 2, 3)
        Array* sales = create_array(TYPE_INT, 2, sales_shape, sales_data);

        // Price per unit (1D array - broadcastable)
        size_t price_shape[1] = {3};
        int price_data[3] = {2, 3, 4}; // Prices: Product1 = $2, Product2 = $3, Product3 = $4
        Array* prices = create_array(TYPE_INT, 1, price_shape, price_data);

        // Check if arrays were created successfully
        if (!sales || !prices) {
            fprintf(stderr, "Failed to create arrays.\n");
            return 1;
        }

        // Print sales data
        printf("Sales Data (Units Sold per Store per Product):\n");
        print_array(sales);
        printf("\n");

        // Print price per product
        printf("Price per Product:\n");
        print_array(prices);
        printf("\n");

        // Compute revenue per product (Broadcasting: sales * prices)
        Array* revenue = broadcast_arrays(sales, prices, '*');
        printf("Revenue per Store per Product:\n");
        print_array(revenue);
        printf("\n");

        // Compute total revenue per store
        Array* total_revenue = sum_along_axis(revenue, 1);
        printf("Total Revenue per Store:\n");
        print_array(total_revenue);

        // Free memory
        free_array(sales);
        free_array(prices);
        free_array(revenue);
        free_array(total_revenue);

        return 0;
    }
    
```

## Compilation

```bash
make run
```

