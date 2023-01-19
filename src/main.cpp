#include "options.hpp"
#include "ai.hpp"
#include "con4.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>

MCTS::Node ROOT;

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    }
    // Initialize SDL_ttf
    if (TTF_Init() != 0)
    {
        std::cout << "Error initializing TTF: " << TTF_GetError() << std::endl;
    }

    // Display
    SDL_Window *win = SDL_CreateWindow("CONNECT 4", // Create the window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       800, 720, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); // Create the renderer
    TTF_Font *font = TTF_OpenFont("../LilitaOne-Regular.ttf", 85);              // Initalize Font

    Con4 board;     // Create the board
    ROOT.state = board; // Link the root node to the empty board

    int winner;   // Winner of game
    int turn = 1; // The initial turn
    int choice;   // The move that will be selected
    bool reset = false;

    output(board, rend); // Draw the board

    SDL_Event e;
    while (true)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            { // Exit
                TTF_Quit();
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(win);
                SDL_Quit();
                std::ofstream ofs("data");
                return 1;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            { // Play again
                if (reset) // Game needs to be reset
                {
                    board.reset();
                    output(board, rend);
                    reset = false;
                    turn = 1;
                }
                else
                {
                    choice = find(board, turn);
                    board.drop(choice, turn); // Drop the piece
                    turn = 3 - turn;
                    if (board.check() || board.pos().size() == 0)
                    {
                        output(board, rend, font, board.check());
                        reset = true;
                    }
                    else
                    {
                        output(board, rend);
                    }
                }
            }
        }
    }
}