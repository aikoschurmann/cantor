#include "array.h"
#include "operations.h" // Include this to define OperationFunc

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



Array* create_array(DataType dtype, size_t ndim, size_t* shape, void* data) {
    Array* arr = malloc(sizeof(Array));
    if (!arr) {
        fprintf(stderr, "Failed to allocate memory for Array\n");
        return NULL;
    }

    arr->ndim = ndim;
    arr->shape = malloc(ndim * sizeof(size_t));
    memcpy(arr->shape, shape, ndim * sizeof(size_t));

    if (!arr->shape) {
        free(arr);
        fprintf(stderr, "Failed to allocate memory for shape\n");
        return NULL;
    }

    arr->dtype = dtype;
    arr->size = 1;
    
    for (size_t i = 0; i < ndim; i++) {
        arr->size *= shape[i];
    }

    arr->data = malloc(arr->size * get_dtype_size(dtype));

    if (!arr->data) {
        free(arr->shape);
        free(arr);
        fprintf(stderr, "Failed to allocate memory for data\n");
        return NULL;
    }

    data ? memcpy(arr->data, data, arr->size * get_dtype_size(dtype)) : memset(arr->data, 0, arr->size * get_dtype_size(dtype));
    
    return arr;
}

void free_array(Array* arr) {
    free(arr->shape);
    free(arr->data);
    free(arr);
}

void* get_element(Array* arr, size_t* indices) {
    size_t offset = 0;
    size_t stride = 1;

    if (arr->ndim <= 0) {
       return NULL;
    }

    for (size_t i = 0; i < arr->ndim; i++) {
        if (indices[i] >= arr->shape[i]) {
            return NULL;
        }
    }
    
    for (size_t i = 0; i < arr->ndim; i++) {
        offset += indices[i] * stride;
        stride *= arr->shape[i];
    }

    return (char*)arr->data + offset * get_dtype_size(arr->dtype);
}

void set_element(Array* arr, size_t* indices, void* value) {
    void* element = get_element(arr, indices);
    if (element) {
        memcpy(element, value, get_dtype_size(arr->dtype));
    }
}

SizeAndShape check_shapes_and_calculate_size(Array* arr_a, Array* arr_b) {
    SizeAndShape result = {0, NULL, 0};
    size_t max_ndim = (arr_a->ndim > arr_b->ndim) ? arr_a->ndim : arr_b->ndim;
    
    result.shape = (size_t*)malloc(max_ndim * sizeof(size_t));
    if (!result.shape) {
        fprintf(stderr, "Memory allocation failed for result shape\n");
        return result; // Return default with NULL shape
    }

    result.total_size = 1;
    for (size_t i = 0; i < max_ndim; i++) {
        size_t size_a = (i < arr_a->ndim) ? arr_a->shape[arr_a->ndim - 1 - i] : 1;
        size_t size_b = (i < arr_b->ndim) ? arr_b->shape[arr_b->ndim - 1 - i] : 1;

        // Check compatibility for broadcasting
        if (size_a != size_b && size_a != 1 && size_b != 1) {
            free(result.shape); // Clean up allocated memory
            fprintf(stderr, "Shapes are not compatible\n");
            result.shape = NULL; // Indicate error
            return result; // Return default with NULL shape
        }

        result.shape[max_ndim - 1 - i] = (size_a > size_b) ? size_a : size_b; // Set the shape
        result.total_size *= result.shape[max_ndim - 1 - i]; // Calculate total size
    }

    result.ndim = max_ndim; // Set the number of dimensions
    
    return result; // Return the total size and shape
}

Array* opperate_on_arrays(Array* array_a, Array* array_b, char operation_symbol) {
    SizeAndShape result = check_shapes_and_calculate_size(array_a, array_b);
    if (!result.shape) {
        return NULL; // Return NULL if shapes are not compatible
    }

    Array* result_array = create_array(array_a->dtype, result.ndim, result.shape, NULL);
    if (!result_array) {
        free(result.shape); // Clean up allocated memory
        return NULL; // Return NULL if memory allocation failed
    }

    size_t element_size = get_dtype_size(array_a->dtype);
    OperationFunc op = get_operation(operation_symbol);

    if (!op) {
        fprintf(stderr, "Unsupported operation: %c\n", operation_symbol);
        free(result.shape);
        free_array(result_array); // Clean up allocated memory
        return NULL; // Return NULL if operation is not supported
    }

    for (size_t i = 0; i < result.total_size; i++) {
        size_t indices_a[result.ndim];
        size_t indices_b[result.ndim];

        // Convert linear index to multi-dimensional indices
        size_t temp = i;
        for (size_t j = 0; j < result.ndim; j++) {
            indices_a[result.ndim - 1 - j] = temp % result.shape[result.ndim - 1 - j];
            indices_b[result.ndim - 1 - j] = indices_a[result.ndim - 1 - j]; // Assuming same indices for broadcasting
            temp /= result.shape[result.ndim - 1 - j];
        }

        // Get the values from both arrays
        void* value_a = get_element(array_a, indices_a);
        void* value_b = get_element(array_b, indices_b);
        //print
        //printf("value_a: %d\n", *(int*)value_a);
        //printf("value_b: %d\n", *(int*)value_b);
        

        void* result_value = (void*)result_array->data + i * element_size;

        // Call the operation function
        op(result_value, value_a, value_b);

        //print
        //printf("result_value: %d\n", *(int*)result_value);
    }

    free(result.shape); // Clean up the shape array
    return result_array; // Return the resulting array
}
