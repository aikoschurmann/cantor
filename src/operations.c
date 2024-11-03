#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "array.h"
#include "operations.h"

// Define function pointer type for operations
typedef void (*OpFunc)(void* result, const void* a, const void* b);

// Integer operations
void add_int(void* result, const void* a, const void* b) {
    *(int*)result = *(int*)a + *(int*)b;
}

void sub_int(void* result, const void* a, const void* b) {
    *(int*)result = *(int*)a - *(int*)b;
}

void mul_int(void* result, const void* a, const void* b) {
    *(int*)result = *(int*)a * *(int*)b;
}

void div_int(void* result, const void* a, const void* b) {
    *(int*)result = *(int*)a / *(int*)b;
}

// Float operations
void add_float(void* result, const void* a, const void* b) {
    *(float*)result = *(float*)a + *(float*)b;
}

void sub_float(void* result, const void* a, const void* b) {
    *(float*)result = *(float*)a - *(float*)b;
}

void mul_float(void* result, const void* a, const void* b) {
    *(float*)result = *(float*)a * *(float*)b;
}

void div_float(void* result, const void* a, const void* b) {
    *(float*)result = *(float*)a / *(float*)b;
}

// Double operations
void add_double(void* result, const void* a, const void* b) {
    *(double*)result = *(double*)a + *(double*)b;
}

void sub_double(void* result, const void* a, const void* b) {
    *(double*)result = *(double*)a - *(double*)b;
}

void mul_double(void* result, const void* a, const void* b) {
    *(double*)result = *(double*)a * *(double*)b;
}

void div_double(void* result, const void* a, const void* b) {
    *(double*)result = *(double*)a / *(double*)b;
}

// Function pointer tables for each data type and operation
OpFunc int_ops[] = { add_int, sub_int, mul_int, div_int };
OpFunc float_ops[] = { add_float, sub_float, mul_float, div_float };
OpFunc double_ops[] = { add_double, sub_double, mul_double, div_double };

// Lookup table for selecting the appropriate operations table
OpFunc* op_tables[] = { int_ops, float_ops, double_ops };

// Utility function to map operation symbols to table indices
int get_op_index(char op) {
    switch (op) {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        default: return -1;
    }
}

void apply_operation(char operation_symbol, void* result, void* a, void* b, DataType dtype) {
    int op_index = get_op_index(operation_symbol);
    if (op_index == -1 || dtype < TYPE_INT || dtype > TYPE_DOUBLE) {
        fprintf(stderr, "Unsupported operation or data type\n");
        return;
    }
    // Execute the appropriate operation
    op_tables[dtype][op_index](result, a, b);
}

#endif