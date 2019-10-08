//
// Created by shahnoor on 10/5/19.
//

#include "network_percolation_explosive_inverted.h"

/****
 *  NetworkBApercolationExplosive_Inverted_v3 definitions
 */

bool NetworkBApercolationExplosive_Inverted_v3::occupyLink() {
    if (occupied_link_count >= linkCount()){
        return false;
    }
    // link selection
    double tc = tc_values[_M_link];
    size_t pos = link_for_min_cluster_sum_product_v3_adaptive(tc, occupied_link_count);
    return NetworkBApercolation_v3::placeSelectedLink(pos);
}


/*****
 * NetworkBApercolationExplosive_Inverted_v5 definitions
 */

bool NetworkBApercolationExplosive_Inverted_v5::occupyLink() {
    if (occupied_link_count >= linkCount()){
        return false;
    }
    // link selection
    double tc = tc_values[_M_link];
    size_t pos = link_for_min_cluster_sum_product_v3_adaptive(tc, occupied_link_count);
    return NetworkBApercolation_v5::placeSelectedLink(pos);
}
