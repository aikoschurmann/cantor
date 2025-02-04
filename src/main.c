#include "gui.h"
#include "chess_gui.h"
#include "chess_bitboard.h"



// Main entry point
int main(int argc, char const *argv[]) {
    // Initialize everything
    initialize_window("Chess Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    initialize_keyboard_state();
    initialize_sprites();
    initialize_bitboards();

    // Current bitboard setup (Pawn to start with)
    Piece current_bitboard = PAWN;
    int white = 1; // 1 for white, 0 for black
    int draw_bitboard_mode = 1;

    while (should_continue) {
        // Handle key events
        if (is_key_down(SDL_SCANCODE_LEFT)) {
            current_bitboard = cycle_bitboard(current_bitboard, -1); // Go to previous bitboard
        }

        if (is_key_down(SDL_SCANCODE_RIGHT)) {
            current_bitboard = cycle_bitboard(current_bitboard, 1); // Go to next bitboard
        }

        if (is_key_down(SDL_SCANCODE_SPACE) || is_key_down(SDL_SCANCODE_UP)) {
            white = !white; // Toggle between white and black
        }

        if (is_key_down(SDL_SCANCODE_B)) {
            draw_bitboard_mode = !draw_bitboard_mode; // Toggle between drawing bitboards and not drawing them
        }

        // Process events (keyboard and mouse)
        handle_events();

        // Clear screen and render new frame
        clear_window();
        draw_chess_board();

        // Draw the selected bitboard (based on current piece and color)
        if (white && draw_bitboard_mode) {
            draw_bitboard(white_bitboards[current_bitboard], 255, 0, 0, 100); // Red for white
        } else if (!white && draw_bitboard_mode) {
            draw_bitboard(black_bitboards[current_bitboard], 0, 0, 255, 100); // Blue for black
        }

        // Present the rendered frame
        present_window();
    }

    return 0;
}
