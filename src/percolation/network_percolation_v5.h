//
// Created by shahnoor on 7/1/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_V5_H
#define NETWORK_NETWORK_PERCOLATION_V5_H

#include "../nets/network.h"
#include "../nets/link/link.h"
#include "cluster/cluster_v2.h"
#include "cluster/cluster_v3.h"
#include "../nets/BA/network_BA_v2.h"


/**
 * class : NetworkBApercolation_v5
 * used classes : NetworkBA_v2
 *
 */
class NetworkBApercolation_v5 { // TODO

    double log_1_by_size{}; // to make calculations easier
    double one_by_size{};
    size_t N_size;
    //time measurement variables
    double _time_placeSelectedLink{};
    size_t _random_state{};
    std::mt19937 _random;

    /*
     * cluster sizes and roots are here
     * if i-th element of _clusters is negative then it is the root node
     *      and the value gives the size of the cluster
     * if i-th element of _clusters is positive then it the value is
     *      the index of a node that is closer to the root node
     */
    std::vector<int> _clusters;

    std::vector<uint> list_of_link_indices; // from zero to N-1
    size_t occupied_link_count{};
    double _entropy_val{};
    size_t largest_cluster_size{};
    int largest_cluster_index{};
protected:
    NetworkBA_v2 _network_frame;

public:
    virtual ~NetworkBApercolation_v5()  = default;
    NetworkBApercolation_v5()           = default;
    NetworkBApercolation_v5(size_t m0, size_t m, size_t size);
    void setRandomState(size_t seed=0, bool g=true);
    size_t getRandomState() {return _random_state;};
    void init(bool g=true);
    void reset(int i);

    int findRoot(int a);
    int find_root_v1(int a);
    int find_root_v2(int a);

    void mergeClusters(int root_a, int root_b);
    size_t clusterCount();
    size_t clusterSizeSum();
    int clusterSize(int a);

    bool occupyLink();
    double relativeLinkDensity() { return double(occupied_link_count)/_network_frame.getLinkCount();};
    void viewClusters();
    void viewListOfLinkIndices();
    void viewNetwork(){_network_frame.view();}

    void initialize_network();
    void initializeNetwork() {initialize_network();};

    size_t nodeCount(){return _network_frame.getNodeCount();}
    size_t linkCount(){return _network_frame.getLinkCount();}

    double entropy(){return entropy_v2();}
    double entropy_v1();
    double entropy_v2();
    void subtract_entropy(int a, int b);
    void add_entropy(int a);
    void track_largest_cluster(int a);
    size_t largestCluster() const {return largest_cluster_size;}

    void initialize_cluster();

    void randomize_indices(std::vector<uint>&);
};



#endif //NETWORK_NETWORK_PERCOLATION_V5_H
