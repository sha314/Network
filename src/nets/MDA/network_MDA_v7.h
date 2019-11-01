//
// Created by shahnoor on 11/1/19.
//

#ifndef NETWORK_NETWORK_MDA_V7_H
#define NETWORK_NETWORK_MDA_V7_H


#include "../network.h"

// MDA network
class NetworkMDA_v7 : Network_v7{

public:
    virtual std::string getClassName(){return "Network_MDA";}
    void rebuild();
};


#endif //NETWORK_NETWORK_MDA_V7_H
