#include <stdio.h>
#include "bitboard.h"

// Prints a bitboard for debugging
void print_bitboard(Bitboard bb) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            printf("%d ", (bb & (1ULL << square)) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n");
}
