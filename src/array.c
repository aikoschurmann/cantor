#include "array.h"
#include "operations.h"

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

Array* allocate_array_memory() {
    Array* arr = malloc(sizeof(Array));
    if (!arr) {
        fprintf(stderr, "Failed to allocate array memory");
        return NULL;
    }
    return arr;
}

size_t* allocate_shape_memory(size_t ndim) {
    //initialize to zero

    size_t *shape = calloc(ndim, sizeof(size_t));

    if (!shape) {
        fprintf(stderr, "Failed to allocate shape memory");
        return NULL;
    }
    return shape;
}

void* allocate_data_memory(size_t data_size) {
    void *data = calloc(data_size, 1);
    if (!data) {
        fprintf(stderr, "Failed to allocate memory for data");
        return NULL;
    }
    return data;
}

void print_shape(size_t* shape, size_t ndim) {
    if (shape == NULL) {
        printf("\033[31mShape: NULL\033[0m\n");  // Red for NULL
        return;
    }
    
    printf("\033[32mShape:\033[0m (");  // Green for shape
    for (size_t i = 0; i < ndim; i++) {
        printf("%zu", shape[i]);
        if (i < ndim - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

Array* create_array(DataType dtype, size_t ndim, size_t *shape, void *data) {
    Array* arr = allocate_array_memory();
    if (!arr) return NULL;

    arr->shape = allocate_shape_memory(ndim);
    if (!arr->shape) {
        free(arr);
        return NULL;
    }

    memcpy(arr->shape, shape, ndim * sizeof(size_t));

    arr->ndim = ndim;
    arr->size = 1;
    for (size_t i = 0; i < ndim; i++) {
        arr->size *= shape[i];
    }

    size_t data_size = arr->size * get_dtype_size(dtype);
    arr->data = allocate_data_memory(data_size);
    if (!arr->data) {
        free(arr->shape);
        free(arr);
        return NULL;
    }

    data ? memcpy(arr->data, data, data_size) : memset(arr->data, 0, data_size);

    arr->dtype = dtype;

    return arr;
}

void free_array(Array* arr) {
    if (arr) {
        free(arr->shape);
        free(arr->data);
        free(arr);
        arr = NULL; // Prevent dangling pointer
    }
}


int validate_indices(Array* arr, size_t* indices) {
    for (size_t i = 0; i < arr->ndim; i++) {
        if (indices[i] >= arr->shape[i]) {
            return 0; // Invalid indeces
        }
    }
    return 1; // Valid indices
}

size_t calculate_offset(Array* arr, size_t* indices) {
    size_t offset = 0;
    size_t stride = 1;

    // Iterate over the dimensions in reverse order
    for (size_t i = arr->ndim; i > 0; i--) {
        size_t dim_index = i - 1; // Adjust index for zero-based indexing
        offset += indices[dim_index] * stride;
        stride *= arr->shape[dim_index];
    }
    return offset;
}


void* get_element(Array* arr, size_t* indices) {
    if (arr->ndim <= 0) {
        return NULL;
    }

    if (!validate_indices(arr, indices)) {
        return NULL;
    }

    size_t offset = calculate_offset(arr, indices);


    // Cast to (char*) for byte-wise pointer arithmetic, as char is 1 byte. 
    // This allows us to calculate the correct memory address for the element 
    // based on the byte offset (offset * get_dtype_size(arr->dtype)),
    // where get_dtype_size returns the size in bytes of the element type 
    // stored in the array, ensuring accurate indexing for any data type.
    return (char*)arr->data + offset * get_dtype_size(arr->dtype);
}

int set_element(Array* arr, size_t* indices, void* value) {
    void* element = get_element(arr, indices);
    if(element){
        memcpy(element, value, get_dtype_size(arr->dtype));
        return 1; // Succes
    }
    return 0; // Failure
}

void get_dim_value(size_t* shape, size_t ndim, size_t index, size_t* dim_value) {
    if (index < ndim) {
        *dim_value = shape[ndim - 1 - index]; // Access from the end
    } else {
        *dim_value = 1; // Out of bounds, treat as 1
    }
}

int are_dims_compatible(size_t dimA, size_t dimB) {
    return (dimA == dimB) || (dimA == 1) || (dimB == 1);
}

size_t* broadcast_shapes(size_t* shapeA, size_t ndimA, size_t* shapeB, size_t ndimB, size_t* result_ndim) {
    if (!shapeA || !shapeB) {
        fprintf(stderr, "One of the shapes is NULL\n");
        return NULL;
    }

    if (ndimA == 0 || ndimB == 0) {
        fprintf(stderr, "Empty shape detected. Broadcasting is not possible.\n");
        return NULL; 
    }

    size_t max_rank = (ndimA > ndimB) ? ndimA : ndimB;
    size_t* result_shape = malloc(max_rank * sizeof(size_t));
    if (!result_shape) {
        fprintf(stderr, "Failed to allocate memory for result shape\n");
        return NULL;
    }

    for (size_t i = 0; i < max_rank; i++) {
        size_t dimA, dimB;
        get_dim_value(shapeA, ndimA, i, &dimA);
        get_dim_value(shapeB, ndimB, i, &dimB);

        if (!are_dims_compatible(dimA, dimB)) {
            free(result_shape);
            fprintf(stderr, "Shapes are not broadcastable: (%zu) and (%zu)\n", dimA, dimB);
            return NULL; 
        }

        result_shape[max_rank - 1 - i] = (dimA > dimB) ? dimA : dimB; 
    }

    *result_ndim = max_rank; 
    return result_shape; 
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


Array* broadcast_arrays_fast(Array* arr_a, Array* arr_b, char operation_symbol) {
    size_t data_size = arr_a->size * get_dtype_size(arr_a->dtype);
    Array* result = create_array(arr_a->dtype, arr_a->ndim, arr_a->shape, NULL);
    if (!result) {
        fprintf(stderr, "Failed to allocate memory for result array\n");
        return NULL;
    }

    for (size_t i = 0; i < arr_a->size; i++) {
        void* elem_a = (char*)arr_a->data + i * get_dtype_size(arr_a->dtype);
        void* elem_b = (char*)arr_b->data + i * get_dtype_size(arr_b->dtype);
        void* elem_res = (char*)result->data + i * get_dtype_size(result->dtype);

        apply_operation(operation_symbol, elem_res, elem_a, elem_b, arr_a->dtype);
    }
    return result;
}


Array* broadcast_arrays(Array* arr_a, Array* arr_b, char operation_symbol) {
    if (!arr_a || !arr_b) {
        fprintf(stderr, "One of the arrays is NULL\n");
        return NULL;
    }

    if (are_shapes_equal(arr_a->shape, arr_a->ndim, arr_b->shape, arr_b->ndim)) {
        return broadcast_arrays_fast(arr_a, arr_b, operation_symbol);
    }

    size_t result_ndim;
    size_t* result_shape = broadcast_shapes(arr_a->shape, arr_a->ndim, arr_b->shape, arr_b->ndim, &result_ndim);
    if (!result_shape) {
        fprintf(stderr, "Failed to broadcast shapes\n");
        return NULL;
    }

    Array* result = create_array(arr_a->dtype, result_ndim, result_shape, NULL);
    if (!result) {
        fprintf(stderr, "Failed to allocate memory for result array\n");
        return NULL;
    }

    size_t* indices = allocate_shape_memory(result_ndim);
    if (!indices) {
        free_array(result);
        return NULL;
    }

    for (size_t i = 0; i < result->size; i++) {
        for (size_t j = 0; j < result_ndim; j++) {
            indices[j] = i / result->shape[j] % result->shape[j];
        }

        size_t* indices_a = map_broadcasted_to_original_indices(result_shape, result_ndim, arr_a->shape, arr_a->ndim, indices);
        size_t* indices_b = map_broadcasted_to_original_indices(result_shape, result_ndim, arr_b->shape, arr_b->ndim, indices);
        free(result_shape);

        if (!indices_a || !indices_b) {
            fprintf(stderr, "Failed to map indices\n");
            free_array(result);
            free(indices);
            free(indices_a);
            free(indices_b);
            return NULL;
        }

        void* elem_a = get_element(arr_a, indices_a);
        void* elem_b = get_element(arr_b, indices_b);
        void* elem_res = get_element(result, indices);

        apply_operation(operation_symbol, elem_res, elem_a, elem_b, arr_a->dtype);

        free(indices_a);
        free(indices_b);
    }

    free(indices);
    return result;
}

Array* flatten_array(Array* arr) {
    if (!arr) {
        fprintf(stderr, "Array is NULL\n");
        return NULL;
    }

    if (arr->ndim == 1) {
        return arr;
    }

    size_t new_shape[] = {arr->size};
    Array* result = create_array(arr->dtype, 1, new_shape, NULL);
    if (!result) {
        fprintf(stderr, "Failed to allocate memory for result array\n");
        return NULL;
    }

    for (size_t i = 0; i < arr->size; i++) {
        size_t indices[] = {i};
        void* elem = get_element(arr, indices);
        void* elem_res = get_element(result, indices);
        memcpy(elem_res, elem, get_dtype_size(arr->dtype));
    }

    return result;
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

        //print 
        printf("elem_a: %d\n", *(int*)elem_a);
        printf("elem_b: %d\n", *(int*)elem_b);

        if (memcmp(elem_a, elem_b, get_dtype_size(arr_a->dtype)) != 0) {
            
        }
    }

    return 1;
}

size_t* map_broadcasted_to_original_indices(
    size_t* broadcasted_shape, size_t ndim_broadcasted,
    size_t* original_shape, size_t ndim_original,
    size_t* broadcasted_indices
) {
    //check if any of the inputs are NULL
    if (!broadcasted_shape || !original_shape || !broadcasted_indices) {
        printf("One of the inputs is NULL\n");
        return NULL;  // One of the inputs is NULL
    }

    // Allocate memory for original indices
    size_t* original_indices = allocate_shape_memory(ndim_original);
    if (!original_indices) {
        printf("Failed to allocate memory for original indices\n");
        return NULL;  // Memory allocation failed
    }

    size_t shape_offset = ndim_broadcasted - ndim_original;

    // Check for valid shape offset
    if (shape_offset < 0) {
        free(original_indices);
        printf("Invalid shape offset\n");
        return NULL;  // Invalid offset
    }

    for (size_t i = 0; i < ndim_original; i++) {
        // Check if we're accessing valid indices
        if (i + shape_offset >= ndim_broadcasted) {
            free(original_indices);
            printf("Accessing out of bounds in broadcasted_shape\n");
            return NULL;  // Accessing out of bounds in broadcasted_shape
        }
        if (i >= ndim_original) {
            free(original_indices);
            printf("Accessing out of bounds in original_shape\n");
            return NULL;  // Accessing out of bounds in original_shape
        }

        size_t broadcast_dim = broadcasted_shape[i + shape_offset];
        size_t original_dim = original_shape[i];
        size_t broadcast_index = broadcasted_indices[i + shape_offset];

        // Ensure the index is valid for the broadcasted shape
        if (broadcast_index >= ndim_broadcasted) {
            free(original_indices);
            printf("Accessing out of bounds in broadcasted_indices\n");
            return NULL;  // Accessing out of bounds in broadcasted_indices
        }

        if (original_dim == broadcast_dim) {
            original_indices[i] = broadcast_index;
        } else if (original_dim == 1) {
            original_indices[i] = 0;  // Use the first index
        } else {
            free(original_indices);  // Free memory to prevent a leak
            printf("Shapes are incompatible\n");
            return NULL;  // Shapes are incompatible
        }
    }

    return original_indices;
}
