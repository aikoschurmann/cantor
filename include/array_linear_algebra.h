#ifndef ARRAY_LINEAR_ALGEBRA_H
#define ARRAY_LINEAR_ALGEBRA_H

#include "array.h"

// Function prototypes
size_t* calculate_strides(const size_t* shape, size_t ndim);
int is_valid_permutation(const size_t* perm, size_t ndim);
void* reorder_data(Array* arr, size_t* permutation, size_t* new_shape);
Array* transpose(Array* arr, size_t* permutation);

#endif // ARRAY_LINEAR_ALGEBRA_H