//
// Created by shahnoor on 9/29/2019.
//

#ifndef NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V5_H
#define NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V5_H

#include "../network_percolation_v5.h"


/**
 * version 5 : inherits NetworkBApercolation_v5
 *
 * with m, N, M = 2, 1000000, 9 => time elapsed = ??? sec
 */
class NetworkBApercolationExplosive_v5 : public NetworkBApercolation_v5{
private:
    // time measurement variable
    double _time_selectLink{}, _time_link_for_min_cluster_sum_rule {};
    size_t _count_link_for_min_cluster_sum_rule_a{}, _count_link_for_min_cluster_sum_rule_b{}, _count_link_for_min_cluster_sum_rule_c{};
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive_v5() = default;
    NetworkBApercolationExplosive_v5() = default;
    NetworkBApercolationExplosive_v5(size_t m0, size_t m, size_t size, uint M);

    bool occupyLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "network_BA_percolation_explosive_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _N_size << "_M_" << _M << '-';
        return ss.str();
    }
    std::string getClassName(){return "NetworkBApercolationExplosive_v5";}

    /**
     * Sum rule and Product rule
     */
    uint link_for_min_cluster_sum_product_v2(size_t start_at=0); // both sum and product rule and randomly chooses from _randomized_link_indices
    uint link_for_min_cluster_sum_product_v3_adaptive(double tc, size_t start_at);
    uint selectLink_v2();

    void time_summary();
};

#endif //NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V5_H
