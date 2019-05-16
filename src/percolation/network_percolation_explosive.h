//
// Created by shahnoor on 5/16/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_H
#define NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_H


#include "network_percolation.h"

/**
 * A universal class that can perform explosive percolation on any network
 * that is extended from the class "Network"
 *
 * Instruction for using sum rule or product rule is provided using constructor argument
 * @tparam NET : type of the network
 */
template <class NET>
class NetworkPercolationExplosive: public NetworkPercolation{
public:
    enum class ExplosiveType{SUM, PRODUCT}; // sum rule or product rule

    NetworkPercolationExplosive(size_t m0, size_t m, size_t size, int M, ExplosiveType type);
    bool occupyLink();
    size_t selectLink();

private:
    ExplosiveType _rule;
    int _M{1}; // number of link to choose for sum/product rule
};

template <class NET>
NetworkPercolationExplosive<NET>::NetworkPercolationExplosive
        (
                size_t m0, size_t m, size_t size, int M,
                NetworkPercolationExplosive::ExplosiveType type
        ):NetworkPercolation(m0, m, size)
{
    if(M < 1){
        std::cerr << "M must be >=1" << std::endl;
        std::cout << "Setting M=1 to avoid errors" << std::endl;
        M = 1;
    }
    _M = M;
    _rule=type;
}


template <class NET>
bool NetworkPercolationExplosive<NET>::occupyLink(){
    size_t pos  = selectLink();  // 2018.12.29

    auto retValue = occupySelectedLink(pos); // 2018.12.29

    return retValue;
}

/**
 * Select M links randomly and out of M links perform sum/product rule.
 * For which link sum/product rule give smallest value, occupy that link.
 * That way we will be shifting the critical point toward right
 * @tparam NET
 * @return
 */
template <class NET>
size_t NetworkPercolationExplosive<NET>::selectLink() {
    size_t sz = _randomized_indices.size();
    size_t limit = (sz + _M) % sz; //  so that limit <= _M
    auto it = _randomized_indices.begin();
    auto it_last = _randomized_indices.begin() + limit;
    std::vector<uint> selected_links;

    // moving it from _randomized_indices
    std::move(it, it_last, std::back_inserter(selected_links));

    // checking for one selectable link
    if(_rule == ExplosiveType::SUM) {
        int lnk{-1};
        int sum{};
        for (size_t i{}; i < selected_links.size(); ++i) {

        }
    }else if(_rule == ExplosiveType::PRODUCT){

    }


    // moving unselected indices to _randomized_indices
    std::move(selected_links.begin(), selected_links.end(),
              std::back_inserter(_randomized_indices));


}


#endif //NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_H
