//
// Created by shahnoor on 8/7/18.
//

#ifndef NETWORK_NETWORK_H
#define NETWORK_NETWORK_H

#include <glob.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "node.h"
#include "link.h"


/**
 * Network consists of Nodes and Links
 */
class Network{
protected:
    std::vector<Node> _nodes;
    std::vector<Link> _links;

    // m0 is the seed. Initial number of nodes
    size_t _m0, _m;

    double _total_degree; // total degree of the network

    // for selecting m links
    std::vector<size_t> _m_links, _node_indices;
public:
    ~Network() = default;
    explicit Network(size_t m0=0, size_t m=1);

    virtual void reset(){
        _nodes.clear();
        _links.clear();
        _total_degree = 0;
        initialize();
    }

    size_t get_m0() const { return _m0;}
    size_t get_m() const { return _m;}


    size_t number_of_nodes() const  {return _nodes.size();}
    size_t number_of_links() const  {return _links.size();}

    void add_node(std::vector<size_t> with_node);
    void add_node(size_t with_node);
    void add_node();

    double get_total_degree() const { return _total_degree;}

    void view_nodes();
    void view_links();

    virtual std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_m0_";
        ss << _m0 << "_m_" << _m << "-";
        return ss.str();
    }

    std::vector<double> degrees();

    void initialize();
};

/**
 * Barabasi-Albert Network model
 */
class NetworkBA : public Network{
    double time{};

    // holds the index of node.
    // number of degree the nodes have if the number of repetition of that index
    // helps efficiently selecting nodes preferentially
    std::vector<size_t> _preferentially;

    // methods
    [[deprecated("Only able to calculate for m=1")]]
    void add_node_v0();
    void add_node_v1();
    void add_node_v2();
    void add_node_v3();

    void select_m_links_preferentially_v1(size_t sz) ;
    void select_m_links_preferentially_v2(size_t sz) ;
    void select_m_links_preferentially_v3(size_t sz) ;

    void connect_with_m_nodes_v1(size_t sz);
    void connect_with_m_nodes_v2(size_t sz);
    void connect_with_m_nodes_v3(size_t sz);
    void connect_with_m_nodes_v4(size_t sz);
public:
    ~NetworkBA() = default;
    NetworkBA(size_t m0=0, size_t m=1) : Network(m0, m) {

        initialize_preferential();
    }

    void initialize_preferential();


    void add_node();
    void timeElapsed() {std::cout << time << " sec" << std::endl;}
    void view_preferentially();
    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_m0_";
        ss << _m0 << "_m_" << _m << "-";
        return ss.str();
    }

};


/**
 * Link percolation on Barabasi-Albert Network model
 */
class NetworkBApercolation : public NetworkBA{
    size_t _network_size; // number of nodes on the network
    size_t _link_count;
    std::vector<size_t> _link_indices;
    std::vector<size_t> _randomized_indices;
    double _number_of_occupied_links{};
    double _number_of_nodes_in_the_largest_cluster;
    size_t index_var{};
    Link *_last_lnk;

    // methods
    void randomize();
public:
    ~NetworkBApercolation() = default;
    NetworkBApercolation(size_t m0, size_t m, size_t size);
    void reset(){
        index_var = 0;
        _number_of_occupied_links = 0;
        _number_of_nodes_in_the_largest_cluster = 0;
        randomize();
    }
    bool occupy_link();
    std::string get_signature() override {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_m0_";
        ss << _m0 << "_m_" << _m << "_size_" << _network_size << "-";
        return ss.str();
    }

    void lastLink() {std::cout << (*_last_lnk) << std::endl;}
    void viewActiveLinks(){
        // todo
    }
};

#endif //NETWORK_NETWORK_H
