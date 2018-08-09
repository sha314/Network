//
// Created by shahnoor on 8/9/18.
//

#include <iostream>
#include "node.h"

using namespace std;


Node::Node(size_t id) {
    _id = id;
}

void Node::add_neighbor(Node n) {
    _connected_nodes.push_back(n.get_id());
}

void Node::add_neighbor(size_t n) {
    _connected_nodes.push_back(n);
}

void Node::add_neighbor_checked(Node n) {
    size_t i = n.get_id();
    for(size_t k{}; k < _connected_nodes.size(); ++k){
        if(i == _connected_nodes[k]){
            cerr << "Already a member : line " << __LINE__ << endl;
        }
    }
    _connected_nodes.push_back(i);

}

void Node::add_neighbor_checked(size_t n) {
    for(size_t k{}; k < _connected_nodes.size(); ++k){
        if(n == _connected_nodes[k]){
            cerr << "Already a member : line " << __LINE__ << endl;
        }
    }
    _connected_nodes.push_back(n);
}

std::ostream& operator<<(std::ostream& os, const Node& node){
    return os << node.get_id();
}
