//
// Created by shahnoor on 8/7/18.
//

#ifndef NETWORK_NETWORK_H
#define NETWORK_NETWORK_H

#include <glob.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "node/node.h"
#include "link/link.h"
#include "../percolation/cluster/cluster.h"
#include "../ext_libs/inverse_array.h"
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
    std::vector<uint> _m_nodes;
    // random engine

    size_t _random_state{};
    std::mt19937 _random;
    std::chrono::duration<double> shuffle_time=chrono::duration<double>(0);

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

    std::vector<uint> degrees();
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
    void setRandomState(size_t seed=0, bool g=true);
    size_t getRandomState() {return _random_state;};

};


/*******************************************
 * General network class
 * version 2
 * indication version 7 : so that we can easily track the classes that uses it
 * date : 2019.10.29
 ***************************************/
class Network_v7{


    /*
 * Counts the degree of node
 * i-th element = number of connection i-th node have
 */

protected:
    size_t N_size;
    std::vector<int> _nodes;
    std::vector<std::vector<int>> _adjacency_list;
    /*
 * i-th node of network_map_A is connected to the
 * i-th node of network_map_B
 */
    std::vector<int> _network_map_A;
    std::vector<int> _network_map_B;
    std::vector<uint> degree_count;
    // does not need to be in a class. make it globally available for all classes
    size_t _random_state{};
    std::mt19937 _gen;

public:
    virtual ~Network_v7() = default;
    Network_v7() = default;
//    Network_v2(size_t m0=3, size_t m=1); // only seeding

    void setRandomState(size_t seed=0, bool g=true);
    size_t getRandomState() {return _random_state;};
//    void reset();
    std::vector<uint> degrees() { return degree_count;}

    void view(); // general property of network
    void viewAdjacencyList();
    virtual std::string getClassName(){return "Network_v2";}
    virtual void initialize(size_t N) = 0; // pure virtual function
    virtual void rebuild() = 0;
    virtual std::string get_signature()=0;
    virtual void viewLocal() =0; // other properties of a network that is different for different types of network
    size_t getNodeCount() const { return  _nodes.size();}
    size_t getLinkCount() const { return  _network_map_A.size();}

    int getNodeA(int link) const {return _network_map_A[link];}
    int getNodeB(int link) const {return _network_map_B[link];}

    std::vector<double> degreeDistribution();
};


#endif //NETWORK_NETWORK_H
