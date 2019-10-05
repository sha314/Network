//
// Created by shahnoor on 5/16/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_H
#define NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_H


#include "../network_percolation.h"
#include "../network_percolation_v2.h"
#include "../network_percolation_v3.h"
#include "../network_percolation_template.h"


/***********************************************************
 * Explosive percolation on BA network
 ***********************************************************/
class NetworkBApercolationExplosive : public NetworkBApercolation{
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive() = default;
    NetworkBApercolationExplosive() = default;
    NetworkBApercolationExplosive(size_t m0, size_t m, size_t size, uint M);

    bool occupyLink();

    bool placeLink(); // link occupation

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

    /**
     * Sum rule and Product rule
     */
    size_t link_for_min_cluster_sum_rule();
    size_t link_for_min_cluster_product_rule();
    // TODO write function that take a lambda function and executes as it says
    size_t selectLink(char rule);
};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveSum : public NetworkBApercolationExplosive{

public:
    ~NetworkBApercolationExplosiveSum() = default;
    NetworkBApercolationExplosiveSum() = default;
    NetworkBApercolationExplosiveSum(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive(m0, m, size, M) {};

    bool placeLink();
    bool occupyLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_sum_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveProduct: public NetworkBApercolationExplosive{

public:
    ~NetworkBApercolationExplosiveProduct() = default;
    NetworkBApercolationExplosiveProduct() = default;
    NetworkBApercolationExplosiveProduct(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive(m0, m, size, M) {};

    bool occupyLink();
    bool placeLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_product_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};


/***********************************************************
 * Explosive percolation on BA network
 ***********************************************************/
class NetworkBApercolationExplosive_v2 : public NetworkBApercolation_v2{
private:
    // time measurement variable
    double _time_selectLink{};
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive_v2() = default;
    NetworkBApercolationExplosive_v2() = default;
    NetworkBApercolationExplosive_v2(size_t m0, size_t m, size_t size, uint M);

    bool occupyLink();

    bool placeLink(); // link occupation

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

    /**
     * Sum rule and Product rule
     */
    size_t link_for_min_cluster_sum_rule();
    size_t link_for_min_cluster_product_rule();
    // TODO write function that take a lambda function and executes as it says
    size_t selectLink(char rule);

    void time_summary();
};


/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveSum_v2 : public NetworkBApercolationExplosive_v2{

public:
    ~NetworkBApercolationExplosiveSum_v2() = default;
    NetworkBApercolationExplosiveSum_v2() = default;
    NetworkBApercolationExplosiveSum_v2(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive_v2(m0, m, size, M) {};

    bool placeLink();
    bool occupyLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_sum_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveProduct_v2: public NetworkBApercolationExplosive_v2{

public:
    ~NetworkBApercolationExplosiveProduct_v2() = default;
    NetworkBApercolationExplosiveProduct_v2() = default;
    NetworkBApercolationExplosiveProduct_v2(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive_v2(m0, m, size, M) {};

    bool occupyLink();
    bool placeLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_product_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};

//
///**
// * A universal class that can perform explosive percolation on any network
// * that is extended from the class "Network"
// *
// * Instruction for using sum rule or product rule is provided using constructor argument
// * @tparam NET : type of the network
// */
//template <class NET>
//class NetworkPercolationExplosive: public NetworkPercolation{
//public:
//    enum class ExplosiveType{SUM, PRODUCT}; // sum rule or product rule
//
//    NetworkPercolationExplosive(size_t m0, size_t m, size_t size, int M, ExplosiveType type);
//    bool occupyLink();
//    size_t selectLink();
//
//private:
//    ExplosiveType _rule;
//    int _M{1}; // number of link to choose for sum/product rule
//};
//
//template <class NET>
//NetworkPercolationExplosive<NET>::NetworkPercolationExplosive
//        (
//                size_t m0, size_t m, size_t size, int M,
//                NetworkPercolationExplosive::ExplosiveType type
//        ):NetworkPercolation(m0, m, size)
//{
//    if(M < 1){
//        std::cerr << "M must be >=1" << std::endl;
//        std::cout << "Setting M=1 to avoid errors" << std::endl;
//        M = 1;
//    }
//    _M = M;
//    _rule=type;
//}
//
//
//template <class NET>
//bool NetworkPercolationExplosive<NET>::occupyLink(){
//    size_t pos  = selectLink();  // 2018.12.29
//
//    auto retValue = occupySelectedLink(pos); // 2018.12.29
//
//    return retValue;
//}
//
///**
// * Select M links randomly and out of M links perform sum/product rule.
// * For which link sum/product rule give smallest value, occupy that link.
// * That way we will be shifting the critical point toward right
// * @tparam NET
// * @return
// */
//template <class NET>
//size_t NetworkPercolationExplosive<NET>::selectLink() {
//    size_t sz = _randomized_indices.size();
//    size_t limit = (sz + _M) % sz; //  so that limit <= _M
//    auto it = _randomized_indices.begin();
//    auto it_last = _randomized_indices.begin() + limit;
//    std::vector<uint> selected_links;
//
//    // moving it from _randomized_link_indices
//    std::move(it, it_last, std::back_inserter(selected_links));
//
//    // checking for one selectable link
//    if(_rule == ExplosiveType::SUM) {
//        int lnk{-1};
//        int sum{};
//        for (size_t i{}; i < selected_links.size(); ++i) {
//
//        }
//    }else if(_rule == ExplosiveType::PRODUCT){
//
//    }
//
//
//    // moving unselected indices to _randomized_link_indices
//    std::move(selected_links.begin(), selected_links.end(),
//              std::back_inserter(_randomized_indices));
//
//
//}


#endif //NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_H
