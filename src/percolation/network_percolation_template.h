//
// Created by shahnoor on 5/17/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_TEMPLATE_H
#define NETWORK_NETWORK_PERCOLATION_TEMPLATE_H


#include <random>
#include <algorithm>
#include <iostream>
#include "cluster/cluster.h"
#include "../nets/link.h"
#include <chrono>

/**
 * A universal class that can perform random percolation on any network
 * that is extended from the class "Network"
 *
 * Efficiency:
 * Network      m   N           Total Time
 *
 * NetworkBA    1   1000000     2.56 sec
 * NetworkBA    5   1000000     6.914 sec
 *
 * NetworkMDA   1   1000000     1.89 sec
 * NetworkMDA   5   1000000     4.981799 sec
 *
 *
 * @tparam NET : type of the network
 */
template <class NET>
class NetworkPercolation{

    size_t _m0, _m;
    double log_1_by_size{}; // to make calculations easier
    double time_elapsed{0}; // only for debugging purposes
protected:
    NET _network_frame;
    Link _last_lnk;
    std::vector<Cluster> _cluster;
    int group_id_count{};

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

    std::mt19937 _random; // random generator

    // methods
    void randomize_v1();
    void initialize_for_percolation();
    bool placeLink();
    size_t selectLink(); // select links randomly
    bool occupySelectedLink(size_t pos); // place the selected link

public:

    virtual ~NetworkPercolation() = default;
    explicit NetworkPercolation() = default;
    NetworkPercolation(size_t m0, size_t m, size_t size);

    void initialize_network();

    void reset(int i=0);

    virtual std::string get_signature() {
        std::stringstream ss;
        ss << _network_frame.get_signature();
        ss << "_N_" << nodeCount() << "-";
        return ss.str();
    }

    /***********************************************
     * Properties and Informations
     * Information about the state of network
     ***********************************************/
    double occupationProbability() const { return _number_of_occupied_links / _link_count;}
    size_t largestClusterSize() const {return _number_of_nodes_in_the_largest_cluster;}
    size_t linkCount() const { return _network_frame.number_of_links();}
    size_t nodeCount() const { return _network_frame.number_of_nodes();}
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
    void manage_cluster_v1(size_t position);
    void relabel_nodes(Cluster& clster, int id);

    /***************************************
     * View the network
     *************************************/
    void viewNodes() { _network_frame.viewNodesExtended();}
    void viewLinks() {_network_frame.view_links();}
    void viewCluster();
    void viewClusterExtended();
    void viewActiveLinks(){
        // todo
    }

    double logOneBySize() const { return log_1_by_size;}

    double time() const {return time_elapsed;}
};

//#endif //NETWORK_NETWORK_PERCOLATION_H


/*****************************************
 * Generalized percolation class
 *
 *
 */
template <class NET>
NetworkPercolation<NET>::NetworkPercolation(size_t m0, size_t m, size_t size)
{
    _m0 = m0;
    _m = m;
    _network_size = size;
//    _network_size = size+_m0;
    initialize_network();
    initialize_for_percolation();

    size_t seed = 0;
    std::cerr << "automatic seeding is turned off" << std::endl;
//    std::random_device _random_device;
//    auto seed = _random_device();
    _random.seed(seed);
    //    std::cout << _randomized_link_indices.size() << " : " << _randomized_link_indices << std::endl;
    randomize_v1();
    log_1_by_size = std::log(1.0 / _network_size);
}

template <class NET>
void NetworkPercolation<NET>::initialize_for_percolation() {
    auto t0 = std::chrono::system_clock::now();
    _link_count = _network_frame.getNumberOfLinks();
    _link_indices.resize(_link_count);
    _randomized_indices.resize(_link_count);
    for(uint i = 0; i < _link_count; ++i){
        _link_indices[i] = i;
    }
    _randomized_indices = _link_indices;

    // each node is a cluster of it's own size
    _cluster.resize(nodeCount());
    for(uint i{}; i < _cluster.size(); ++i){
        _cluster[i].add_node({i});
        _network_frame.set_node_group_id(i, i);
        _cluster[i].set_group_id(i);
    }
    auto t1 = std::chrono::system_clock::now();
    std::cout << "Initialization for percolation time " << std::chrono::duration<double>(t1-t0).count() << std::endl;
}

template <class NET>
void NetworkPercolation<NET>::initialize_network() {
    std::cout << "Initializing Network ... " << std::flush;
    auto t0 = std::chrono::system_clock::now();
    _network_frame = NET(_m0, _m);
    uint i= _m0;
    while (i < _network_size){ // sp that number of nodes is the network size
        _network_frame.addNode();
        ++i;
    }
    auto t1 = std::chrono::system_clock::now();
    std::cout << "done." << std::endl;
    std::cout << "Network initialization time " << std::chrono::duration<double>(t1-t0).count() << std::endl;
}


