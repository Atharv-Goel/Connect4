#include "run.hpp"
#include "ai.hpp"
#include "con4.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>

MCTS::Node ROOT;

int main()
{
    // If there is a data file and serializing is on, then load it
    if (SERIALIZATION) {
        std::ifstream ifs("data.txt");
        if (ifs.peek() == std::ifstream::traits_type::eof())
        {
            std::cout << "No data file found, will create a new one" << std::endl;
        }
        else
        {
            boost::archive::text_iarchive ia(ifs);
            ia >> ROOT;
            std::cout << "Loaded data file" << std::endl;
        }
    }

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

    SDL_Window *win = SDL_CreateWindow("CONNECT 4", SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 800, 720, 0);    // Create the window
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); // Create the renderer
    TTF_Font *font = TTF_OpenFont("../LilitaOne-Regular.ttf", 85);              // Initalize Font

    Con4 board;         // Create the board
    ROOT.state = board; // Link the root node to the empty board

    int winner;   // Winner of game
    int turn = 1; // The initial turn
    int choice;   // The move that will be selected
    bool reset = false;

    output(board, rend); // Draw the board

    SDL_Event e;
    while (true)
    {
        //Check for events
        if (SDL_PollEvent(&e))
        {
            // Check for exit
            if (e.type == SDL_QUIT) {
                // Destroy all the graphics
                TTF_Quit();
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(win);
                SDL_Quit();
                // Save the data into a file
                if (SERIALIZATION) {
                    std::ofstream ofs("data.txt");
                    boost::archive::text_oarchive oa(ofs);
                    oa << ROOT;
                }
                return 1;
            }
            //Check for click
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Reset game if needed
                if (reset) {
                    board.reset();
                    output(board, rend);
                    reset = false;
                    turn = 1;
                }
                // Play a move and display the board
                else {
                    choice = find(board, turn);
                    board.drop(choice, turn);
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