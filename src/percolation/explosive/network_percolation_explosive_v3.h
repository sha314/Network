//
// Created by shahnoor on 9/29/2019.
//

#ifndef NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V3_H
#define NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V3_H


#include "../network_percolation_v3.h"

/**
 * version 3 : inherits NetworkBApercolation_v3
 *
 * with m, N, M = 2, 1000000, 9 => time elapsed = 18.774518 sec
 */
class NetworkBApercolationExplosive_v3 : public NetworkBApercolation_v3{
private:
    // time measurement variable
    double _time_selectLink{}, _time_link_for_min_cluster_sum_rule {};
    size_t _count_link_for_min_cluster_sum_rule_a{}, _count_link_for_min_cluster_sum_rule_b{}, _count_link_for_min_cluster_sum_rule_c{};
protected:
    uint _M_link{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive_v3() = default;
    NetworkBApercolationExplosive_v3() = default;
    NetworkBApercolationExplosive_v3(size_t m0, size_t m, size_t size, uint M);

    bool occupyLink();

    bool placeLink(); // link occupation

    std::string get_signature() {
        std::stringstream ss;
        ss << "network_BA_percolation_explosive_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "_M_" << _M_link << '-';
        return ss.str();
    }
    virtual std::string getClassName(){return "NetworkBApercolationExplosive_v3";}
    /**
     * Sum rule and Product rule
     */
    size_t link_for_min_cluster_sum_rule(size_t start_at=0);
    size_t link_for_min_cluster_product_rule(size_t start_at=0);
    size_t link_for_min_cluster_sum_product(size_t start_at=0); // both sum and product rule
    size_t link_for_min_cluster_sum_product_v2(size_t start_at=0); // both sum and product rule and randomly chooses from _randomized_link_indices
    size_t link_for_min_cluster_sum_product_v3_adaptive(double tc, size_t start_at);
    size_t selectLink(char rule);
    size_t selectLink_v2();

    void time_summary();
};

#endif //NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V3_H
