#pragma once
#include "con4.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

// Print the board out onto the screen
void output(const Con4 &board, SDL_Renderer *rend, TTF_Font *font, const int winner);

// Print the board out onto the screen
void output(const Con4 &board, SDL_Renderer *rend);

// Finds a move
int find(const Con4 &board);