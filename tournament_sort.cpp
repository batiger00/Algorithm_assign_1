#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int value;
    vector<Node*> children;
    Node(int v) : value(v) {}
};


Node* tournament_play(Node* a, Node* b) {
    if (a->value <= b->value) {
        a->children.push_back(b);
        return a;
    } else {
        b->children.push_back(a);
        return b;
    }
}


Node* tournament(vector<Node*>& forest) {
    while (forest.size() > 1) {
        vector<Node*> next_round;
        for (size_t i = 0; i < forest.size(); i += 2) {
            if (i + 1 < forest.size()) {
                Node* winner = tournament_play(forest[i], forest[i + 1]);
                next_round.push_back(winner);
            } else {
                next_round.push_back(forest[i]); 
            }
        }
        forest = next_round;
    }
    return forest.empty() ? nullptr : forest[0];
}


int pop_min(Node*& tree) {
    int min_val = tree->value;
    vector<Node*> next_forest = tree->children;
    delete tree;
    tree = tournament(next_forest);
    return min_val;
}


vector<int> tournament_sort(vector<int>& input) {
    vector<Node*> forest;
    for (int v : input) {
        forest.push_back(new Node(v));
    }

    Node* tree = tournament(forest);

    vector<int> result;
    while (tree != nullptr) {
        result.push_back(pop_min(tree));
    }
    return result;
}

