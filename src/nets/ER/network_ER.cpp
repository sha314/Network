//
// Created by shahnoor on 10/29/19.
//

#include "network_ER.h"
#include <iostream>

using namespace std;

/*************************
 * class Network_v2
 ***************************/
void Network_v2::setRandomState(size_t seed, bool g) {
    _random_state = seed;
    if(g){
        std::random_device rd;
        _random_state = rd();
    }
    _gen.seed(_random_state);
    cout << "random seed Network_v2 " << _random_state << endl;
}

void Network_v2::view() {
    cout << "Network_v2::view __LINE__" << __LINE__ << endl;
    for(size_t i{}; i < _network_map_A.size(); ++i){
        cout << "Link[" << i << "] => " << _network_map_A[i] << " - " << _network_map_B[i] << endl;
    }
    cout << "Netowrk size " << _nodes.size() << endl;
    cout << "Link count " << _network_map_A.size() << endl;
    cout << "Max link " << _nodes.size() *( _nodes.size()-1)/2 << endl;

}

void Network_v2::viewAdjacencyList() {
    for(size_t i{}; i < _adjacency_list.size(); ++i){
        cout << "node[" << i << "]={";
        for(size_t j{}; j < _adjacency_list[i].size(); ++j){
            cout << _adjacency_list[i][j] << ",";
        }
        cout << "}" << endl;
    }
}


/*****************************
 * Erdos Renyi network
 * class Network_ER
 *****************************/
Network_ER::Network_ER(size_t N, double p) : Network_v2(){
    N_size = N;
    p_value = p;
}


bool Network_ER::build(size_t network_size) {
    degree_count.resize(network_size);
    double p{};
    std::uniform_real_distribution<double> random(0, 1);
    for(int i{}; i < network_size; ++i){
        _nodes.emplace_back(i);
        for(int j{i+1}; j < network_size; ++j){
            p = random(_gen);
            if(p <= p_value){
                _network_map_A.emplace_back(i);
                _network_map_B.emplace_back(j);
                degree_count[i] += 1;
                degree_count[j] += 1;
            }
        }
    }
    return false;
}

bool Network_ER::build_with_adjacency_list(size_t network_size) {
    degree_count.resize(network_size);
    _adjacency_list.resize(network_size);
    double p{};
    std::uniform_real_distribution<double> random(0, 1);
    for(int i{}; i < network_size; ++i){
        _nodes.emplace_back(i);
        for(int j{i+1}; j < network_size; ++j){
            p = random(_gen);
            if(p <= p_value){
                _network_map_A.emplace_back(i);
                _network_map_B.emplace_back(j);
                degree_count[i] += 1;
                degree_count[j] += 1;
                _adjacency_list[i].emplace_back(j);
                _adjacency_list[j].emplace_back(i);
            }
        }
    }
    return false;
}

void Network_ER::initialize() {
//    build(N_size);
    build_with_adjacency_list(N_size);
}

void Network_ER::rebuild() {
    initialize();
}

void Network_ER::viewLocal() {
    cout << "G(N,p) : N="  << N_size << ",p=" << p_value << endl;
    cout << "Connection number peaked around " << p_value * N_size*(N_size - 1)/2 << endl;
}



