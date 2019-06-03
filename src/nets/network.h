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
#include "../percolation/cluster.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <unordered_set>
#include <sys/types.h>

/**********************************
 * Network consists of Nodes and Links
 * Random Network
 */
class Network{
protected:
    std::vector<Node> _nodes;
    std::vector<Link> _links;

    // m0 is the seed. Initial number of nodes
    uint _m0, _m;

    double _total_degree; // total neighborCount of the network

    // for selecting m links
    std::vector<uint> _node_indices;
//    std::unordered_set<uint> _m_links; // so that repetitionis automatically ignored
    std::vector<uint> _m_links;
    // random engine
    std::mt19937 _random_generator;
    std::chrono::duration<double> shuffle_time=std::chrono::duration<double>(0);
public:
    ~Network() = default;
    explicit Network(uint m0=0, uint m=1);

    virtual void reset(){
        _nodes.clear();
        _links.clear();
        _total_degree = 0;
        initialize();
    }

    uint get_m0() const { return _m0;}
    uint get_m() const { return _m;}


    size_t number_of_nodes() const  {return _nodes.size();}
    size_t number_of_links() const  {return _links.size();}

    void add_node(const std::vector<uint>& with_node);
    void add_node(uint with_node);

    void add_node_v1();
    void add_node_v2();


    double get_total_degree() const { return _total_degree;}

    void viewNodes();
    void viewNodesExtended();
    void view_links();

    virtual std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_m0_";
        ss << _m0 << "_m_" << _m << "-";
        return ss.str();
    }

    std::vector<size_t> degrees();
    double clusteringCoefficient() const {
        std::cout << "TODO : line " << __LINE__ << std::endl;
        return 0;
    }
    double clusteringCoefficient(size_t i) const {
        std::cout << "TODO : line " << __LINE__ << std::endl;
        return 0;
    }
    void initialize();
    void initialize_v2();

    size_t getNumberOfLinks() const { return _links.size();}
    size_t getNumberOfNodes() const { return _nodes.size();}

    Link getLink(size_t pos) const;
    Node getNode(size_t pos) const;
    std::vector<Node> getNodes() const {return _nodes;}
    std::vector<Link> getLinks() const {return _links;}

    void set_link_group_id(size_t pos, int id);
    void set_node_group_id(size_t pos, int id);
    int get_link_group_id(size_t pos);
    int get_node_group_id(size_t pos);

    void activateLink(size_t pos);
    void activateNode(size_t pos);

    void getTime() const {std::cout << "shuffle time " << shuffle_time.count() << std::endl;}

    double size_in_MB();

    void shrink_to_fit();

};

/****************************************
 * Barabasi-Albert (BA) Network model
 *
 */
class NetworkBA : public Network{
    double time{};

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

    void select_m_links_preferentially_v1(uint sz) ;
    void select_m_links_preferentially_v2(uint sz) ;
    void select_m_links_preferentially_v3(uint sz) ;
    void select_m_links_preferentially_v4(uint sz) ;

    void connect_with_m_nodes_v1(uint sz);
    void connect_with_m_nodes_v2(uint sz);
    void connect_with_m_nodes_v3(uint sz);
    void connect_with_m_nodes_v4(uint sz);

public:
    virtual ~NetworkBA() = default;
    NetworkBA() = default;
    NetworkBA(size_t m0, size_t m):Network(m0, m) {

        initialize_preferential();
    }

    void initialize_preferential();

    void reset();

    void addNode();
    void timeElapsed() {std::cout << time << " sec" << std::endl;}
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

};

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



#endif //NETWORK_NETWORK_H
