#ifndef ARRAY_H
#define ARRAY_H

// Define this in your project configuration or compilation flags

//suggested to keep on at all times
#define DEBUG_MODE 1  // Set to 1 to enable debug checks, 0 to disable (checks for NULL pointers, etc.)

#define LOG_DEBUG 1  // Set to 1 to enable debug logging, 0 to disable

#include <stddef.h>  // For size_t
#include <stdio.h>   // For standard I/O operations
#include <stdlib.h>  // For memory allocation, NULL
#include <string.h>  // For memcpy, memset, memcmp
#include "utils.h"   // For log_error

// Enum representing the supported data types for elements in the array.
typedef enum {
    TYPE_INT,    // Integer type
    TYPE_FLOAT,  // Floating-point type
    TYPE_DOUBLE   // Double precision floating-point type
} DataType;

// Structure representing an n-dimensional array.
typedef struct {
    size_t* shape;      // Pointer to an array containing the size of each dimension (shape).
    size_t ndim;        // Number of dimensions of the array.
    size_t size;        // Total number of elements in the array (product of shape).
    DataType dtype;     // Data type of the elements (e.g., int, float, double).
    void* data;         // Pointer to the data storage, containing the array elements.
} Array;


// Function prototypes

// Memory allocation functions

// Allocates memory for an Array structure.
// Returns a pointer to the allocated memory, or NULL if the allocation fails.
Array* allocate_array_memory();

// Allocates memory for an array that holds the size of each dimension.
// ndim: The number of dimensions for which to allocate memory.
// Returns a pointer to the allocated memory, or NULL if the allocation fails.
size_t* allocate_shape_memory(size_t ndim);

// Allocates memory for the data block of the array based on the total size required.
// data_size: The size of the data block, calculated as the product of shape elements and size of the data type.
// Returns a pointer to the allocated memory, or NULL if the allocation fails.
void* allocate_data_memory(size_t data_size);


// Array creation and destruction functions

// Creates an Array structure with the specified data type, number of dimensions, shape, and data.
// dtype: The data type of the elements in the array.
// ndim: The number of dimensions of the array.
// shape: Pointer to an array containing the size of each dimension.
// data: Pointer to the initial data to populate the array.
// Returns a pointer to the created Array structure, or NULL if memory allocation fails.
// Note: if data is NULL, the data block will be initialized to zero.
Array* create_array(DataType dtype, size_t ndim, size_t* shape, void* data);

// Frees the memory allocated for an Array structure.
// arr: Pointer to the Array structure to free.
void free_array(Array* arr);


// Shape and index management functions

// Gets the size in bytes of the specified data type.
// dtype: The data type for which to get the size.
// Returns the size of the specified data type in bytes.
size_t get_dtype_size(DataType dtype);

// Validates that the provided indices are within the bounds of the specified array.
// arr: Pointer to the Array structure.
// indices: Pointer to an array containing the indices to validate, with size equal to arr->ndim.
// Returns 1 if the indices are valid; returns 0 otherwise.
int validate_indices(Array* arr, size_t* indices);

// Calculates the offset of the element in the data block based on the provided indices.
// The calculation is done using row-major order.
// arr: Pointer to the Array structure.
// indices: Pointer to an array containing the indices for which to calculate the offset.
// Returns the calculated offset within the data block.
size_t calculate_offset(Array* arr, size_t* indices);

// Broadcasts the shapes of two arrays for element-wise operations.
// shapeA: Pointer to the shape of the first array.
// ndimA: Number of dimensions of the first array.
// shapeB: Pointer to the shape of the second array.
// ndimB: Number of dimensions of the second array.
// result_ndim: Pointer to a variable where the number of dimensions of the broadcasted shape will be stored.
// Returns a pointer to the broadcasted shape array, or NULL if memory allocation fails.
size_t* broadcast_shapes(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB, size_t* result_ndim);

// Maps broadcasted indices back to their original indices in the original array.
// Eg. broadcasted_indices = [1, 2, 0], broadcasted_shape = [2, 3, 1], original_shape = [2, 1, 1]
// The function will return [1, 0, 0] as the original indices.
// broadcasted_shape: Pointer to the shape of the broadcasted array.
// ndim_broadcasted: Number of dimensions of the broadcasted array.
// original_shape: Pointer to the shape of the original array.
// ndim_original: Number of dimensions of the original array.
// broadcasted_indices: Pointer to an array containing the broadcasted indices.
// Returns a pointer to the original indices, or NULL if memory allocation fails.
size_t* map_broadcasted_to_original_indices(size_t* broadcasted_shape, size_t ndim_broadcasted, 
                                            size_t* original_shape, size_t ndim_original, 
                                            size_t* broadcasted_indices);


// Element access functions