/**
 * Uses Better Random number generator from standard library
 */
template <class NET>
void NetworkPercolation<NET>::randomize_v1() {

//    std::cout << "before " << _randomized_link_indices << std::endl;
    std::shuffle(_randomized_indices.begin(), _randomized_indices.end(), _random);
//    std::cout << "after " << _randomized_link_indices << std::endl;

}

template <class NET>
bool NetworkPercolation<NET>::occupyLink(){
//    return placeLink(); // added 2018.12.25
    size_t pos  = selectLink();  // 2018.12.25

    auto retValue = occupySelectedLink(pos); // 2018.12.25

    return retValue;
}

template <class NET>
bool NetworkPercolation<NET>::placeLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
//    std::cout << index_var << " ==? " << _randomized_link_indices.size();
    // select a link randomly
    size_t last_link_pos_in_randomized = _randomized_indices[index_var];
    ++index_var;
    _last_lnk = _network_frame.getLink(last_link_pos_in_randomized);
    _network_frame.activateLink(last_link_pos_in_randomized); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    subtract_entropy(_last_lnk);
    manageCluster(last_link_pos_in_randomized);
    add_entropy(_last_lnk);

    return true;
}

template <class NET>
void NetworkPercolation<NET>::manageCluster(size_t position) {
//    manage_cluster_v0(position);
    manage_cluster_v1(position);
}

/**
 *
 * @param pos : radnomized position of the link index
 */
template <class NET>
void NetworkPercolation<NET>::manage_cluster_v0(size_t pos) {
    Link lnk = _network_frame.getLink(pos);
    // getting all details
//    int id = lnk.get_group_id();
    uint node_a = lnk.get_a();
    int id_a = _network_frame.get_node_group_id(node_a);
    uint node_b = lnk.get_b();
    int id_b = _network_frame.get_node_group_id(node_b);
//    std::cout << "a " << id_a << " and b " << id_b << std::endl;
    // if both nodes have id -1. means they are not part of any cluster
    if(id_a == -1 && id_b == -1){
//        std::cout << "################ new cluster ################ : line " <<__LINE__ << std::endl;
        size_t  sz = _cluster.size();
        _cluster.push_back({});
        _cluster[sz].add_link(lnk);
        _cluster[sz].add_node(node_a);
        _cluster[sz].add_node(node_b);
        _cluster[sz].set_group_id(group_id_count);
        _number_of_connected_nodes += 2; // 2 node is connected here
        _network_frame.set_node_group_id(node_a, group_id_count);
        _network_frame.set_node_group_id(node_b, group_id_count);
        _network_frame.set_link_group_id(pos, group_id_count); // must set value in the global array. not local
        ++group_id_count;
        if(_number_of_nodes_in_the_largest_cluster < _cluster[sz].numberOfNodes()){
            _number_of_nodes_in_the_largest_cluster = _cluster[sz].numberOfNodes();
        }
    }else if(id_a != -1 && id_b == -1){
        // add to id_a
        size_t index = size_t(id_a);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_b); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_b, id_a);
        _network_frame.set_link_group_id(pos, id_a);
        if(_number_of_nodes_in_the_largest_cluster < _cluster[index].numberOfNodes()){
            _number_of_nodes_in_the_largest_cluster = _cluster[index].numberOfNodes();
        }
    }
    else if(id_a == -1 && id_b != -1){
        // add to id_b
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_a); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_a, id_b);
        _network_frame.set_link_group_id(pos, id_b);
        if(_number_of_nodes_in_the_largest_cluster < _cluster[index].numberOfNodes()){
            _number_of_nodes_in_the_largest_cluster = _cluster[index].numberOfNodes();
        }
    } else if(id_a == id_b){
        // no new nodes
        // not -1 but same then just insert the link since both nodes already belong to same cluster
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _network_frame.set_link_group_id(pos, id_b);
    }
    else{
        /// merge cluster
        // no need to add nodes to the cluster. they are already there
        size_t index_a = size_t(id_a);
        size_t index_b = size_t(id_b);
        // base is the larger cluster (number of nodes in it)
        size_t size_a = _cluster[index_a].numberOfNodes();
        size_t size_b = _cluster[index_b].numberOfNodes();
        if(size_a > size_b){
            // index_a will survive the process
            _cluster[index_a].add_link(lnk);
            _network_frame.set_node_group_id(node_b, id_a);
            _network_frame.set_link_group_id(pos, id_a);
            _cluster[index_a].insert(_cluster[index_b]);
            // relabeling
            relabel_nodes(_cluster[index_b], id_a);
            _cluster[index_b].clear();

        }else{
            // index_b will survive the process
            _cluster[index_b].add_link(lnk);
            _network_frame.set_node_group_id(node_a, id_b);
            _network_frame.set_link_group_id(pos, id_b);
            _cluster[index_b].insert(_cluster[index_a]);
            relabel_nodes(_cluster[index_a], id_b);
            _cluster[index_a].clear();

        }
        if(_number_of_nodes_in_the_largest_cluster < (size_a + size_b)){
            _number_of_nodes_in_the_largest_cluster = size_a + size_b;
        }

    }

}

