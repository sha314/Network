//
// Created by shahnoor on 11/1/19.
//

#ifndef NETWORK_NETWORK_MDA_H
#define NETWORK_NETWORK_MDA_H

/***
 * The Mediation-Driven Attachment (MDA) network
 *
 * The process:
 *  Randomly choose a node and name it "N1" say
 *  and then connect the new node to the neighbors of the "N1" node
 *  repeate the process
 *
 *  m : number of links to connect at each step
 *  N : network size or number of nodes
 *  T : time required to construct the network
 *  m   N       T
 *  1   1000000 0.632 sec
 *  2   1000000 0.757 sec
 *  10  1000000 1.443 sec
 *  50  1000000 4.417 sec
 */
class NetworkMDA : public Network{

    void initialize();
    void add_node_v0();
    void add_node_v1();

    void connect_with_m_nodes_v0(uint sz);
public:
    double time{};

    virtual ~NetworkMDA() = default;
    NetworkMDA() = default;
    NetworkMDA(size_t m0, size_t m):Network(m0, m) {
        std::cout << "MDA network" << std::endl;
    }

    // methods

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_MDA_m0_";
        ss << _m0 << "_m_" << _m << "-";
        return ss.str();
    }

    void addNode();


    void select_m_links_v1(uint sz) ;

    void connect_with_m_nodes_v1(uint sz);
};



#endif //NETWORK_NETWORK_MDA_H
