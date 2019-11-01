//
// Created by shahnoor on 11/1/19.
//

#ifndef NETWORK_NETWORK_BA_H
#define NETWORK_NETWORK_BA_H

#include "../network.h"

/****************************************
 * Barabasi-Albert (BA) Network model
 *
 */
class NetworkBA : public Network{
    double time_select_m_nodes_preferentially_v3{};
    double time_connect_with_m_nodes_v3{};
    size_t _N_size{}; // current network size
    // holds the index of node.
    // number of neighborCount the nodes have is the number of repetition of that index
    // helps efficiently selecting nodes preferentially
    std::vector<uint> _preferentially;
    bool _static{false};

    // methods
    [[deprecated("Only able to calculate for m=1")]]
    void add_node_v0();
    void add_node_v1();
    void add_node_v2();
    void add_node_v3();

    void select_m_nodes_preferentially_v1(uint sz) ;
    void select_m_nodes_preferentially_v2(uint sz) ;
    void select_m_nodes_preferentially_v3(uint sz) ;
    void select_m_nodes_preferentially_v4(uint sz) ;

    void connect_with_m_nodes_v1(uint sz);
    void connect_with_m_nodes_v2(uint sz);
    void connect_with_m_nodes_v3(uint sz);
    void connect_with_m_nodes_v4(uint sz);

public:
    virtual ~NetworkBA() = default;
    NetworkBA() = default;
    NetworkBA(size_t m0, size_t m):Network(m0, m) {
        _N_size = get_m0();
        initialize_preferential();
    }

    void initialize_preferential();

    void reset();

    void addNode();
    void timeElapsed() ;
    void view_preferentially();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_m0_";
        ss << _m0 << "_m_" << _m << "-";
        return ss.str();
    }

    void clear_node_id(){
        for(size_t i{}; i < _nodes.size(); ++i){
            _nodes[i].set_group_id(-1);
        }
    }

    void clear_preferentially() {_preferentially.clear(); _static = true;}

    double size_in_MB();

    void shrink_to_fit(); // since pushback allocates extra memory, this function resizes it to minimum

    bool grow(size_t netowk_size);

};



#endif //NETWORK_NETWORK_BA_H
