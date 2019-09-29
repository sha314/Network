//
// Created by shahnoor on 7/1/19.
//

#ifndef NETWORK_NETWORK_BA_V2_H
#define NETWORK_NETWORK_BA_V2_H

#include <vector>
#include <random>

/**
 * There is a list of nodes called (@var _nodes).
 * There is network map A and B. i-th member of A is connected to i-th member of B.
 * A _preferentially list. Which contains node ids. Number of times the same id appears is the degree of that node
 */
class NetworkBA_v2{
    // constant properties
    size_t _m0{3}; // seed
    size_t _m{}; // number of new nodes each node comes with

    // variable properties
    std::vector<int> _nodes;
    size_t N_size{3}; // network size = number of nodes
    size_t _N_max;
    int _node_index; // 1 less _N
    size_t _link_count{}; // number of links

    /*
     * i-th node of network_map_A is connected to the
     * i-th node of network_map_B
     */
    std::vector<int> network_map_A;
    std::vector<int> network_map_B;

    /*
     * Very useful for preferential attachment
     */
    std::vector<uint> _preferentially;

    /*
     * Counts the degree of node
     * i-th element = number of connection i-th node have
     */
    std::vector<int> degree_count;

    size_t _random_state{};
    std::mt19937 _random;

    size_t _c{};

private:
    bool add_node_v1();
    bool add_node_v2();

public:
    ~NetworkBA_v2() = default;

    explicit NetworkBA_v2(size_t m0=3, size_t m=1);
    void setRandomState(size_t seed=0, bool g=true);
    size_t getRandomState() {return _random_state;};
    void reset();
    void setMaxNetworkSize(size_t N);
    size_t getBlankSize();
    size_t get_m0()const { return  _m0;}
    size_t get_m() const { return  _m;}
    size_t getNodeCount() const { return  _N;}
    size_t getLinkCount() const { return  _link_count;}
    bool spaceAvailable();
    bool grow(size_t netowk_size);
    bool addNode();

    void initialize();
    void view();


    std::vector<double> degreeDistribution();
    int fromNetworkMapA(uint a) { return network_map_A[a];}
    int fromNetworkMapB(uint a) { return network_map_B[a];}

    void clear_preferentially(){_preferentially.clear();}
};


#endif //NETWORK_NETWORK_BA_V2_H
