//
// Created by shahnoor on 12/24/18.
//

#include "../network.h"
#include "network_MDA.h"

using namespace std;



void NetworkMDA::initialize() {
    Network::initialize();
}

void NetworkMDA::add_node_v0() {
    uint sz = _nodes.size(); // new node id

    std::unordered_set<uint> selected_links;
    // select m nodes using MDA rule to connect with
    cout << "mediator :{";
    if(selected_links.size() < _m){
        // randomly select one node, the mediator
        auto r = _random() % sz;
        cout << r << ',';
        // get the list of the nodes connected to the mediator
        auto neighbors = _nodes[r].get_neighbors();
        // select one of the neighbor to connect with
        auto r2 = _random() % neighbors.size();
        selected_links.insert(neighbors[r2]);

        // what if I select the same mediator again or
        // what if I select different mediator but ultimately leads to the same node
        // solution -> use set to avoid repetition
    }
    cout << '}' << endl;
    // printing selected links
    cout << "selected nodes to connect with :{";
    for(auto itr = selected_links.begin(); itr != selected_links.end(); ++itr){
        cout << *itr << ",";
    }
    cout << '}' << endl;

    // inserting selected links to the vector
    _m_nodes.clear();
    _m_nodes.insert(_m_nodes.end(), selected_links.begin(), selected_links.end());

    // adding new node
    _nodes.push_back({sz});
    connect_with_m_nodes_v0(sz);
}

void NetworkMDA::add_node_v1() {
    uint sz = _nodes.size(); // new node id

    std::unordered_set<uint> selected_links;
    // select m nodes using MDA rule to connect with
    if(selected_links.size() < _m){
        // randomly select one node, the mediator
        auto r = _random() % sz;
        // get the list of the nodes connected to the mediator
        auto neighbors = _nodes[r].get_neighbors();
        // select one of the neighbor to connect with
        auto r2 = _random() % neighbors.size();
        selected_links.insert(neighbors[r2]);

        // what if I select the same mediator again or
        // what if I select different mediator but ultimately leads to the same node
        // solution -> use set to avoid repetition
    }

    // inserting selected links to the vector
    _m_nodes.clear();
    _m_nodes.insert(_m_nodes.end(), selected_links.begin(), selected_links.end());

    // adding new node
    _nodes.push_back({sz});
    connect_with_m_nodes_v0(sz);
}

/**
 *
 * @param sz : id of the new node that is to be connected
 */
void NetworkMDA::connect_with_m_nodes_v0(uint sz) {
    uint i{};

    size_t link_old_size = _links.size();
    _links.resize(link_old_size + _m);
    for(size_t k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_nodes[k];
        _nodes[sz].addNeighbor(i);
        _nodes[i].addNeighbor(sz);

        _total_degree += 2; // one link  increases total degree by 2.
        _links[link_old_size + k] = Link(sz, i);
    }
}

void NetworkMDA::addNode() {
//    add_node_v0();
    add_node_v1();
}

