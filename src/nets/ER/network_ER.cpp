//
// Created by shahnoor on 10/29/19.
//

#include "network_ER.h"
#include <iostream>

using namespace std;


/*****************************
 * Erdos Renyi network
 * class Network_ER
 *****************************/
NetworkER_v7::NetworkER_v7(double p) : Network_v7(){
//    N_size = N;
    p_value = p;
}


bool NetworkER_v7::build(size_t network_size) {
    degree_count.resize(network_size);
    double p{};
    std::uniform_real_distribution<double> random(0, 1);
    for(int i{}; i < network_size; ++i){
        _nodes.emplace_back(i);
        for(int j{i+1}; j < network_size; ++j){
            p = random(_gen);
//            cout << p << endl;
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

bool NetworkER_v7::build_with_adjacency_list(size_t network_size) {
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

void NetworkER_v7::initialize(size_t N) {
    if (N_size > 0 && N != N_size){
        cout << "p value was set for different network size";
        N_size = N;
        size_t M = maxEdges(N_size);
        p_value = edges_/double(M);
    }else{
        N_size = N;
    }
//    cout << "p_value  " << p_value  << endl;
    build(N_size);
//    build_with_adjacency_list(N_size);
}

void NetworkER_v7::rebuild() {
    initialize(N_size);
}

void NetworkER_v7::viewLocal() {
    cout << "G(N,p) : N="  << N_size << ",p=" << p_value << endl;
    cout << "Connection number peaked around " << p_value * N_size*(N_size - 1)/2 << endl;
}

NetworkER_v7::NetworkER_v7(size_t N_size, size_t edges) {
    size_t M = maxEdges(N_size);
    this->N_size = N_size;
    edges_ = edges;
    if(edges >= M){
        cout << "We got a problem" << endl;
        p_value = 0.5;
    }else{
        p_value = edges_/double(M);
    }

}



