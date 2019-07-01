//
// Created by shahnoor on 12/26/18.
//

#ifndef NETWORK_PERCOLATION_H
#define NETWORK_PERCOLATION_H


#include <random>
#include "../nets/link.h"
#include "cluster/cluster.h"


/**
 * A generalalized percolation class.
 * Only to be used after extending this class
 * @tparam NET
 */
template <class NET>
class Percolation{
protected:
    size_t _m0, _m;
    double log_1_by_size{}; // to make calculations easier
    double time_elapsed{0}; // only for debugging purposes

    // the network frame
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

    std::mt19937 _random_generator;

    // methods
    void randomize_v1();
    void initialize_for_percolation();
    bool placeLink();
    size_t selectLink(); // select links randomly
    bool occupySelectedLink(size_t pos); // place the selected link

public:

    virtual ~Percolation() = default;
    explicit Percolation() = default;
    Percolation(size_t m0, size_t m, size_t size);

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

    double entropy();

    void jump(); // calculates jump of entropy and
    double largestEntropyJump()const { return _largest_jump_entropy;}
    double largestEntropyJump_pc()const { return _entropy_jump_pc;} // occupation probability at largest jump of entropy

    virtual bool occupyLink();


    /***************************************
     *  cluster management and relabeling
     ***********************************/
    void manageCluster(size_t position);


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

template <class NET>
void Percolation<NET>::reset(int i) {
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


#endif //NETWORK_PERCOLATION_H
