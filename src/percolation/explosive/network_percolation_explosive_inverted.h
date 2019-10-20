//
// Created by shahnoor on 10/5/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_INVERTED_H
#define NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_INVERTED_H

#include "network_percolation_explosive_v3.h"
#include "network_percolation_explosive_v5.h"

/**********************
 * Regular explosive percolation using sum rule or product rule up to a certain relative link density (say tc)
 * after that point we do the opposite.
 *
 * before tc we try to choose link which minimizes cluster size
 * after  tc we try to choose link which maximizes cluster size
 *
 **/

/**
 *
 */
class NetworkBApercolationExplosive_Inverted_v3 : public NetworkBApercolationExplosive_v3{
    std::vector<double> tc_values={0,0,0.733484,0.888528,0.946605,0.972533,0.985232,0.991819,0.99538,0.997349,0.998465};

public:
    NetworkBApercolationExplosive_Inverted_v3(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive_v3(m0, m, size, M){
        cout << "class NetworkBApercolationExplosive_Inverted_v3" << endl;
    };


    bool occupyLink() override;

    std::string get_signature() {
        std::stringstream ss;
        ss << "network_BA_percolation_explosive_inverted_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m()
           << "_size_" <<  _network_size << "_M_" << _M_link << '-';
        return ss.str();
    }
    std::string getClassName(){return "NetworkBApercolationExplosive_Inverted_v3";}
};


/**
 *
 */
class NetworkBApercolationExplosive_Inverted_v5 : public NetworkBApercolationExplosive_v5{
    std::vector<double> tc_values={0,0,0.733484,0.888528,0.946605,0.972533,0.985232,0.991819,0.99538,0.997349,0.998465};
public:
    NetworkBApercolationExplosive_Inverted_v5(size_t m0, size_t m, size_t size, uint M)
    : NetworkBApercolationExplosive_v5(m0, m, size, M){
        std::cout << "class NetworkBApercolationExplosive_Inverted_v5" << std::endl;
    };


    bool occupyLink() override;

    std::string get_signature() {
        std::stringstream ss;
        ss << "network_BA_percolation_explosive_inverted_m0_";
        ss << get_m0() << "_m_" << get_m()
           << "_size_" << _network_size << "_M_" << _M_link << '-';
        return ss.str();
    }
    std::string getClassName(){return "NetworkBApercolationExplosive_Inverted_v5";}

};

#endif //NETWORK_NETWORK_PERCOLATION_EXPLOSIVE_INVERTED_H
