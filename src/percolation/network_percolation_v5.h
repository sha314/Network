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
class NetworkBApercolation_v5 {
protected:
    //time measurement variables
    double _time_placeSelectedLink{};
    size_t _random_state{};


    /*
     * cluster sizes and roots are here
     * if i-th element of _clusters is negative then it is the root node
     *      and the value gives the size of the cluster
     * if i-th element of _clusters is positive then it the value is
     *      the index of a node that is closer to the root node
     */
    std::vector<int> _clusters;

    std::vector<uint> list_of_link_indices; // from zero to N-1

    double _entropy_val{},    _largest_jump_entropy{}, _previous_entropy{}, _entropy_jump_pc{};
    size_t largest_cluster_size{};
    int largest_cluster_index{};

    std::vector<uint> _randomized_indices;
    std::mt19937 _random;
    size_t occupied_link_count{};
    double log_1_by_size{}; // to make calculations easier
    double one_by_size{};
    size_t N_size{1};
    size_t _link_count{};
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
    void jump();
    double largestEntropyJump()const{return _largest_jump_entropy;};
    double largestEntropyJump_pc()const{return _entropy_jump_pc;};
    double relativeOccupationProbability()const {return occupied_link_count/double(N_size);};
    void track_largest_cluster(int a);
    size_t largestClusterSize() const {return largest_cluster_size;}

    void initialize_cluster();

    void randomize_indices(std::vector<uint>&);

    bool placeSelectedLink(uint i);
//    uint getLinkIndex(uint a){return list_of_link_indices[a];}

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << N_size;
        return ss.str();
    }
    std::string getClassName(){return "NetworkBApercolation_v5";}
};



#endif //NETWORK_NETWORK_PERCOLATION_V5_H
