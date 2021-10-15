//
// Created by shahnoor on 10/14/21.
//

#ifndef NETWORK_NETWORK_PERCOLATION_V7_FLIPPER_H
#define NETWORK_NETWORK_PERCOLATION_V7_FLIPPER_H


#include <climits>
#include "network_percolation_v7.h"

class NetworkPercolationExplosive_v7_Flipper : public NetworkPercolationExplosive_v7{
    long threshold=2;

public:

    std::string getClassName() override {return "NetworkPercolationExplosive_v7_Flipper";}

    NetworkPercolationExplosive_v7_Flipper(Network_v7 *net, int M)
            :NetworkPercolationExplosive_v7(net,  M)
    {
        threshold = net->getNodeCount() / 2;
    }

    bool cluster_extremizing_condition(long n_nodes, long prod_sum) const {
#ifdef DEBUG_FLAG
        cout << "NetworkPercolationExplosive_v7_Flipper::cluster_extremizing_condition" << endl;
        if(largestClusterSize() > threshold){
            cout << "fractional number of links occupied " << double(occupied_link_count)/_network_size << endl;
            exit(-1);
        }
#endif
//        if (largestClusterSize() <= threshold) return n_nodes < prod_sum;
//        else return n_nodes > prod_sum;

        return (largestClusterSize() <= threshold) && (n_nodes < prod_sum);

    }



};

#endif //NETWORK_NETWORK_PERCOLATION_V7_FLIPPER_H
