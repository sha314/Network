//
// Created by shahnoor on 11/10/19.
//

#ifndef NETWORK_OPERATION_H
#define NETWORK_OPERATION_H

#include "../nets/network.h"
#include "../percolation/network_percolation_v7.h"


/**
 * Calling methods of this class
 */
class PerformPercolation{
    Network_v7 * net= nullptr;
    NetworkPercolation_v7 * percolation= nullptr;



    double entropy_jump{}, order_jump{};
    vector<double> entropy, order_param; // entropy and order parameter

    string tm ;
//    string signature = percolation.get_signature();
    string signature ;
    int m_val{1}, N_size{10}, M_val{1};
    double p_val{0.5};

public:
    ~PerformPercolation() = default;
    PerformPercolation() = default;

    void set_network(string name, int m, int N);
    void set_network(string name, double p, int N);
    void set_percolation(string name, int M=1);
    void run(int ensemble_size, int reset_interval=50);
    void write_data();
    void write_jump_data();
};


#endif //NETWORK_OPERATION_H
