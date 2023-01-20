#pragma once
#include <string>
#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Tern
{
private:
    //Serialization
    friend class boost::serialization::access;
    template <class Archive>
    void save(Archive &ar, const unsigned int version) const
    {
        ar & to_ullong();
        ar & size;
    }
    template <class Archive>
    void load(Archive &ar, const unsigned int version)
    {
        unsigned long long temp = 0;
        ar & temp;
        ar & size;
        ones = new bool[size];
        twos = new bool[size];
        for (int i  = 0; i < size; i++)
        {
            set(i, temp % 3);
            temp /= 3;
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

protected:
    bool *ones = nullptr; // Boolean array to keep track of all ones
    bool *twos = nullptr; // Boolean array to keep track of all twos
    int size;             // Length of arrays

public:
    Tern(const std::size_t length); // Constructor
    Tern(const Tern &tern);         // Copy constructor
    Tern &operator=(Tern tern);     // Assignemnt operator
    Tern(Tern &&tern) noexcept;     // Move constructor
    ~Tern();                        // Destructor

    int count(const int state = 2) const; // Returns the number of a certain state in the array (default 2)
    std::size_t getSize() const;          // Returns the size of the array
    bool *getOnes() const;                // Returns the ones bool array
    bool *getTwos() const;                // Returns the twos bool array

    void set(const int index, const int state = 2); // Sets an index to a state (default 2)
    int get(const int index) const;                 // Returns the state of an index
    void reset();                                   // Sets all indices to 0

    std::string to_string() const;        // Returns the array as a string
    unsigned long to_ulong() const;       // Returns the array as an unsigned long
    unsigned long long to_ullong() const; // Returns the array as an unsigned long long

    /*
    There are 19683 ternary operators
    Thus only the basic binary operators are implemented
    The rest can be made through custom functions or combinations
    */
    Tern operator&(const Tern &tern) const;  // Binary operator and
    Tern operator|(const Tern &tern) const;  // Binary operator or
    Tern operator^(const Tern &tern) const;  // Binary operator xor
    Tern operator~() const;                  // Binary operator not
    Tern operator<<(const int &shift) const; // Left shift
    Tern operator>>(const int &shift) const; // Right shift

    bool operator==(const Tern &tern) const; // Equivalence
    bool operator!=(const Tern &tern) const; // Anti Equivalence

    void operator&=(const Tern &tern);  // In-place binary operator and
    void operator|=(const Tern &tern);  // In-place binary operator or
    void operator^=(const Tern &tern);  // In-place binary operator xor
    void operator<<=(const int &shift); // In-place left shift
    void operator>>=(const int &shift); // In-place right shift
};

std::ostream &operator<<(std::ostream &os, const Tern &tern); // Cout << overload