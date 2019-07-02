//
// Created by shahnoor on 7/1/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_V5_H
#define NETWORK_NETWORK_PERCOLATION_V5_H

#include "../nets/network.h"
#include "../nets/link/link.h"
#include "cluster/cluster_v2.h"
#include "cluster/cluster_v3.h"
#include "../nets/BA/network_BA_v2.h"


/**
 * class : NetworkBApercolation_v5
 * used classes : NetworkBA_v2
 *
 */
class NetworkBApercolation_v5 { // TODO
    size_t _m0, _m;

    double log_1_by_size{}; // to make calculations easier

    //time measurement variables
    double _time_placeSelectedLink{};

    // counting. how many times one of the four options is used
    int manage_cluster_v2_a{-1}, manage_cluster_v2_b{-1}, manage_cluster_v2_c{-1}, manage_cluster_v2_d{-1};
protected:
    NetworkBA_v2 _network_frame;

};



#endif //NETWORK_NETWORK_PERCOLATION_V5_H
