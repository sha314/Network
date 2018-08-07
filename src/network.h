//
// Created by shahnoor on 8/7/18.
//

#ifndef NETWORK_NETWORK_H
#define NETWORK_NETWORK_H

#include <glob.h>
#include <vector>
#include <iostream>
#include <sstream>


/**
 * Node of a network
 */
class Node{
    size_t  _id;
    std::vector<size_t> _connected_nodes;

    int _group_id;
public:
    ~Node() = default;
    Node(size_t id);
    size_t get_id() const { return  _id;}
    int get_group_id() const { return  _group_id;}
    void set_group_id(int id) {_group_id = id;}

    const std::vector<size_t>& get_neighbors() const {return _connected_nodes;}

    void add_neighbor(Node n);
    void add_neighbor(size_t n);

    /**
     * Checked version
     * less efficient but useful for debugging
     */
    void add_neighbor_checked(Node n);
    void add_neighbor_checked(size_t n);

    /**
     * Number of links a node have is the degree of that node
     */
    double degree() const {return _connected_nodes.size();}

};


std::ostream& operator<<(std::ostream& os, const Node& node);

/**
 *
 */
class Link{
    //size_t _id;
    int _group_id{-1}; // will be used for percolation
    bool _active{false};
    size_t _id_node_a, _id_node_b;
public:
    ~Link() = default;
    Link(size_t id_a, size_t id_b);

    int get_group_id() const { return  _group_id;}
    void set_group_id(int id) {_group_id = id;}

    bool is_active() const { return _active;}
    void activate() {_active = true;}

    std::string to_string() const {
        std::stringstream ss;
        ss << "(" << _id_node_a  << "," << _id_node_b << ")";
        return ss.str();
    }
};

std::ostream& operator<<(std::ostream& os, const Link& lnk);

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

public:
    virtual ~Network() = default;

    explicit Network(size_t m0=0, size_t m=1);

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
};

/**
 * Barabasi-Albert Network model
 */
class NetworkBA : public Network{

public:
    ~NetworkBA() = default;
    NetworkBA(size_t m0=0, size_t m=1) : Network(m0, m) {};

    void add_node();

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
public:
    ~NetworkBApercolation() = default;
    NetworkBApercolation(size_t m0, size_t m, size_t size);

    std::string get_signature() override {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_m0_";
        ss << _m0 << "_m_" << _m << "_size_" << _network_size << "-";
        return ss.str();
    }
};

#endif //NETWORK_NETWORK_H
