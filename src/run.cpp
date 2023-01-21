#include "run.hpp"
#include "ai.hpp"
#include <vector>
#include <algorithm>
#include <string>

// Filled circle function
void SDL_RenderFillCircle(SDL_Renderer *rend, int x0, int y0, int radius)
{
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;
    while (x >= y)
    {
        SDL_RenderDrawLine(rend, x + x0, y + y0, -x + x0, y + y0);
        SDL_RenderDrawLine(rend, y + x0, x + y0, -y + x0, x + y0);
        SDL_RenderDrawLine(rend, -x + x0, -y + y0, x + x0, -y + y0);
        SDL_RenderDrawLine(rend, -y + x0, -x + y0, y + x0, -x + y0);
        y++;
        if (radiusError < 0)
            radiusError += 2 * y + 1;
        else
        {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}

void output(const Con4 &board, SDL_Renderer *rend, TTF_Font *font, int winner)
{

    SDL_SetRenderDrawColor(rend, 45, 45, 55, 255); // Set the renderer color
    SDL_RenderClear(rend);                         // Clear the renderer

    SDL_SetRenderDrawColor(rend, 0, 0, 255, 255); // Set the renderer color
    SDL_Rect base = {100, 100, 600, 520};
    SDL_RenderFillRect(rend, &base);

    int piece;
    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 7; col++)
        {

            piece = board.get(41 - (row * 7 + col));
            if (piece == 0)
            {
                SDL_SetRenderDrawColor(rend, 45, 45, 55, 255);
            }
            else if (piece == 1)
            {
                SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(rend, 255, 255, 0, 255);
            }
            SDL_RenderFillCircle(rend, col * 80 + 160, row * 80 + 160, 25); // Fill the rect with the color
        }
    }

    const char *txt;
    if (winner == 0)
    {
        txt = "DRAW";
    }
    else
    {
        std::string tempText = (std::string("PLAYER ") + std::to_string(winner) + " WINS!");
        txt = tempText.c_str();
    }

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, txt, {255, 255, 255});
    SDL_Texture *text = SDL_CreateTextureFromSurface(rend, textSurface);

    SDL_Rect textBox;
    textBox.x = 400 - textSurface->w / 2;
    textBox.y = 50 - textSurface->h / 2;
    textBox.w = textSurface->w;
    textBox.h = textSurface->h;

    SDL_RenderCopy(rend, text, NULL, &textBox);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(text);

    SDL_RenderPresent(rend); // Draw the renderer on the screen
}

void output(const Con4 &board, SDL_Renderer *rend)
{

    SDL_SetRenderDrawColor(rend, 45, 45, 55, 255); // Set the renderer color
    SDL_RenderClear(rend);                         // Clear the renderer

    SDL_SetRenderDrawColor(rend, 0, 0, 255, 255); // Set the renderer color
    SDL_Rect base = {100, 100, 600, 520};
    SDL_RenderFillRect(rend, &base);

    int piece;
    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 7; col++)
        {

            piece = board.get(41 - (row * 7 + col));
            if (piece == 0)
            {
                SDL_SetRenderDrawColor(rend, 45, 45, 55, 255);
            }
            else if (piece == 1)
            {
                SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(rend, 255, 255, 0, 255);
            }
            SDL_RenderFillCircle(rend, col * 80 + 160, row * 80 + 160, 25); // Fill the rect with the color
        }
    }

    SDL_RenderPresent(rend); // Draw the renderer on the screen
}

int find(const Con4 &board, const int turn)
{
    if (turn == 0) {
        // Find all available columns and add them to the vector
        std::vector<int> pos = board.pos();

        // Check to see if there are any valid moves
        if (pos.size() == 0)
        {
            return -1;
        }

        /*
        int choice = -1;
        do {
            std::cout << "move: ";
            std::cin >> choice;
        } while (!(std::count(pos.begin(), pos.end(), choice)));
        return choice;*/
        return *random(pos);
    }
    else {
        return(MCTS::controller(board));
    }
}

int run(Con4 &board)
{
    // Connect 4 variables
    int turn = 1;
    int choice;
    int favor;

    // Play until connect 4
    do
    {
        choice = find(board, turn); // Get the chosen move
        if (choice < 0)
        { // Tie
            return 0;
        }
        board.drop(choice, turn);  // Drop the piece
        turn = 3 - turn;           // Switch to other player's turn
    } while (!(favor = board.check()));

    return favor;
}