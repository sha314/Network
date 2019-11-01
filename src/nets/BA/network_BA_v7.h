//
// Created by shahnoor on 11/1/19.
//

#ifndef NETWORK_NETWORK_BA_V7_H
#define NETWORK_NETWORK_BA_V7_H


#include "../network.h"

// Barabasi Albert network
class NetworkBA_v7 : Network_v7{

    // constant properties
    uint _m0{3}; // seed
    uint _m{2}; // number of new nodes each node comes with

    // variable properties

    size_t _N_max;
    /*
     * Very useful for preferential attachment
     */
    std::vector<uint> _preferentially;
public:
    NetworkBA_v7(size_t m0=3, size_t m=1); // only seeding
    void clear_preferentially(){_preferentially.clear();}
    uint get_m0()const { return  _m0;}
    uint get_m() const { return  _m;}
    virtual std::string getClassName(){return "Network_BA";}
    void rebuild();
};

#endif //NETWORK_NETWORK_BA_V7_H
