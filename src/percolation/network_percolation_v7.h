//
// Created by shahnoor on 10/30/19.
//

#ifndef NETWORK_NETWORK_PERCOLATION_V7_H
#define NETWORK_NETWORK_PERCOLATION_V7_H

#include "../nets/ER/network_ER.h"


/**
 * created for the unification
 * Date : 2019.10.30
 *
 * Constructor takes a constructed network as an argument and perform percolation on it.
 * The netwrodk must be inherited from Network_v2 class
 * Other class of percolation must be inherited from this class.
 */
class NetworkPercolation_v7{
private:

    double _findRoot_time{};

protected:
    Network_v7* _net;
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
    size_t _number_of_clusters{}; //number of clusters;
    std::vector<uint> list_of_link_indices; // from zero to N-1
    std::vector<uint> _randomized_indices;

    long double delta_H{}; // equivalent to specific heat
    long delta_P{}; // equivalent to susceptibility

    long double _entropy_val{},    _largest_jump_entropy{}, _previous_entropy{};
    double _entropy_jump_tc{};
    long largest_cluster_size{}, largest_jump_cluster_size{},_previous_cluster_size{};
    int largest_cluster_index{};


    std::mt19937 _random_generator; // Mersenne Twister pseudo-random generator
    size_t occupied_link_count{};
    double log_1_by_size{}; // to make calculations easier
    double one_by_size{};
    size_t _network_size{1};
    size_t _link_count{};


    bool _self_cluster_jump{false};

public:
    virtual ~NetworkPercolation_v7()          = default;
    NetworkPercolation_v7()           = default;

    explicit NetworkPercolation_v7(Network_v7* net);

    void setRandomState(size_t seed);
    size_t getRandomState() {return _random_state;};
    void initialize();
    virtual void reset(int i);

    int findRoot(int a);
    int find_root_v1(int a);
    int find_root_v2(int a);
    int find_root_v2_test(int a);
    int find_root_v3(int i); // recursive method
    int find_root_v3_test(int i, int depth=0); // recursive method

    int mergeClusters(int root_a, int root_b);
    int merge_cluster_v1(int root_a, int root_b) ;
    int merge_cluster_v2(int root_a, int root_b) ;

    size_t clusterCount();
    size_t clusterCountFast();
    size_t clusterSizeSum();
    int clusterSize(int a);

    virtual bool occupyLink();
    double relativeLinkDensity() { return double(occupied_link_count)/_network_size;};
    void viewClusters();
    void viewListOfLinkIndices();
    void viewNetwork(){_net->view();}

    void sumClusters();





    long double entropy();
    long double entropy_v1();
    long double entropy_v2();
    void subtract_entropy(int a, int b);
    void add_entropy(int a);
    void jump();
    void jump_v1();
    void jump_v2();
    long double jump_entropy(); // jump of entropy at each step. equivalent to specific heat
    long jump_largest_cluster(); // jump of largest cluster at each step. equivalent to susceptibility
    double largestEntropyJump()const{return static_cast<double>(abs(_largest_jump_entropy));};
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
        std::cout << "TODO " << std::endl;
        std::stringstream ss;
        ss << getClassName()
//           << "_m0_" << _net->get_m0()
//           << "_m_" << get_m()
           << "_size_" << _network_size;
        return ss.str();
    }

    virtual std::string getClassName() {return "NetworkPercolation_v7";}

    bool isSelfClusterJump() {return _self_cluster_jump;}

    virtual void summary();

    void track_cluster_count(int a, int root_b);
};

class NetworkPercolationExplosive_v7: public NetworkPercolation_v7{
    std::map<int, double> tcs;

    int _M_link{2};
public:
    ~NetworkPercolationExplosive_v7() override = default;
    NetworkPercolationExplosive_v7(Network_v7* net, int M);

    NetworkPercolationExplosive_v7() = default;
//    void initialize();

    bool occupyLink() override ;

    virtual uint link_for_min_cluster_sum_product(size_t start_at);

    virtual uint link_by_maximizing_entropy(size_t start_at);
    double entropy_diff_for_i_j(int root_a, int root_b);

    virtual std::string get_signature() {
        std::cout << "TODO " << std::endl;
        std::stringstream ss;
        ss << getClassName()
           //           << "_m0_" << _net->get_m0()
           //           << "_m_" << get_m()
           << "_size_" << _network_size;
        return ss.str();
    }

    std::string getClassName() override {return "NetworkPercolationExplosive_v7";}
    double get_tc(int M){return tcs[M];}

    virtual bool cluster_extremizing_condition(long n_nodes, long prod_sum) const;
};

class NetworkPercolationInverted_v7: public NetworkPercolation_v7{
    int _M_link{2};
public:
    ~NetworkPercolationInverted_v7() override = default;
    NetworkPercolationInverted_v7(Network_v7* net, int M);
//    void initialize();
    virtual std::string get_signature() {
        std::cout << "TODO " << std::endl;
        std::stringstream ss;
        ss << getClassName()
           << "_N_" << _network_size << "_M_" << _M_link;
        return ss.str();
    }
    bool occupyLink() override ;
    std::string getClassName() override {return "NetworkPercolationInverted_v7";}

    uint link_for_max_cluster_sum_product(size_t start_at);
};


#endif //NETWORK_NETWORK_PERCOLATION_V7_H
