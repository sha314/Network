//
// Created by shahnoor on 12/24/18.
//



#ifndef NETWORK_NETWORK_PERCOLATION_H
#define NETWORK_NETWORK_PERCOLATION_H

#include "../nets/network.h"
#include "../nets/link/link.h"
#include "cluster/cluster_v2.h"
#include "cluster/cluster_v3.h"


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
    double relativeLinkDensity()    const { return double(_number_of_occupied_links) / getNetworkSize();}
//    int largestClusterID()          const {return _last_largest_cluster_id;}


    double entropy_v1();
    double entropy_v2();

    void subtract_entropy(Link& lnk);
    void add_entropy(Link& lnk);

    void jump(); // calculates jump of entropy and
    double largestEntropyJump()const { return _largest_jump_entropy;}
    double largestEntropyJump_pc()const { return _entropy_jump_pc;} // occupation probability at largest jump of entropy
    double maxEntropy(){return log(nodeCount());}; // clusters mesured by nodes
    virtual bool occupyLink();


    /***************************************
     *  cluster management and relabeling
     ***********************************/
    void manageCluster(size_t position);
    void manage_cluster_v0(size_t position);
    void relabel_nodes(Cluster& clster, int id);
    void manage_cluster_v1(size_t position);
    void manage_cluster_v2(size_t link_pos); // only merges the clusters

    void merge_cluster(size_t link_pos, const Link &lnk, int id_a, int id_b,
                       uint node_b);

//    void relabel_nodes(Cluster& clster, int id);
    void relabel_links(Cluster& clster, int id);

    /***************************************
     * View the network
     *************************************/

    void viewNodes() { net.viewNodes();}
    void viewLinks() {net.view_links();}

    void viewCluster();
    void viewClusterExtended();
    void viewActiveLinks(){
        // todo
    }

    double logOneBySize()           const { return log_1_by_size;}
//    double oneBySize()              const { return one_by_N;}

    void track_cluster();
    void track_cluster_v2();
//    bool isSelfClusterJump() {return _self_cluster_jump;}

    virtual void time_summary();

    size_t clusterCount()           const;


    void view_randomized_indices()  const ;

    double sizeSummary_in_MB();

    const std::vector<double> clusterPickingProbability();

    uint get_m0() const { return net.get_m0();}
    uint get_m() const { return net.get_m();}
    size_t getNetworkSize() const  {return net.number_of_nodes();}
    size_t number_of_links() const  {return net.number_of_links();}

    std::vector<double> clusterSizeDistribution();


};





#endif //NETWORK_NETWORK_PERCOLATION_H // TODO