// Retrieves a pointer to the element in the array at the specified indices.
// arr: Pointer to the Array structure.
// indices: Pointer to an array containing the indices to access, with size equal to arr->ndim.
// Returns a pointer to the requested element, or NULL if the indices are invalid.
void* get_element(Array* arr, size_t* indices);

// Sets the value of an element in the array at the specified indices.
// arr: Pointer to the Array structure.
// indices: Pointer to an array containing the indices where the value will be set, with size equal to arr->ndim.
// value: Pointer to the value to set at the specified indices.
// Returns 1 if the value is successfully set; returns 0 otherwise.
int set_element(Array* arr, size_t* indices, void* value);


// Array operations

// Broadcasts two arrays and performs the specified element-wise operation between them.
// arr_a: Pointer to the first Array structure.
// arr_b: Pointer to the second Array structure.
// operation_symbol: Character representing the operation to be performed ('+', '-', '*', '/').
// Returns a pointer to the result Array structure, or NULL if the arrays cannot be broadcasted or memory allocation fails.
Array* broadcast_arrays(Array* arr_a, Array* arr_b, char operation_symbol);

// Flattens the input array into a one-dimensional array.
// arr: Pointer to the Array structure to flatten.
// Returns a pointer to the new flattened Array structure, or NULL if memory allocation fails.
Array* flatten_array(Array* arr);

// Compares two arrays for equality without broadcasting.
// arr_a: Pointer to the first Array structure.
// arr_b: Pointer to the second Array structure.
// Returns 1 if the arrays are equal; returns 0 otherwise.
int arrays_are_equal(Array* arr_a, Array* arr_b);


// Compatibility and validation functions

// Checks if the dimensions of two arrays are compatible for broadcasting.
// dimA: Dimension size of the first array.
// dimB: Dimension size of the second array.
// Returns 1 if the dimensions are compatible; returns 0 otherwise.
int are_dims_compatible(size_t dimA, size_t dimB);

// Checks if the shapes of two arrays are equal.
// shapeA: Pointer to the shape of the first array.
// ndimA: Number of dimensions of the first array.
// shapeB: Pointer to the shape of the second array.
// ndimB: Number of dimensions of the second array.
// Returns 1 if the shapes are equal; returns 0 otherwise.
int are_shapes_equal(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB);


// Utility functions

// Prints the shape of the given array to standard output.
// shape: Pointer to an array containing the shape of the array.
// ndim: Number of dimensions of the array.
void print_shape(size_t* shape, size_t ndim);


// Prints the elements of the given array to standard output.
// arr: Pointer to the Array structure to print.
void print_array(Array* arr);

// Retrieves the size of a specific dimension based on an index, returning 1 if the index is out of bounds.
// shape: Pointer to an array containing the shape of the array.
// ndim: Number of dimensions of the array.
// index: Index of the dimension to retrieve the value for.
// dim_value: Pointer to the variable where the dimension value will be stored.
void get_dim_value(size_t* shape, size_t ndim, size_t index, size_t* dim_value);

// Applies the specified operation on two elements and stores the result in the provided result pointer.
// operation_symbol: Character representing the operation to be performed ('+', '-', '*', '/').
// result: Pointer to where the result will be stored.
// a: Pointer to the first operand.
// b: Pointer to the second operand.
// dtype: The data type of the elements being operated on.
void apply_operation(char operation_symbol, void* result, void* a, void* b, DataType dtype);

// Calculate the strides for each dimension based on the shape of the array.
// shape: Pointer to an array containing the shape of the array.
// ndim: Number of dimensions of the array.
// Returns a pointer to the calculated strides array, or NULL if memory allocation fails.
size_t* calculate_strides(const size_t* shape, size_t ndim);


// Check if a permutation is valid for the given number of dimensions.
// perm: Pointer to an array containing the permutation.
// ndim: Number of dimensions for which the permutation is intended.
// Returns 1 if the permutation is valid; returns 0 otherwise.
int is_valid_permutation(const size_t* perm, size_t ndim);

// Reorder the data of an array based on the provided permutation and new shape.
// arr: Pointer to the Array structure to reorder.
// permutation: Pointer to an array containing the permutation.
// new_shape: Pointer to an array containing the new shape after reordering.
// Returns a pointer to the reordered data, or NULL if memory allocation fails.
void* reorder_data(Array* arr, size_t* permutation, size_t* new_shape);

// Transpose the data of an array based on the provided permutation.
// arr: Pointer to the Array structure to transpose.
// permutation: Pointer to an array containing the permutation.
// Returns a pointer to the transposed Array structure, or NULL if memory allocation fails.
Array* transpose(Array* arr, size_t* permutation);

// Sum the elements of an array along the specified axis.
// arr: Pointer to the Array structure to sum.
// axis: The axis along which to sum the elements.
// Returns a pointer to the new Array structure containing the summed elements, or NULL if memory allocation fails.
Array* sum_along_axis(Array* arr, size_t axis);

#endif // ARRAY_H