/**
 * Use when initially all nodes are part of a cluster of size one.
 * So we don't have to add new nodes to the cluster.
 * We just have to join the cluster of different sizes.
 * Group id of the nodes is the same as the cluster id or index that they belong to
 *
 * @param pos : radnomized position of the link index
 */
template <class NET>
void NetworkPercolation<NET>::manage_cluster_v1(size_t pos) {
    Link lnk = _network_frame.getLink(pos);
    // getting all details
//    int id = lnk.get_group_id();
    uint node_a = lnk.get_a();
    int id_a = _network_frame.get_node_group_id(node_a);
    uint node_b = lnk.get_b();
    int id_b = _network_frame.get_node_group_id(node_b);
//    std::cout << "a " << id_a << " and b " << id_b << std::endl;
    // if both nodes have id -1. means they are not part of any cluster
    if(id_a == -1 || id_b == -1){
        std::cout << "manage_cluster_v1 requires all nodes to be part of a cluster : line " <<__LINE__ << std::endl;

    } else if(id_a == id_b){
        // no new nodes
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _network_frame.set_link_group_id(pos, id_b);
    }
    else{
        /// merge cluster
        // no need to add nodes to the cluster. they are already there
        size_t index_a = size_t(id_a);
        size_t index_b = size_t(id_b);
        // base is the larger cluster (number of nodes in it)
        size_t size_a = _cluster[index_a].numberOfNodes();
        size_t size_b = _cluster[index_b].numberOfNodes();
        if(size_a > size_b){
            // index_a will survive the process
            _cluster[index_a].add_link(lnk);
            _network_frame.set_node_group_id(node_b, id_a);
            _network_frame.set_link_group_id(pos, id_a);
            _cluster[index_a].insert(_cluster[index_b]);
            // relabeling
            relabel_nodes(_cluster[index_b], id_a);
            _cluster[index_b].clear();

        }else{
            // index_b will survive the process
            _cluster[index_b].add_link(lnk);
            _network_frame.set_node_group_id(node_a, id_b);
            _network_frame.set_link_group_id(pos, id_b);
            _cluster[index_b].insert(_cluster[index_a]);
            relabel_nodes(_cluster[index_a], id_b);
            _cluster[index_a].clear();

        }
        if(_number_of_nodes_in_the_largest_cluster < (size_a + size_b)){
            _number_of_nodes_in_the_largest_cluster = size_a + size_b;
        }

    }

}

template <class NET>
void NetworkPercolation<NET>::viewCluster() {
    for(size_t i{}; i < _cluster.size(); ++i){
        if(_cluster[i].empty()){
            continue;
        }
        std::cout << "cluster[" << i << "] : id " << _cluster[i].get_group_id() << "{" << std::endl;
        std::cout << "  Nodes (" << _cluster[i].numberOfNodes() << "): ";
        _cluster[i].viewNodes();
        std::cout << std::endl;
        std::cout << "  Links (" << _cluster[i].numberOfLinks() << "): ";
        _cluster[i].viewLinks();
        std::cout << std::endl;
        std::cout << "}" << std::endl;
    }

}

template <class NET>
void NetworkPercolation<NET>::viewClusterExtended() {
    for(size_t i{}; i < _cluster.size(); ++i){
        if(_cluster[i].empty()){
            continue;
        }
        std::cout << "cluster[" << i << "] : id " << _cluster[i].get_group_id() << "{" << std::endl;
        std::cout << "  Nodes (" << _cluster[i].numberOfNodes() << "): ";
        auto nds = _cluster[i].getNodes();
        std::cout << "(index, id)->";
        for(auto n: nds){
            std::cout << "(" << n << "," << _network_frame.get_node_group_id(n) << "),";
        }
        std::cout << std::endl;
        std::cout << "  Links (" << _cluster[i].numberOfLinks() << "): ";
        _cluster[i].viewLinksExtended();
        std::cout << std::endl;
        std::cout << "}" << std::endl;
    }

}

template <class NET>
void NetworkPercolation<NET>::relabel_nodes(Cluster& clstr, int id) {
    auto nds = clstr.getNodes();
    for(size_t i{}; i < nds.size(); ++i){
        _network_frame.set_node_group_id(nds[i],id);
    }
}

