//
// Created by shahnoor on 7/1/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_V3_H
#define NETWORK_NETWORK_PERCOLATION_V3_H


#include "../nets/network.h"
#include "../nets/link/link.h"
#include "cluster/cluster_v2.h"
#include "cluster/cluster_v3.h"


/**
 * Random Link percolation on Barabasi-Albert Network model
 * make it a template class and the template argument is the NetworkBA or NetworkMDA or NetworkRandom
 *
 * paradigm shift:
 *  1. clusters are no longer erased instead just clear them so that each nodes contains
 *      cluster id which is the cluster index. Therefore, InverseArray class is not needed here.
 *  2. all noder are a cluster of size one initially. This class creates these unit size
 *      clusters beforehand so that when percolating only thing is to do is merging.
 *
 * memory usages details
 *  m   N           memory
 *  5   1000000     628 MB
 *  5   2000000     1206 MB
 *  5   4000000     2399 MB
 */
class NetworkBApercolation_v3 {
    size_t _m0, _m;

    double one_by_N{};
    double log_1_by_size{}; // to make calculations easier

    //time measurement variables
    double _time_placeSelectedLink{};

    // counting. how many times one of the four options is used
    int manage_cluster_v2_a{-1}, manage_cluster_v2_b{-1}, manage_cluster_v2_c{-1}, manage_cluster_v2_d{-1};
protected:
    NetworkBA _network_frame;
    Link _last_lnk;
    std::vector<Cluster> _cluster;
    int group_id_count{};

    size_t _network_size; // number of nodes on the network including m0
    size_t _link_count;
    std::vector<uint> _link_indices;  // size in Bytes = number of links * 4
    std::vector<uint> _randomized_indices;  // size in Bytes = number of links * 4
    size_t _number_of_occupied_links{};
    size_t _number_of_connected_nodes{}; // not isolated
    size_t _number_of_nodes_in_the_largest_cluster{0};
    int _last_largest_cluster_id{0};
    bool _self_cluster_jump{false};

    double _current_entropy{};
    double _previous_entropy{};
    double _largest_jump_entropy{}; // todo
    double _entropy_jump_pc{};
//    size_t _last_cluster_size{};
//    size_t _previous_cluster_size{};
//    size_t _largest_jump_cluster_size{}; //todo

    double _entropy{};
    size_t index_var{};

    size_t _random_state{};
    std::mt19937 _random_generator;

    // methods
    void randomize_v1();
    void initiate(size_t m0, size_t m, size_t size);
    void initialize_cluster();
    bool placeLink();
    size_t selectLink(); // select links randomly
    bool placeSelectedLink(size_t link_pos); // place the selected link

public:

    virtual ~NetworkBApercolation_v3()  = default;
    NetworkBApercolation_v3()           = default;
    NetworkBApercolation_v3(size_t m0, size_t m, size_t size);
    void initialize_network();

    void reset(int i=0);

    virtual std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "-";
        return ss.str();
    }

    /***********************************************
     * Properties and Informations
     * Information about the state of network
     ***********************************************/
    double occupationProbability()  const { return double(_number_of_occupied_links) / _link_count;}
    double relativeLinkDensity()    const { return double(_number_of_occupied_links) / _network_size;}
    size_t largestClusterSize()     const {return _number_of_nodes_in_the_largest_cluster;}
    int largestClusterID()          const {return _last_largest_cluster_id;}
    size_t linkCount()              const { return _network_frame.number_of_links();}
    size_t nodeCount()              const { return _network_frame.number_of_nodes();}
    Link lastLink()                 const {return _last_lnk;}

    double entropy_v1();
    double entropy_v2();

    void subtract_entropy(Link& lnk);
    void add_entropy(Link& lnk);

    void jump(); // calculates jump of entropy and
    double largestEntropyJump()     const { return _largest_jump_entropy;}
    double largestEntropyJump_pc()  const { return _entropy_jump_pc;} // occupation probability at largest jump of entropy

    virtual bool occupyLink();


    /***************************************
     *  cluster management and relabeling
     ***********************************/
    void manageCluster(size_t position);
    void manage_cluster_v0(size_t position);
    void manage_cluster_v1(size_t position);
    void manage_cluster_v2(size_t link_pos); // only merges the clusters

    void merge_cluster(size_t link_pos, const Link &lnk, int id_a, int id_b,
                       uint node_b);

    void relabel_nodes(Cluster& clster, int id);
    void relabel_links(Cluster& clster, int id);

    /***************************************
     * View the network
     *************************************/
    void viewNodes() { _network_frame.viewNodes();}
    void viewLinks() {_network_frame.view_links();}
    void viewCluster();
    void viewClusterExtended();
    void viewActiveLinks(){
        // todo
    }

    double logOneBySize()           const { return log_1_by_size;}

    void track_cluster();
    void track_cluster_v2();
    bool isSelfClusterJump() {return _self_cluster_jump;}

    virtual void time_summary();

    size_t clusterCount()           const;


    void view_randomized_indices()  const ;

    double sizeSummary_in_MB();

    double oneBySize() const {return one_by_N;}

    const std::vector<double> clusterPickingProbability();
    void setRandomState(size_t seed, bool g) ;
    std::vector<double> clusterSizeDistribution();
};




#endif //NETWORK_NETWORK_PERCOLATION_V3_H
