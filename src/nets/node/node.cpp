//
// Created by shahnoor on 8/9/18.
//

#include <iostream>
#include "node.h"

using namespace std;



void Node::add_neighbor(Node n) {
    _connected_nodes.push_back(n.get_id());
}

void Node::addNeighbor(uint n) {
    _connected_nodes.push_back(n);
}

void Node::add_neighbor_checked(Node n) {
    uint i = n.get_id();
    for(uint k{}; k < _connected_nodes.size(); ++k){
        if(i == _connected_nodes[k]){
            cerr << "Already a member : line " << __LINE__ << endl;
        }
    }
    _connected_nodes.push_back(i);

}

void Node::add_neighbor_checked(uint n) {
    for(uint k{}; k < _connected_nodes.size(); ++k){
        if(n == _connected_nodes[k]){
            cerr << "Already a member : line " << __LINE__ << endl;
        }
    }
    _connected_nodes.push_back(n);
}

double Node::size_in_MB() {
    double sz = 0;

    sz += 2* sizeof(uint) + sizeof(int) + sizeof(std::vector<uint>);
    sz += sizeof(uint) * _connected_nodes.capacity();

    return sz / (1024*1024);
}

std::ostream& operator<<(std::ostream& os, const Node& node){
    return os << node.get_id();
}
