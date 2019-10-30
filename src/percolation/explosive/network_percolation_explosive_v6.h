//
// Created by shahnoor on 10/20/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V6_H
#define NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V6_H


#include "../network_percolation_v6.h"

/**
 * version 5 : inherits NetworkBApercolation_v5
 *
 * with m, N, M = 2, 1000000, 9 => time elapsed = ??? sec
 */
class NetworkBApercolationExplosive_v6 : public NetworkBApercolation_v6{
private:
    // time measurement variable
    double _time_selectLink{}, _time_link_for_min_cluster_sum_rule {};
//    size_t _count_link_for_min_cluster_sum_rule_a{}, _count_link_for_min_cluster_sum_rule_b{}, _count_link_for_min_cluster_sum_rule_c{};
//    std::vector<long> M_sum_products;
//    std::vector<int>  M_link_indices;
protected:
    uint _M_link{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive_v6() = default;
    NetworkBApercolationExplosive_v6() = default;
    NetworkBApercolationExplosive_v6(size_t m0, size_t m, size_t size, uint M);
    void reset(int i) override;
    bool occupyLink() override;

    std::string get_signature() override {
        std::stringstream ss;
        ss << getClassName()
           << "_m0_" << get_m0()
           << "_m_" << get_m()
           << "_size_" << _network_size
           << "_M_" << _M_link << '-';
        return ss.str();
    }
    std::string getClassName() override {return "NetworkBApercolationExplosive_v6";}

    /**
     * Sum rule and Product rule
     */
    uint link_for_min_cluster_sum_product_v2(size_t start_at); // both sum and product rule and randomly chooses from _randomized_link_indices
    uint link_for_min_cluster_sum_product_v3_adaptive(double tc, size_t start_at);

    uint selectLink_v2();

    void summary();

    void swap_randomized_index(size_t link, size_t count);

    void replace_randomized_index(size_t link, size_t count);
};

#endif //NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_V6_H
