//
// Created by shahnoor on 12/24/18.
//



#ifndef NETWORK_NETWORK_PERCOLATION_H
#define NETWORK_NETWORK_PERCOLATION_H

#include "../nets/network.h"
#include "../nets/link.h"


/**
 * Random Link percolation on Barabasi-Albert Network model
 * make it a template class and the template argument is the NetworkBA or NetworkMDA or NetworkRandom
 *
 * paradigm shift:
 *  1. clusters are no longer erased instead just clear them so that each nodes contains
 *      cluster id which is the cluster index. Therefore, InverseArray class is not needed here.
 *  2. all noder are a cluster of size one initially. This class creates these unit size
 *      clusters beforehand so that when percolating only thing is to do is merging.
 *  3. Since there is no explicit use of Link or LinkIndex.
 *      This class does not include them in the Cluster. TODO
 *
 * memory usages details
 *  m   N           memory      time
 *  5   1000000     375 MB      20 sec
 *  5   2000000     737 MB      43 sec
 *  5   4000000     1470 MB     1 min 30 sec
 *
 */
class NetworkBApercolation_v4 { // TODO
    size_t _m0, _m;

    double log_1_by_size{}; // to make calculations easier

    //time measurement variables
    double _time_placeSelectedLink{};

    // counting. how many times one of the four options is used
    int manage_cluster_v2_a{-1}, manage_cluster_v2_b{-1}, manage_cluster_v2_c{-1}, manage_cluster_v2_d{-1};
protected:
    NetworkBA _network_frame;
    Link _last_lnk;
    std::vector<Cluster_v3> _cluster;
    int group_id_count{};

    size_t _network_size; // number of nodes on the network including m0
    size_t _link_count;
    std::vector<uint> _randomized_link_indices; // size in Bytes = number of links * 4

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

    std::random_device _random_device;
    std::mt19937 _random_generator;

    // methods
    void randomize_v1();
    void initiate(size_t m0, size_t m, size_t size);
    void initialize_cluster();

    size_t selectLink(); // select links randomly
    bool placeSelectedLink(size_t link_pos); // place the selected link

public:

    virtual ~NetworkBApercolation_v4()  = default;
    NetworkBApercolation_v4()           = default;
    NetworkBApercolation_v4(size_t m0, size_t m, size_t size);
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

    void manage_cluster_v2(size_t link_pos); // only merges the clusters

    void merge_cluster(size_t link_pos, const Link &lnk, int id_a, int id_b,
                       uint node_b);

    void relabel_nodes(Cluster_v3& clster, int id);
    void relabel_links(Cluster_v3& clster, int id);

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
};



/**
 * version 3 : inherits NetworkBApercolation_v3
 *
 * with m, N, M = 2, 1000000, 9 => time elapsed = 18.774518 sec
 */
class NetworkBApercolationExplosive_v4 : public NetworkBApercolation_v4{
private:
    // time measurement variable
    double _time_selectLink{}, _time_link_for_min_cluster_sum_rule {};
    size_t _count_link_for_min_cluster_sum_rule_a{}, _count_link_for_min_cluster_sum_rule_b{}, _count_link_for_min_cluster_sum_rule_c{};
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive_v4() = default;
    NetworkBApercolationExplosive_v4() = default;
    NetworkBApercolationExplosive_v4(size_t m0, size_t m, size_t size, uint M);

    bool occupyLink();

    bool placeLink(); // link occupation

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

    /**
     * Sum rule and Product rule
     */
    size_t link_for_min_cluster_sum_rule(size_t start_at=0);
    size_t link_for_min_cluster_product_rule(size_t start_at=0);
    size_t link_for_min_cluster_sum_product(size_t start_at=0); // both sum and product rule
    size_t link_for_min_cluster_sum_product_v2(size_t start_at=0); // both sum and product rule and randomly chooses from _randomized_link_indices
    // TODO write function that take a lambda function and executes as it says
    size_t selectLink(char rule);
    size_t selectLink_v2();

    void time_summary();
};



#endif //NETWORK_NETWORK_PERCOLATION_H // TODO




