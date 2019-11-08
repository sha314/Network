//
// Created by shahnoor on 11/1/19.
//

#include <sys/types.h>
#include <set>
#include "network_MDA_v7.h"

NetworkMDA_v7::NetworkMDA_v7(uint m0, uint m) {
    _m0 = m0;
    _m = m;
    if(_m0 <= _m){
        _m0 = 2*_m;
    }
//    cout << "m0 " << _m0 << " m " << _m << endl;
    _link_count=0;
    _node_index=0;
}

void NetworkMDA_v7::initialize(size_t N) {
    N_size = N;
    degree_count.resize(N_size);
    _adjacency_list.resize(_m0);
    for(int i{}; i < _m0; ++i){
        _nodes.emplace_back(i);
        for(int j{i+1}; j < _m0; ++j){
            _network_map_A.emplace_back(i);
            _network_map_B.emplace_back(j);
            degree_count[i] += 1;
            degree_count[j] += 1;
            _adjacency_list[i].emplace_back(j);
            _adjacency_list[j].emplace_back(i);
//                _c += 1;
            ++_link_count;

        }
    }
//    cout << "Links in seed " << _link_count << endl;
//    view();
    _node_index = _m0;
    // MDA network construction requires adjacency list
    grow_adjacency(N_size);
}


bool NetworkMDA_v7::grow_adjacency(size_t netowk_size) {
    if(netowk_size <= _node_index){
        cout << "cannot grow" << endl;
        return false;
    }
    degree_count.resize(N_size);
    _adjacency_list.resize(N_size);
//    view();
    while(_node_index < netowk_size){
        add_node_adjacency_list();
    }

    return true;
}

bool NetworkMDA_v7::add_node_adjacency_list() {
// first select m_nodes preferentially with no repetition
// to prevent self connection

    std::set<int> m_nodes;
    size_t rnd;
    auto r = _gen() % _nodes.size();
    auto first_chosen = _nodes[r]; // it turns out that r==_nodes[r] always

    _nodes.emplace_back(_node_index); // add one new node
    auto neighbors = _adjacency_list[first_chosen];
    uniform_int_distribution<size_t> dist(0, neighbors.size()-1);
    while (m_nodes.size() < _m) {
// since  each  new node  comes with  m  new links
        rnd = dist(_gen);
        m_nodes.insert(neighbors[rnd]);
    }
    _link_count += m_nodes.size();
    if(degree_count.size()>= _node_index) degree_count.resize(_node_index + 10);
// then connect the new node to the selected m_nodes
    for(int j: m_nodes){
        _network_map_A.emplace_back(_node_index);
        _network_map_B.emplace_back(j);
        degree_count[_node_index] += 1;
        degree_count[j] += 1;
        _adjacency_list[_node_index].emplace_back(j);
        _adjacency_list[j].emplace_back(_node_index);
//        _c += 1;
    }
    ++_node_index;
    return true;
}

void NetworkMDA_v7::rebuild() {
    _link_count=0;
    _node_index=0;
    reset();
    // MDA network construction requires adjacency list
    initialize(N_size);
}

std::vector<std::string> NetworkMDA_v7::get_signature_array() {
    std::vector<std::string> a;
    a.emplace_back("MDA");

    std::stringstream ss;
    ss << "_m0_" << _m0 << "_m_" << _m << "-";
    a.emplace_back(ss.str());
    return a;
}

void NetworkMDA_v7::viewLocal() {

}

bool NetworkMDA_v7::grow(size_t n) {
    grow_adjacency(n);
    return false;
}