/**
 * Calculates shanon entropy
 * mu = (number of nodes in a cluster) / (total number of nodes)
 * H = - sum mu_i log(mu_i)
 * @return
 */
template <class NET>
double NetworkPercolation<NET>::entropy_v1() {
    size_t count{}, n;
    double mu{}, H{};
    for(size_t i{}; i < _cluster.size(); ++i){
        n = _cluster[i].numberOfNodes();
        count += n;
        mu = n / double(_network_size);
        H += mu * std::log(mu); // shanon entropy
    }
    if(_network_size != _network_frame.number_of_nodes()){
        std::cout << "_network_size != _network_frame.getNetworkSize() ; line " << __LINE__ << std::endl;
    }
    size_t remaining = _network_size - count;
    H += remaining * logOneBySize() / double(_network_size); // for the clusters of size one
    _current_entropy = -H;
    return _current_entropy;
}

template <class NET>
double NetworkPercolation<NET>::entropy_v2() {
    size_t isolated_node = _network_size - _number_of_connected_nodes;
//    double lg = std::log(1.0 / _network_size);
//    std::cout << logOneBySize() << std::endl;
    double H = - (logOneBySize() * isolated_node) / _network_size;
    _current_entropy = H + _entropy;
    return _current_entropy;
}

/**
 * Subtract entropy for two nodes of the lnk link
 * @param lnk
 */
template <class NET>
void NetworkPercolation<NET>::subtract_entropy(Link &lnk) {
    size_t a = lnk.get_a();
    size_t b = lnk.get_b();
    size_t n{};
    double H{};
    double mu {};

    int id_a = _network_frame.get_node_group_id(a);
    int id_b = _network_frame.get_node_group_id(b);
//    std::cout << "node " << a << " id " << id_a << std::endl;
//    std::cout << "node " << b << " id " << id_b << std::endl;
    if(id_a == id_b && id_a != -1){
        // belongs to same cluster
        n = _cluster[id_a].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
        _entropy -= H;
        return; // no need to go further
    }
    if(id_a != -1){
        n = _cluster[id_a].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
    }
    if(id_b != -1){
        n = _cluster[id_b].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
    }
    _entropy -= H;
//    auto t1 = chrono::system_clock::now();
//    time_elapsed += chrono::duration<double>(t1-t0).count();
}

template <class NET>
void NetworkPercolation<NET>::add_entropy(Link &lnk) {
    size_t a = lnk.get_a();
    size_t b = lnk.get_b();
    size_t n{};
    double H{}, mu{};
    if(_network_frame.getNode(a).get_group_id() != -1) {
        n = _cluster[_network_frame.getNode(a).get_group_id()].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
    }
    _entropy += H;
}

/**
 * Selects a link randomly
 * @return
 */
template <class NET>
size_t NetworkPercolation<NET>::selectLink() {
    if(index_var >= _link_count){
        return _link_count + 1; // so that program chashes or stops
    }
    // select a link randomly
    size_t pos = _randomized_indices[index_var];
    ++index_var;
    return pos;
}

/**
 * place the link whose index in network is passed as argument
 * @param pos
 * @return
 */
template <class NET>
bool NetworkPercolation<NET>::occupySelectedLink(size_t pos) {
    if(pos >= _link_count){
//        std::cout << "pos > link_count : line " << __LINE__ << std::endl;
        return false;
    }
    _last_lnk = _network_frame.getLink(pos);
    _network_frame.activateLink(pos); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    subtract_entropy(_last_lnk);
    manageCluster(pos);
    add_entropy(_last_lnk);

    return true;
}

template <class NET>
void NetworkPercolation<NET>::reset(int i) {
    index_var = 0;
    _number_of_occupied_links = 0;
    _number_of_connected_nodes = 0;
    _number_of_nodes_in_the_largest_cluster = 0;
    _entropy = 0;
    _largest_jump_entropy =0;
    _previous_entropy = 0;
    _current_entropy = 0;
    group_id_count = 0;
    _last_lnk = {};
    if(i == 1){
        // initialize the network again
        initialize_network();
    }
    _randomized_indices = _link_indices;
    randomize_v1();
    _cluster.clear();
    _network_frame.clear_node_id();

}


/**
 * Must be called after entropy is calculated each time. If not then this function will not work.
 */
template <class NET>
void NetworkPercolation<NET>::jump() {
    double delta_H{};
    if(_number_of_occupied_links == 1){
        _previous_entropy = _current_entropy;
    }else{
        delta_H = _current_entropy - _previous_entropy;
        _previous_entropy = _current_entropy; // be ready for next step
    }
    if(abs(delta_H) > abs(_largest_jump_entropy)){
        _largest_jump_entropy = delta_H;
        _entropy_jump_pc = occupationProbability();
    }

}


#endif //NETWORK_NETWORK_PERCOLATION_TEMPLATE_H
