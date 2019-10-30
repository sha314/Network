//
// Created by shahnoor on 10/17/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_V6_H
#define NETWORK_NETWORK_PERCOLATION_V6_H

#include "../nets/BA/network_BA_v2.h"

/**
 * created for better performance only
 *
 * Two options :
 * 1. Inherit NetworkBA_v2 class. check if it helps.
 *
 * or
 *
 * 2. exactly as NetworkBApercolation_v5 except for the fact that there is no NetworkBA_v2 object.
 * NetworkBA_v2 is constructed explicitly here for better performance
 */
class NetworkBApercolation_v6 : public NetworkBA_v2{
private:
    double _findRoot_time{};

protected:
    //time measurement variables
    double _time_placeSelectedLink{};
    int _max_recursion_depth{};
    size_t _random_state{};

    size_t max_link_index {};

    /*
     * cluster sizes and roots are here
     * if i-th element of _clusters is negative then it is the root node
     *      and the value gives the size of the cluster
     * if i-th element of _clusters is positive then it the value is
     *      the index of a node that is closer to the root node
     */
    std::vector<int> _cluster_info;

    std::vector<uint> list_of_link_indices; // from zero to N-1
    std::vector<uint> _randomized_indices;

    double _entropy_val{},    _largest_jump_entropy{}, _previous_entropy{}, _entropy_jump_tc{};
    size_t largest_cluster_size{}, largest_jump_cluster_size{},_previous_cluster_size{};
    int largest_cluster_index{};


    std::mt19937 _random_generator; // Mersenne Twister pseudo-random generator
    size_t occupied_link_count{};
    double log_1_by_size{}; // to make calculations easier
    double one_by_size{};
    size_t _network_size{1};
    size_t _link_count{};


    bool _self_cluster_jump{false};

public:
    virtual ~NetworkBApercolation_v6()          = default;
    NetworkBApercolation_v6()           = default;
    NetworkBApercolation_v6(size_t m0, size_t m, size_t size);

    void setRandomState(size_t seed=0, bool g=true);
    size_t getRandomState() {return _random_state;};
    void init(bool g=true);
    virtual void reset(int i);

    int findRoot(int a);
    int find_root_v1(int a);
    int find_root_v2(int a);
    int find_root_v2_test(int a);
    int find_root_v3(int i); // recursive method
    int find_root_v3_test(int i, int depth=0); // recursive method

    void mergeClusters(int root_a, int root_b);
    size_t clusterCount();
    size_t clusterSizeSum();
    int clusterSize(int a);

    virtual bool occupyLink();
    double relativeLinkDensity() { return double(occupied_link_count)/_network_size;};
    void viewClusters();
    void viewListOfLinkIndices();
    void viewNetwork(){view();}

    void initialize_network();
    void initializeNetwork() {initialize_network();};


    double entropy();
    double entropy_v1();
    double entropy_v2();
    void subtract_entropy(int a, int b);
    void add_entropy(int a);
    void jump();
    void jump_v1();
    void jump_v2();
    double largestEntropyJump()const{return _largest_jump_entropy;};
    double largestEntropyJump_pc()const{return _entropy_jump_tc;};
    double largestOrderJump() const { return largest_jump_cluster_size/double(_network_size);}
    void track_largest_cluster(int a);
    void track_largest_cluster_v2(int a);
    size_t largestClusterSize() const {return largest_cluster_size;}

    void initialize_cluster();

    void randomize_indices(std::vector<uint>&);

    bool placeSelectedLink(uint i);
//    uint getLinkIndex(uint a){return list_of_link_indices[a];}

    virtual std::string get_signature() {
        std::stringstream ss;
        ss << getClassName()
           << "_m0_" << get_m0()
           << "_m_" << get_m()
           << "_size_" << _network_size;
        return ss.str();
    }

    std::string getClassName() override {return "NetworkBApercolation_v6";}

    bool isSelfClusterJump() {return _self_cluster_jump;}

    virtual void summary();

};
#endif //NETWORK_NETWORK_PERCOLATION_V6_H
