#pragma once
#include "con4.hpp"
#include "options.hpp"
#include <random>
#include <boost/serialization/vector.hpp>

// Chooses a random item in a vector
template <typename T>
T *random(std::vector<T> &iterable)
{
    assert(iterable.size() > 0 && "Invalid Size");
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, iterable.size() - 1);
    T *it = &iterable[dis(gen)];
    return it;
}

template <typename T>
T *randomWeight(std::vector<T> &iterable, std::vector<double> &weights)
{
    assert(iterable.size() > 0 && "Invalid Size");
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::discrete_distribution<> dis(weights.begin(), weights.end());
    T *it = &iterable[dis(gen)];
    return it;
}

// Serialize atomic types
namespace boost
{
    namespace serialization
    {
        template <typename Archive, typename T>
        void save(Archive &ar, const std::atomic<T> &t, const unsigned int version)
        {
            ar & t.load();
        }
        template <typename Archive, typename T>
        void load(Archive &ar, std::atomic<T> &t, const unsigned int version)
        {
            T tmp;
            ar & tmp;
            t.store(tmp);
        }
        template <typename Archive, typename T>
        void serialize(Archive &ar, std::atomic<T> &t, const unsigned int version)
        {
            boost::serialization::split_free(ar, t, version);
        }
    }
}

// Monte Carlo Tree Search
namespace MCTS
{
    static const int RESOURCES = 100;

    // Node class
    struct Node
    {
        Node() = default; // Default constructor

        Node(const Node &rhs) noexcept // Copy constructor
        {
            *this = rhs;
        }

        Node &operator=(const Node &rhs) noexcept // Assignment operator
        {
            turn = rhs.turn.load();
            state = rhs.state;
            col = rhs.col.load();
            leaf = rhs.leaf.load();
            parent = rhs.parent;
            children = rhs.children;
            defined = rhs.defined.load();
            wins = rhs.wins.load();
            visits = rhs.visits.load();
            return *this;
        }

        // Serialization
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & state;
            ar & children;
            ar & parent;
            ar & wins;
            ar & visits;
            ar & col;
            ar & turn;
            ar & leaf;
            ar & defined;
        }

        Con4 state;                       // Board corresponding to this node
        std::vector<Node> children{};     // Child nodes
        Node *parent = nullptr;           // Parent node
        std::atomic_int wins = 0;         // Number of winning simulations containing this node
        std::atomic_int visits = 0;       // Number of total sumulations containing this node
        std::atomic_int col = 0;          // Column where piece was placed last
        std::atomic_bool turn = false;    // Player whose turn it is
        std::atomic_bool leaf = true;     // Leaf node
        std::atomic_bool defined = false; // Child nodes instantiated
    };

    void define(Node *node);                                // Instatiates child nodes
    double UCT(Node &node);                                 // Returns the UCT of a node
    Node *select(Node *node);                               // Selection step of MCTS
    Node *expand(Node *node);                               // Expansion step of MCTS
    int simulate(Node *node);                               // Simulation step of MCTS
    void backprop(Node *node, Node *root, const int favor); // Backpropagation step of MCTS
    Node *find(const Con4 &board);                          // Finds the node corresponding to a game state
    void eval(Node *root);                                  // Does the evalution stuff
    int controller(const Con4 &board);                      // Controls the MCTS AI
};

extern MCTS::Node ROOT;