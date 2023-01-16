#include "con4.hpp"

Con4::Con4() : Tern(42) {}

Con4::Con4(const Con4 &con) : Tern(42)
{
    size = con.size;
    ones = new bool[size]{false};
    twos = new bool[size]{false};
    std::copy(con.ones, con.ones + size, ones);
    std::copy(con.twos, con.twos + size, twos);
}

Con4 &Con4::operator=(Con4 con)
{
    size = con.size;
    std::swap(ones, con.ones);
    std::swap(twos, con.twos);
    return *this;
}

Con4::Con4(Con4 &&con) noexcept : Tern(42)
{
    size = con.size;
    std::swap(ones, con.ones);
    std::swap(twos, con.twos);
}

Con4::~Con4() {}

std::vector<int> Con4::pos() const
{
    std::vector<int> pos;
    for (int col = 6; col >= 0; col--)
    {
        if (!get(41 - col))
        {
            pos.push_back(col);
        }
    }
    return pos;
}

Tern Con4::same(const Tern tern) const
{
    Tern temp(std::max((std::size_t)size, tern.getSize()));
    for (int i = 0; i < std::min((std::size_t)size, tern.getSize()); i++)
    {
        int val = (ones[size - i - 1] & tern.getOnes()[tern.getSize() - i - 1]) +
                     2 * (twos[size - i - 1] & tern.getTwos()[tern.getSize() - i - 1]);
        temp.set(i, val);
    }
    return temp;
}

Tern Con4::same(const Tern tern1, const Tern tern2) const
{
    Tern temp(std::max(tern1.getSize(), tern2.getSize()));
    for (int i = 0; i < std::min(tern1.getSize(), tern2.getSize()); i++)
    {
        temp.set(i,
                 (tern1.getOnes()[tern1.getSize() - i - 1] & tern2.getOnes()[tern2.getSize() - i - 1]) +
                     2 * (tern1.getTwos()[tern1.getSize() - i - 1] & tern2.getTwos()[tern2.getSize() - i - 1]));
    }
    return temp;
}

int Con4::check() const
{
    // 1. Check the flattened array to see if there are 4-in-a-row
    const Tern tempH = same(same(*this << 1), same((*this << 2), (*this << 3)));    // Horizontal check
    const Tern tempD1 = same(same(*this << 8), same((*this << 16), (*this << 24))); // Diagonal 1 check
    const Tern tempV = same(same(*this << 7), same((*this << 14), (*this << 21)));  // Vertical check
    const Tern tempD2 = same(same(*this << 6), same((*this << 12), (*this << 18))); // Diagonal 2 check

    // 2. Check to make sure that the 4-in-a-row doesn't wrap around to the next row
    int piece;
    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col <= 3; col++)
        {
            piece = tempV.get(tempV.getSize() - (row * 7 + col) - 1); // Vertical
            if (piece)
            {
                return piece;
            }
            piece = tempD2.get(tempD2.getSize() - (row * 7 + col) - 1); // Diagonal 2
            if (piece)
            {
                return piece;
            }
        }
        for (int col = 3; col < 7; col++)
        {
            piece = tempH.get(tempH.getSize() - (row * 7 + col) - 1); // Horizontal
            if (piece)
            {
                return piece;
            }
            piece = tempD1.get(tempD1.getSize() - (row * 7 + col) - 1); // Diagonal 1
            if (piece)
            {
                return piece;
            }
            piece = tempV.get(tempV.getSize() - (row * 7 + col) - 1); // Vertical
            if (piece)
            {
                return piece;
            }
        }
    }

    // No connect 4
    return 0;
}

void Con4::drop(int col, int turn)
{
    set(41 - col, turn);
    int row = 1;
    // As long as the space below is empty drop the piece
    while (row < 6 && !get(41 - (7 * row + col)))
    {
        set(41 - (7 * (row - 1) + col), 0);
        set(41 - (7 * row + col), turn);
        row++;
    }
}

bool Con4::prev(const Con4 &con) const
{
    for (int i = 0; i < 42; i++)
    {
        if (con.get(i) > 0 && get(i) != con.get(i))
        {
            return false;
        }
    }
    return true;
}