//
// Created by shahnoor on 11/1/19.
//

#ifndef NETWORK_NETWORK_MDA_V7_H
#define NETWORK_NETWORK_MDA_V7_H


#include "../network.h"

// MDA network
/**********
 * MDA network construction requires adjacency list
 *
 */
class NetworkMDA_v7 : public Network_v7{

    size_t _link_count;
    size_t  _node_index;
    // constant properties
    uint _m0{3}; // seed
    uint _m{2}; // number of new nodes each node comes with

    // variable properties

public:
    explicit NetworkMDA_v7(uint m0=3, uint m=1); // only seeding
    uint get_m0()const { return  _m0;}
    uint get_m() const { return  _m;}

    std::string getClassName() override {return "Network_MDA";}
    void rebuild() override ;
    void initialize(size_t N) override;

    std::string get_signature() override {
        return "???";
    }
    std::vector<std::string> get_signature_array()  override;
    void viewLocal() override; // other properties of a network that is different for different types of network
    bool grow(size_t n);
    bool grow_adjacency(size_t netowk_size);


    bool add_node_adjacency_list();
};


#endif //NETWORK_NETWORK_MDA_V7_H
