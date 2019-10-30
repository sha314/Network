//
// Created by shahnoor on 10/29/19.
//

#ifndef NETWORK_NETWORK_ER_H
#define NETWORK_NETWORK_ER_H

#include <vector>
#include <iostream>
#include <random>
#include <sstream>

/**
 * General network class
 */
class Network_v2{


    /*
 * Counts the degree of node
 * i-th element = number of connection i-th node have
 */

protected:
    std::vector<int> _nodes;

    /*
 * i-th node of network_map_A is connected to the
 * i-th node of network_map_B
 */
    std::vector<int> _network_map_A;
    std::vector<int> _network_map_B;
    std::vector<uint> degree_count;
    // does not need to be in a class. make it globally available for all classes
    size_t _random_state{};
    std::mt19937 _gen;

public:
    virtual ~Network_v2() = default;
    Network_v2() = default;
//    Network_v2(size_t m0=3, size_t m=1); // only seeding

    void setRandomState(size_t seed=0, bool g=true);
    size_t getRandomState() {return _random_state;};
//    void reset();
    std::vector<uint> degrees() { return degree_count;}

    void view();
    virtual std::string getClassName(){return "Network_v2";}
    virtual void initialize() = 0; // pure virtual function
    size_t getNodeCount() const { return  _nodes.size();}
    size_t getLinkCount() const { return  _network_map_A.size();}
};


// Barabasi Albert network
class Network_BA : Network_v2{

    // constant properties
    uint _m0{3}; // seed
    uint _m{2}; // number of new nodes each node comes with

    // variable properties

    size_t _N_max;
    /*
     * Very useful for preferential attachment
     */
    std::vector<uint> _preferentially;
public:
    Network_BA(size_t m0=3, size_t m=1); // only seeding
    void clear_preferentially(){_preferentially.clear();}
    uint get_m0()const { return  _m0;}
    uint get_m() const { return  _m;}
    virtual std::string getClassName(){return "Network_BA";}
};

// MDA network
class Network_MDA : Network_v2{

public:
    virtual std::string getClassName(){return "Network_MDA";}
};


// Erdos Renyi network
/**

Erdos-Renyi model
Randomize edge presence/absence
1. Two parameters
    Number of nodes: n
    Probability that an edge is present: p
2. For each of the n(n-1)/2
 possible edges in the network, flip a
(biased) coin that comes up “heads” with probability
p
3a. If coin flip is “heads”, then add the edge to the network
3b. If coin flip is “tails”, then don’t add the edge to the network

Also known as the “G(n,p)"
 model” (graph on n nodes with probability p)
 */
class Network_ER : public Network_v2{
    double p_value{};
    size_t N_size;

    int _node_index; // 1 less _N_size
    size_t _link_count{}; // number of links




    size_t _c{};

protected:


private:
    bool build(size_t netowk_size);

public:
    ~Network_ER() = default;

    explicit Network_ER(size_t N, double p);

    void reset();

    void initialize() override;


    std::vector<double> degreeDistribution();
    int fromNetworkMapA(uint a) const { return _network_map_A[a];}
    int fromNetworkMapB(uint a) const { return _network_map_B[a];}




    int getNodeA(int link) const {return _network_map_A[link];}
    int getNodeB(int link) const {return _network_map_B[link];}

    virtual std::string get_signature() {
        std::stringstream ss;
        ss << getClassName() ;
        ss << "_N_" << N_size << "_p_" << p_value << "-";
        return ss.str();
    }
    virtual std::string getClassName(){return "Network_ER";}


};
#endif //NETWORK_NETWORK_ER_H
