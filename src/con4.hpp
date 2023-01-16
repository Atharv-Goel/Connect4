#pragma once
#include "ternary.hpp"
#include <vector>

class Con4 : public Tern
{

private:
    Tern same(const Tern tern) const;                    // Ternary function similar to and but only works when both trits are equal
    Tern same(const Tern tern1, const Tern tern2) const; // Ternary function similar to and but only works when both trits are equal

public:
    Con4();                    // Constructor
    Con4(const Con4 &con);     // Copy constructor
    Con4 &operator=(Con4 con); // Assignemnt operator
    Con4(Con4 &&con) noexcept; // Move constructor
    ~Con4();                   // Destructor

    void drop(int col, int turn);       // Drops a piece in a column
    std::vector<int> pos() const;       // Gets the possible moves
    int check() const;                  // Checks if the game is over
    bool prev(const Con4 &con) const;   // Checks the board was previously the given board
};