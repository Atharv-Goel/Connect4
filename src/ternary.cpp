#include "ternary.hpp"
#include <algorithm>

Tern::Tern(const std::size_t length)
{
    assert(length > 0 && "Invalid length given");
    size = length;
    ones = new bool[size]{false};
    twos = new bool[size]{false};
}

Tern::Tern(const Tern &tern)
{
    size = tern.size;
    ones = new bool[size]{false};
    twos = new bool[size]{false};
    std::copy(tern.ones, tern.ones + size, ones);
    std::copy(tern.twos, tern.twos + size, twos);
}

Tern &Tern::operator=(Tern tern)
{
    size = tern.size;
    std::swap(ones, tern.ones);
    std::swap(twos, tern.twos);
    return *this;
}

Tern::Tern(Tern &&tern) noexcept
{
    size = tern.size;
    std::swap(ones, tern.ones);
    std::swap(twos, tern.twos);
}

Tern::~Tern()
{
    delete[] ones;
    delete[] twos;
}

int Tern::count(const int state) const
{
    assert(0 <= state && state < 3 && "Invalid state given");
    int count = 0;
    if (state == 1)
    {
        for (int i = 0; i < size; i++)
        {
            count += ones[i];
        }
    }
    else if (state == 2)
    {
        for (int i = 0; i < size; i++)
        {
            count += twos[i];
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            count += 1 - (bool)(ones[i] + twos[i]);
        }
    }
    return count;
}

std::size_t Tern::getSize() const
{
    return size;
}

bool *Tern::getOnes() const
{
    return ones;
}

bool *Tern::getTwos() const
{
    return twos;
}

void Tern::set(const int index, const int state)
{
    assert(0 <= index && index < size && "Invalid index given");
    assert(0 <= state && state < 3 && "Invalid state given");
    if (state == 0)
    {
        ones[size - index - 1] = 0;
        twos[size - index - 1] = 0;
    }
    else if (state == 1)
    {
        ones[size - index - 1] = 1;
        twos[size - index - 1] = 0;
    }
    else
    {
        ones[size - index - 1] = 0;
        twos[size - index - 1] = 1;
    }
}

int Tern::get(const int index) const
{
    assert(0 <= index && "Invalid index given");
    if (index >= size)
    {
        return 0;
    }
    return ((bool)ones[size - index - 1]) + 2 * ((bool)twos[size - index - 1]);
}

void Tern::reset()
{
    for (int i = 0; i < size; i++)
    {
        set(i, 0);
    }
}

std::string Tern::to_string() const
{
    std::string str = "";
    for (int i = size - 1; i >= 0; i--)
    {
        str += std::to_string(get(i));
    }
    return str;
}

unsigned long Tern::to_ulong() const
{
    unsigned long tot = 0;
    unsigned long long pow = 1;
    for (int i = 0; i < size; i++)
    {
        tot += get(i) * pow;
        pow *= 3;
    }
    return tot;
}

unsigned long long Tern::to_ullong() const
{
    unsigned long long tot = 0;
    unsigned long long pow = 1;
    for (int i = 0; i < size; i++)
    {
        tot += get(i) * pow;
        pow *= 3;
    }
    return tot;
}

Tern Tern::operator&(const Tern &tern) const
{
    int maxSize = std::max(size, tern.size);
    Tern temp(maxSize);
    for (int i = 0; i < maxSize; i++)
    {
        temp.set(i, std::min(get(i), tern.get(i)));
    }
    return temp;
}

Tern Tern::operator|(const Tern &tern) const
{
    int maxSize = std::max(size, tern.size);
    Tern temp(maxSize);
    for (int i = 0; i < maxSize; i++)
    {
        temp.set(i, std::max(get(i), tern.get(i)));
    }
    return temp;
}

Tern Tern::operator^(const Tern &tern) const
{
    int maxSize = std::max(size, tern.size);
    Tern temp(maxSize);
    for (int i = 0; i < maxSize; i++)
    {
        temp.set(i, (get(i) + tern.get(i)) % 3);
    }
    return temp;
}

Tern Tern::operator~() const
{
    Tern temp(size);
    for (int i = 0; i < size; i++)
    {
        temp.set(i, 2 - get(i));
    }
    return temp;
}

Tern Tern::operator<<(const int &shift) const
{
    assert(shift > 0 && "Invalid shift given");
    Tern temp(size + shift);
    std::copy(ones, ones + size, temp.ones);
    std::copy(twos, twos + size, temp.twos);
    return temp;
}

Tern Tern::operator>>(const int &shift) const
{
    assert(0 < shift && shift < size && "Invalid shift given");
    Tern temp(size - shift);
    std::copy(ones, ones + size - shift, temp.ones);
    std::copy(twos, twos + size - shift, temp.twos);
    return temp;
}

bool Tern::operator==(const Tern &tern) const
{
    if (size != tern.size)
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        if ((ones[i] != tern.ones[i]) || (twos[i] != tern.twos[i]))
        {
            return false;
        }
    }
    return true;
}

bool Tern::operator!=(const Tern &tern) const
{
    if (size != tern.size)
    {
        return true;
    }
    for (int i = 0; i < size; i++)
    {
        if ((ones[i] != tern.ones[i]) || (twos[i] != tern.twos[i]))
        {
            return true;
        }
    }
    return false;
}

void Tern::operator&=(const Tern &tern)
{
    for (int i = 0; i < std::max(size, tern.size); i++)
    {
        set(i, std::min(get(i), tern.get(i)));
    }
}

void Tern::operator|=(const Tern &tern)
{
    for (int i = 0; i < std::max(size, tern.size); i++)
    {
        set(i, std::max(get(i), tern.get(i)));
    }
}

void Tern::operator^=(const Tern &tern)
{
    for (int i = 0; i < std::max(size, tern.size); i++)
    {
        set(i, (get(i) + tern.get(i)) % 3);
    }
}

void Tern::operator<<=(const int &shift)
{
    assert(shift > 0 && "Invalid shift given");
    Tern temp = *this << shift;
    *this = temp;
}

void Tern::operator>>=(const int &shift)
{
    assert(0 < shift && shift < size && "Invalid shift given");
    *this = (*this >> shift);
}

std::ostream &operator<<(std::ostream &os, const Tern &tern)
{
    os << tern.to_string();
    return os;
}