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
#include "../cluster.h"
#include "../ext_libs/inverse_array.h"


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
    std::vector<uint> _m_links, _node_indices;
public:
    ~Network() = default;
    explicit Network(size_t m0=0, size_t m=1);

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

    void add_node(std::vector<uint> with_node);
    void add_node(uint with_node);
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
    double clusteringCoefficient() const {
        cout << "TODO : line " << __LINE__ << endl;
        return 0;
    }
    double clusteringCoefficient(size_t i) const {
        cout << "TODO : line " << __LINE__ << endl;
        return 0;
    }
    void initialize();

    size_t getNumberOfLinks() const { return _links.size();}
    size_t getNumberOfNodes() const { return _nodes.size();}

    Link getLink(size_t pos) const;
    Node getNode(size_t pos) const;

    void set_link_group_id(size_t pos, int id);
    void set_node_group_id(size_t pos, int id);
    int get_link_group_id(size_t pos);
    int get_node_group_id(size_t pos);

    void activateLink(size_t pos);
    void activateNode(size_t pos);
};

/**
 * Barabasi-Albert (BA) Network model
 *
 */
class NetworkBA : public Network{
    double time{};

    // holds the index of node.
    // number of degree the nodes have is the number of repetition of that index
    // helps efficiently selecting nodes preferentially
    std::vector<uint> _preferentially;

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
    NetworkBA(size_t m0, size_t m) : Network(m0, m) {
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

    void clear_node_id(){
        for(size_t i{}; i < _nodes.size(); ++i){
            _nodes[i].set_group_id(-1);
        }
    }

};

/***
 * The Mediation-Driven Attachment (MDA) network
 */
class NetworkMDA : public Network{

};


/**
 * Link percolation on Barabasi-Albert Network model
 * make it a template class and the template argument is the NetworkBA or NetworkMDA or NetworkRandom
 */
class NetworkBApercolation {
    size_t _m0, _m;

    double log_1_by_size{}; // to make calculations easier
protected:
    NetworkBA net;
    Link _last_lnk;
    std::vector<Cluster> _cluster;
    int group_id_count{};
    InverseArray<int> _cluster_index_from_id;

    size_t _network_size; // number of nodes on the network including m0
    size_t _link_count;
    std::vector<uint> _link_indices;
    std::vector<uint> _randomized_indices;
    double _number_of_occupied_links{};
    size_t _number_of_connected_nodes{}; // not isolated
    size_t _number_of_nodes_in_the_largest_cluster{0};

    double _current_entropy{};
    double _previous_entropy{};
    double _largest_jump_entropy{}; // todo
    double _entropy_jump_pc{};
//    size_t _last_cluster_size{};
//    size_t _previous_cluster_size{};
//    size_t _largest_jump_cluster_size{}; //todo

    double _entropy{};
    size_t index_var{};


    // methods
    void randomize_v0();
    void randomize_v1();
    void initiate(size_t m0, size_t m, size_t size);
    bool placeLink();
    size_t selectLink(); // select links randomly
    bool placeSelectedLink(size_t pos); // place the selected link

public:


    virtual ~NetworkBApercolation() = default;
    NetworkBApercolation() = default;
    NetworkBApercolation(size_t m0, size_t m, size_t size);
    void initialize_network();

    void reset(int i=0);

    virtual std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "-";
        return ss.str();
    }

    /***********************************************
     * Properties and Informations
     * Information about the state of network
     ***********************************************/
    double occupationProbability() const { return _number_of_occupied_links / _link_count;}
    size_t largestClusterSize() const {return _number_of_nodes_in_the_largest_cluster;}
    size_t linkCount() const { return net.number_of_links();}
    size_t nodeCount() const { return net.number_of_nodes();}
    Link lastLink() const {return _last_lnk;}

    double entropy_v1();
    double entropy_v2();

    void subtract_entropy(Link& lnk);
    void add_entropy(Link& lnk);

    void jump(); // calculates jump of entropy and
    double largestEntropyJump()const { return _largest_jump_entropy;}
    double largestEntropyJump_pc()const { return _entropy_jump_pc;} // occupation probability at largest jump of entropy

    virtual bool occupyLink();


    /***************************************
     *  cluster management and relabeling
     ***********************************/
    void manageCluster(size_t position);
    void manage_cluster_v0(size_t position);
    void relabel_nodes(Cluster& clster, int id);

    /***************************************
     * View the network
     *************************************/
    void viewNodes() {net.view_nodes();}
    void viewLinks() {net.view_links();}
    void viewCluster();
    void viewClusterExtended();
    void viewActiveLinks(){
        // todo
    }

    double logOneBySize() const { return log_1_by_size;}

};


/***********************************************************
 * Explosive percolation on BA network
 ***********************************************************/
class NetworkBApercolationExplosive : public NetworkBApercolation{
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive() = default;
    NetworkBApercolationExplosive() = default;
    NetworkBApercolationExplosive(size_t m0, size_t m, size_t size, uint M);

    bool occupyLink();

    bool placeLink(); // link occupation

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

    /**
     * Sum rule and Product rule
     */
    size_t link_for_min_cluster_sum_rule();
    size_t link_for_min_cluster_product_rule();
    // TODO write function that take a lambda function and executes as it says
    size_t selectLink(char rule);
};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveSum : public NetworkBApercolationExplosive{

public:
    ~NetworkBApercolationExplosiveSum() = default;
    NetworkBApercolationExplosiveSum() = default;
    NetworkBApercolationExplosiveSum(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive(m0, m, size, M) {};

    bool placeLink();
    bool occupyLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_sum_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveProduct: public NetworkBApercolationExplosive{

public:
    ~NetworkBApercolationExplosiveProduct() = default;
    NetworkBApercolationExplosiveProduct() = default;
    NetworkBApercolationExplosiveProduct(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive(m0, m, size, M) {};

    bool occupyLink();
    bool placeLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_product_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};

#endif //NETWORK_NETWORK_H
