//
// Created by shahnoor on 10/29/19.
//

#ifndef NETWORK_NETWORK_ER_H
#define NETWORK_NETWORK_ER_H

#include <vector>
#include <iostream>
#include <random>
#include <sstream>
#include "../network.h"




// Erdos Renyi network
/**

Erdos-Renyi model
Randomize edge presence/absence
1. Two parameters
    Number of nodes: n
    Probability that an edge is present: p
2. For each of the n(n-1)/2
 possible edges in the network, flip a
(biased) coin that comes up “heads” with probability
p
3a. If coin flip is “heads”, then add the edge to the network
3b. If coin flip is “tails”, then don’t add the edge to the network

Also known as the “G(n,p)"
 model” (graph on n nodes with probability p)
 */
class NetworkER_v7 : public Network_v7{
    double p_value{};


    int _node_index; // 1 less _N_size
    size_t _link_count{}; // number of links




    size_t _c{};

protected:


private:
    bool build(size_t network_size);
    bool build_with_adjacency_list(size_t network_size);
public:
    ~NetworkER_v7() override = default;

    explicit NetworkER_v7(double p);

    void rebuild() override ;
    void initialize(size_t N) override;


    std::string get_signature() override {
        std::stringstream ss;
        ss << getClassName() ;
        ss << "_N_" << N_size << "_p_" << p_value << "-";
        return ss.str();
    }
    std::string getClassName() override {return "Network_ER";}

    void viewLocal() override;


};
#endif //NETWORK_NETWORK_ER_H
