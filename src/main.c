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
