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



#endif //NETWORK_NETWORK_H
