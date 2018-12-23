//
// Created by shahnoor on 12/24/18.
//

#include "network.h"

using namespace std;


NetworkMDA::NetworkMDA() {}

void NetworkMDA::initialize() {
    Network::initialize();
}

void NetworkMDA::add_node_v0() {
    uint sz = _nodes.size(); // new node id

    // select m nodes using MDA rule to connect with
    for(size_t i{}; i < _m; ++i){
        // randomly select one node, the mediator
        auto r = _random_generator() % sz;
        // get the list of the nodes connected to the mediator
        auto neighbors = _nodes[r].get_neighbors();
        // select one of the neighbor to connect with
        auto r2 = _random_generator() % neighbors.size();
        _m_links[i] = neighbors[r2];
    }
    // todo what if I select the same mediator again or
    // todo what if I select different mediator but ultimately leads to the same node

}

