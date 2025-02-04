#ifndef GUI2_H
#define GUI2_H
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Include for memcpy

SDL_Renderer *renderer;

int mouse_location[2];
int mouse_clicked;
int should_continue;

void initialize_keyboard_state();

int is_key_pressed(SDL_Scancode key);
int is_key_down(SDL_Scancode key);
int is_key_up(SDL_Scancode key);

void handle_events();

void clear_window();

void draw_rectangle(int x, int y, int width, int height);
void initialize_window(const char *title, int window_width, int window_height);
void cleanup();
void present_window();
void set_color(int r, int g, int b, int a);

SDL_Texture *load_sprite(const char *path);
void draw_sprite(SDL_Texture *texture, int x, int y);
void draw_sprite_scaled(SDL_Texture *texture, int x, int y, int width, int height);
void cleanup_sprite(SDL_Texture *texture);

#endif