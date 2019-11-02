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
    for(int i{}; i < _m0; ++i){
        _nodes.emplace_back(i);
        for(int j{i+1}; j < _m0; ++j){
            _network_map_A.emplace_back(i);
            _network_map_B.emplace_back(j);
            _preferentially.emplace_back(i);
            _preferentially.emplace_back(j);
            degree_count[i] += 1;
            degree_count[j] += 1;
//                _c += 1;
            ++_link_count;

        }
    }
//    cout << "Links in seed " << _link_count << endl;
    _node_index = _m0;
    grow(N_size);
}

void NetworkBA_v7::rebuild() {
    _link_count=0;
    _node_index=0;
    initialize(N_size);

}

std::vector<std::string> NetworkBA_v7::get_signature_array() {
    return vector<string>();
}

void NetworkBA_v7::viewLocal() {

}

bool NetworkBA_v7::grow(size_t netowk_size) {
    if(netowk_size <= _N_tmp){
        cout << "cannot grow" << endl;
        return false;
    }
    _N_tmp = _m0;
    degree_count.resize(N_size);
//    view();
    for(size_t i{_N_tmp}; i < netowk_size; ++i){
        addNode();
//        cout << i << "-th node" << endl;
//        view();
    }
//    view();
    _N_tmp = netowk_size;

    return true;

}

bool NetworkBA_v7::addNode() {
// first select m_nodes preferentially with no repetition
// to prevent self connection
    _nodes.emplace_back(_N_tmp); // add one new node
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
    if(degree_count.size()>= _N_tmp) degree_count.resize(_N_tmp + 10);
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
    ++_N_tmp; // increase network size by 1 node
    return true;
}


