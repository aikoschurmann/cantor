#include "array.h"

Array* create_array(DataType dtype, size_t ndim, size_t *shape, void *data) {
    Array* arr = allocate_array_memory();
    if (!arr) return NULL;

    arr->shape = allocate_shape_memory(ndim);
    if (!arr->shape) {
        free(arr);
        return NULL;
    }

    if (shape){
        memcpy(arr->shape, shape, ndim * sizeof(size_t));
    } else {
        free(arr->shape);
        free(arr);
        log_error("Shape is NULL");
        return NULL;
    }


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