#include "ai.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <queue>


double MCTS::UCT(Node &node)
{
    return ((double) node.wins) / node.visits + 0.7 * std::sqrt(std::log(node.parent->visits.load()) / node.visits);
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
        newNode = random(newNode->children);
    }
    // Return the game winner
    return newNode->state.check();
}

void MCTS::backprop(Node* node, Node* root, const int favor)
{
    Node* newNode = node;
    while (newNode->state != root->state)
    {
        newNode->wins += (2 * (newNode->turn ^ (favor - 1)) + !favor) / 2;
        newNode = newNode->parent;
    }
    newNode->wins += (2 * (newNode->turn ^ (favor - 1)) + !favor) / 2;
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

void *MCTS::eval(Node* root)
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
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Node* root = find(board);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;

    // Keep simulating paths while there are resources
    for (int n = 0; n < 100; n++)
    {
        eval(root);
    }

    // Modified select function that chooses child node by ranking UCT
    double max = 0;
    Node* chosen = root;
    double val = 0;
    for (Node &child : chosen->children)
    {
        if ((val = ((double) child.wins) / child.visits) > max)
        {
            max = val;
            chosen = &child;
        }
    }

    // Return the last played column of the child node
    return chosen->col;
}