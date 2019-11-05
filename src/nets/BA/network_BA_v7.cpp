//
// Created by shahnoor on 11/1/19.
//

#include <set>
#include "network_BA_v7.h"

NetworkBA_v7::NetworkBA_v7(uint m0, uint m)
        :Network_v7()
{

    _m0 = m0;
    _m = m;
    if(_m0 <= _m){
        _m0 = 2*_m;
    }
//    cout << "m0 " << _m0 << " m " << _m << endl;
    _link_count=0;
    _node_index=0;
}

void NetworkBA_v7::initialize(size_t N) {
    N_size = N;
    degree_count.resize(N_size);
    _adjacency_list.resize(_m0);
    for(int i{}; i < _m0; ++i){
        _nodes.emplace_back(i);
        for(int j{i+1}; j < _m0; ++j){
            _network_map_A.emplace_back(i);
            _network_map_B.emplace_back(j);
            _preferentially.emplace_back(i);
            _preferentially.emplace_back(j);
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
    grow(N_size);
//    grow_adjacency(N_size);
}

void NetworkBA_v7::rebuild() {
    _link_count=0;
    _node_index=0;
    initialize(N_size);

}

std::vector<std::string> NetworkBA_v7::get_signature_array() {
    std::vector<std::string> a;
    a.emplace_back("BA");

    std::stringstream ss;
    ss << "_m0_" << _m0 << "_m_" << _m << "-";
    a.emplace_back(ss.str());
    return a;
}

void NetworkBA_v7::viewLocal() {
    cout << "preferentially {";
    for(size_t i{}; i < _preferentially.size(); ++i){
        cout << _preferentially[i] << ",";
    }
    cout << "}" << endl;
}

bool NetworkBA_v7::grow(size_t netowk_size) {
    if(netowk_size <= _node_index){
        cout << "cannot grow" << endl;
        return false;
    }

    degree_count.resize(N_size);
    while (_node_index < netowk_size){
        add_node();
    }

    return true;

}

bool NetworkBA_v7::grow_adjacency(size_t netowk_size) {
    if(netowk_size <= _node_index){
        cout << "cannot grow" << endl;
        return false;
    }
    degree_count.resize(N_size);
    _adjacency_list.resize(N_size);
    while (_node_index < netowk_size){
        add_node_adjacency_list();
    }
    return true;
}

bool NetworkBA_v7::addNode() {
//    return add_node();
    return add_node_adjacency_list();
}

bool NetworkBA_v7::add_node() {
    // first select m_nodes preferentially with no repetition
// to prevent self connection
    _nodes.emplace_back(_node_index); // add one new node
    std::set<int> m_nodes;
    size_t rnd, k;
    uniform_int_distribution<size_t> dist(0, _preferentially.size()-1);
    while (m_nodes.size() < _m) {
// since  each  new node  comes with  m  new links
        rnd = dist(_gen);
        k = _preferentially[rnd];
        m_nodes.insert(std::move(k));
    }
    _link_count += m_nodes.size();
    if(degree_count.size()>= _node_index) degree_count.resize(_node_index + 10);
// then connect the new node to the selected m_nodes
    for(int j: m_nodes){
        _network_map_A.emplace_back(_node_index);
        _network_map_B.emplace_back(j);
        _preferentially.emplace_back(_node_index);
        _preferentially.emplace_back(j);
        degree_count[_node_index] += 1;
        degree_count[j] += 1;
//        _c += 1;
    }
    ++_node_index;
    return true;
}

bool NetworkBA_v7::add_node_adjacency_list() {
    // first select m_nodes preferentially with no repetition
// to prevent self connection
    _nodes.emplace_back(_node_index); // add one new node
    std::set<int> m_nodes;
    size_t rnd, k;
    uniform_int_distribution<size_t> dist(0, _preferentially.size()-1);
    while (m_nodes.size() < _m) {
// since  each  new node  comes with  m  new links
        rnd = dist(_gen);
        k = _preferentially[rnd];
        m_nodes.insert(std::move(k));
    }
    _link_count += m_nodes.size();
    if(degree_count.size()>= _node_index) degree_count.resize(_node_index + 10);
// then connect the new node to the selected m_nodes
    for(int j: m_nodes){
        _network_map_A.emplace_back(_node_index);
        _network_map_B.emplace_back(j);
        _preferentially.emplace_back(_node_index);
        _preferentially.emplace_back(j);
        degree_count[_node_index] += 1;
        degree_count[j] += 1;
        _adjacency_list[_node_index].emplace_back(j);
        _adjacency_list[j].emplace_back(_node_index);
//        _c += 1;
    }
    ++_node_index;
    return true;
}


