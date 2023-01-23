#include "ai.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// Multithreading setup
#define NUMTHREADS 1
std::vector<bool> jobs;
std::mutex jobsMutex;
std::thread threads[NUMTHREADS];

double MCTS::UCT(Node &node)
{
    return ((double) node.wins) / node.visits + 0.7 * std::sqrt(std::log(node.parent->visits.load()) / node.visits);;
}

MCTS::Node* MCTS::select(Node* node)
{
    double max;
    Node* newNode = node;
    double val;
    // Continue choosing children until a leaf node is found
    while (!newNode->leaf)
    {
        // Make sure that the children nodes have been instantiated
        define(newNode);
        newNode->visits += 1;

        max = 0;
        // Keep choosing children by ranking UCT
        for (Node &child : newNode->children)
        {
            if ((val = UCT(child)) > max)
            {
                max = val;
                newNode = &child;
            }
        }
    }
    define(newNode);
    newNode->visits += 1;
    return newNode;
}

void MCTS::define(Node* node)
{
    if (!node->defined) {
        node->defined = true;

        // Create a new child for each possible child state
        std::vector<int> pos = node->state.pos();
        for (int col : pos)
        {
            Node child;
            child.turn = !node->turn;
            child.col = col;
            child.parent = node;
            child.state = node->state;
            child.state.drop(col, node->turn + 1);
            node->children.emplace_back(child);
        }
    }
}

MCTS::Node* MCTS::expand(Node* node)
{
    // Choose a random child that has not been visited yet
    std::vector<int> pos;
    for (int i = 0; i < node->children.size(); i++)
    {
        if (!node->children[i].visits) {pos.push_back(i);}
    }

    if (pos.size() == 0 ) {return node;}

    Node* chosen = &(node->children[*random(pos)]);
    chosen->visits += 1;

    // If the chosen node was the last unvisited child node, the node is no longer a leaf
    if (pos.size() == 1) {node->leaf = false;}

    return chosen;
}

int MCTS::simulate(Node* node)
{
    // Keep choosing children until the state is terminal
    int favor;
    Node* newNode = node;
    while (!(favor = newNode->state.check()) && newNode->state.pos().size())
    {
        // Instantiate children if not defined
        define(newNode);
        // Choose a random child using win ratio as weights
        std::vector<double> weights;
        for (Node &child : newNode->children) {
            weights.push_back(((double) child.wins) / child.visits);
        }
        newNode = randomWeight(newNode->children, weights);
    }
    // Return the game winner
    return newNode->state.check();
}

void MCTS::backprop(Node* node, Node* root, const int favor)
{
    Node* newNode = node;
    while (newNode != root)
    {
        if (!newNode->turn == favor - 1) {
            newNode->wins += 2;
        }
        else if (favor == 0) {
            newNode->wins += 1;
        }
        newNode = newNode->parent;
    }
    if (newNode->turn == favor - 1) {
        newNode->wins += 2;
    }
    else if (favor == 0) {
        newNode->wins += 1;
    }
}

MCTS::Node* MCTS::find(const Con4 &board)
{
    std::vector<Node*> queue{&ROOT};
    Node* current;
    while (queue.size())
    {
        current = queue[0];
        queue.erase(queue.begin());
        if (board.prev(current->state))
        {
            if (current->state == board)
            {
                return current;
            }
            for (Node &child: current->children) {
                queue.push_back(&child);
            }
        }
    }
    return current;
}

void MCTS::eval(Node* root)
{
    int favor;
    Node* leaf = select(root);
    Node* child = leaf;

    // If the leaf node is not terminal, then choose a child
    if (!(favor = leaf->state.check()) && leaf->state.pos().size())
    {
        child = expand(leaf);
    }
    
    // Choose a random path from the child to a terminal node and get its score
    favor = simulate(child);

    // Backpropagate from the child node to the root node
    backprop(child, root, favor);
}

int MCTS::controller(const Con4 &board)
{
    // Find the node corresponding to the board
    Node* root = find(board);

    // Keep simulating paths while there are resources using multithreading if enabled
    if (MULTITHREADING)
    {
        // Add jobs to the queue
        jobs = {};
        for (int i = 0; i < RESOURCES; i++) {
            jobs.push_back(0);
        }

        // Start threads
        for (int i = 0; i < NUMTHREADS; i++) {
            threads[i] = std::thread ([&](){ while (!jobs.empty()) {
                jobs.pop_back();
                eval(root);
            }});
        }

        // Wait for threads to finish
        for (int i = 0; i < NUMTHREADS; i++) {
            threads[i].join();
        }
        
    }
    else {
        for (int n = 0; n < RESOURCES; n++) {
            eval(root);
        }
    }

    // Modified select function that chooses child node by ranking UCT
    double max = -10;
    Node* chosen = root;
    double val = 0;
    for (Node &child : chosen->children) {
        if ((val = ((double) child.wins) / child.visits) > max) {
            max = val;
            chosen = &child;
        }
    }

    // Return the last played column of the child node
    return chosen->col;
}