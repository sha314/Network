//
// Created by shahnoor on 7/1/19.
//

#include <iostream>
#include <set>
#include "network_BA_v2.h"

using namespace std;



bool NetworkBA_v2::grow(size_t netowk_size) {
    if(netowk_size <= _N){
        cout << "cannot grow" << endl;
        return false;
    }
    setMaxNetworkSize(netowk_size);
    for(size_t i{_N}; i < netowk_size; ++i){
        addNode();
    }
    N = netowk_size;

    return true;
}

NetworkBA_v2::NetworkBA_v2(size_t m0, size_t m) {
    _m = m;
    _m0 = m0;
    if(m0 < m){
        _m0 += m;
    }
    N = _m0;
    _N_max = _N;
    _node_index = _N-1;
    initialize();
}

/**
 * Initialize the network based on
 */
void NetworkBA_v2::initialize() {
    degree_count.resize(_m0);
    for(int i{}; i < _m0; ++i){
        for(int j{}; j < _m0; ++j){
            if(j > i){
                network_map_A.emplace_back(i);
                network_map_B.emplace_back(j);
                _preferentially.emplace_back(i);
                _preferentially.emplace_back(j);
                degree_count[i] += 1;
                degree_count[j] += 1;
                _c += 1;
                ++_link_count;

            }
        }
    }
    _node_index = _m0;
}

bool NetworkBA_v2::addNode() {
//    add_node_v1();
    add_node_v2();
    return true;
}

/**
 * if _m is greater than unity then there is a chance of repetition of links, i.e., i-the elements
 * of netowrk map A and B and j-the elements of netowrk map A and B.
 * There is no filtering mechanism
 * @return
 */
bool NetworkBA_v2::add_node_v1() {
// version 1 : chances of repetition when m is large
    _nodes.emplace_back(_N);
    vector<int> tmp;
    size_t rnd;
    if(degree_count.size()>= _N) degree_count.resize(_N + 10);
    for (size_t k{}; k <= _m; ++k) {
// # each    node    comes    with    two    new link
        rnd = _random() % _preferentially.size();
        int j = _preferentially[rnd];
        network_map_A.emplace_back(_node_index);
        network_map_B.emplace_back(j);
//# to prevent self connection, don't just add directly to preferentially.
//# store it in a temporary location first
        tmp.emplace_back(_node_index);
        tmp.emplace_back(j);
        degree_count[_N] += 1;
        degree_count[j] += 1;
        ++_c;
        ++_link_count;
    }
    _preferentially.insert(_preferentially.end(),
                           tmp.begin(), tmp.end());
    ++_node_index;
    ++N; // increase network size by 1 node
    return true;
}



/**
 * No chance of repeted link. Since _m nodes are selected first. Then added to required lists.
 * @return
 */
bool NetworkBA_v2::add_node_v2() {
// first select m_nodes preferentially with no repetition
// to prevent self connection
    _nodes.emplace_back(_N); // add one new node
    std::set<int> m_nodes;
    size_t rnd, k;
    while (m_nodes.size() < _m) {
// since  each  new node  comes with  m  new links
        rnd = _random() % _preferentially.size();
        k = _preferentially[rnd];
        m_nodes.insert(std::move(k));
    }
    _link_count += m_nodes.size();
    if(degree_count.size()>= _N) degree_count.resize(_N + 10);
// then connect the new node to the selected m_nodes
    for(int j: m_nodes){
        network_map_A.emplace_back(_node_index);
        network_map_B.emplace_back(j);
        _preferentially.emplace_back(_node_index);
        _preferentially.emplace_back(j);
        degree_count[_node_index] += 1;
        degree_count[j] += 1;
        _c += 1;
    }
    ++_node_index;
    ++N; // increase network size by 1 node
    return true;
}

/**
 *
 * @param seed
 * @param g
 */
void NetworkBA_v2::setRandomState(size_t seed, bool g) {
    _random_state = seed;
    if(g){
        std::random_device rd;
        _random_state = rd();
    }
    _random.seed(_random_state);
}

std::vector<double> NetworkBA_v2::degreeDistribution() {
    double total_degree{};
    for(size_t i{}; degree_count.size(); ++i){
        total_degree += degree_count[i];
    }
    vector<double> distro(degree_count.size());
    for(size_t i{}; degree_count.size(); ++i){
        distro[i] = degree_count[i] / total_degree;
    }
    return distro;
}

void NetworkBA_v2::view() {
    cout << "NetworkBA_v2::view __LINE__" << __LINE__ << endl;
    for(size_t i{}; i < network_map_A.size(); ++i){
        cout << network_map_A[i] << " - " << network_map_B[i] << endl;
    }
    cout << "Netowrk size " << _N << " max size " << _N_max << endl;

}

void NetworkBA_v2::setMaxNetworkSize(size_t N) {
    if(_N_max > N){cout << "cannot initiate with smaller size " << __LINE__ << endl;}
    _N_max = N;
    degree_count.resize(_N_max);
}

size_t NetworkBA_v2::getBlankSize() {
    return _N_max - _N;
}

bool NetworkBA_v2::spaceAvailable(){
    return _N_max > _N;
}
