//
// Created by shahnoor on 12/24/18.
//



#ifndef NETWORK_NETWORK_PERCOLATION_H
#define NETWORK_NETWORK_PERCOLATION_H

#include "../nets/network.h"
#include "../nets/link.h"


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

    double entropy_v1();
    double entropy_v2();

    void subtract_entropy(Link& lnk);
    void add_entropy(Link& lnk);

    void jump(); // calculates jump of entropy and
    double largestEntropyJump()const { return _largest_jump_entropy;}
    double largestEntropyJump_pc()const { return _entropy_jump_pc;} // occupation probability at largest jump of entropy

    virtual bool occupyLink();


    /***************************************
     *  cluster management and relabeling
     ***********************************/
    void manageCluster(size_t position);
    void manage_cluster_v0(size_t position);
    void relabel_nodes(Cluster& clster, int id);

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

    double logOneBySize() const { return log_1_by_size;}

};

/**
 * Random Link percolation on Barabasi-Albert Network model
 * make it a template class and the template argument is the NetworkBA or NetworkMDA or NetworkRandom
 *
 * paradigm shift: clusters are no longer erased instead just clear them so that each nodes contains
 * cluster id which is the cluster index. Therefore, InverseArray class is not needed here.
 */
class NetworkBApercolation_v2 {
    size_t _m0, _m;

    double log_1_by_size{}; // to make calculations easier

    //time measurement variables
    double _time_placeSelectedLink{};
protected:
    NetworkBA _network_frame;
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
    bool placeLink();
    size_t selectLink(); // select links randomly
    bool placeSelectedLink(size_t pos); // place the selected link

public:

    virtual ~NetworkBApercolation_v2() = default;
    NetworkBApercolation_v2() = default;
    NetworkBApercolation_v2(size_t m0, size_t m, size_t size);
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
    double occupationProbability() const { return _number_of_occupied_links / _link_count;}
    size_t largestClusterSize() const {return _number_of_nodes_in_the_largest_cluster;}
    size_t linkCount() const { return _network_frame.number_of_links();}
    size_t nodeCount() const { return _network_frame.number_of_nodes();}
    Link lastLink() const {return _last_lnk;}

    double entropy_v1();
    double entropy_v2();

    void subtract_entropy(Link& lnk);
    void add_entropy(Link& lnk);

    void jump(); // calculates jump of entropy and
    double largestEntropyJump()const { return _largest_jump_entropy;}
    double largestEntropyJump_pc()const { return _entropy_jump_pc;} // occupation probability at largest jump of entropy

    virtual bool occupyLink();


    /***************************************
     *  cluster management and relabeling
     ***********************************/
    void manageCluster(size_t position);
    void manage_cluster_v0(size_t position);
    void manage_cluster_v1(size_t position);
    void relabel_nodes(Cluster& clster, int id);

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

    double logOneBySize() const { return log_1_by_size;}

    void track_cluster();
    void track_cluster_v2();
    bool isSelfClusterJump() {return _self_cluster_jump;}

    void time_summary();

    size_t clusterCount() const;
};


/**
 * Random Link percolation on Barabasi-Albert Network model
 * make it a template class and the template argument is the NetworkBA or NetworkMDA or NetworkRandom
 *
 * paradigm shift:
 *  1. clusters are no longer erased instead just clear them so that each nodes contains
 *      cluster id which is the cluster index. Therefore, InverseArray class is not needed here.
 *  2. all noder are a cluster of size one initially. This class creates these unit size
 *      clusters beforehand so that when percolating only thing is to do is merging.
 */
class NetworkBApercolation_v3 {
    size_t _m0, _m;

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
    std::vector<uint> _link_indices;
    std::vector<uint> _randomized_indices;
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
    bool placeLink();
    size_t selectLink(); // select links randomly
    bool placeSelectedLink(size_t link_pos); // place the selected link

public:

    virtual ~NetworkBApercolation_v3() = default;
    NetworkBApercolation_v3() = default;
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
    double occupationProbability() const { return double(_number_of_occupied_links) / _link_count;}
    size_t largestClusterSize() const {return _number_of_nodes_in_the_largest_cluster;}
    size_t linkCount() const { return _network_frame.number_of_links();}
    size_t nodeCount() const { return _network_frame.number_of_nodes();}
    Link lastLink() const {return _last_lnk;}

    double entropy_v1();
    double entropy_v2();

    void subtract_entropy(Link& lnk);
    void add_entropy(Link& lnk);

    void jump(); // calculates jump of entropy and
    double largestEntropyJump()const { return _largest_jump_entropy;}
    double largestEntropyJump_pc()const { return _entropy_jump_pc;} // occupation probability at largest jump of entropy

    virtual bool occupyLink();


    /***************************************
     *  cluster management and relabeling
     ***********************************/
    void manageCluster(size_t position);
    void manage_cluster_v0(size_t position);
    void manage_cluster_v1(size_t position);
    void manage_cluster_v2(size_t link_pos); // only merges the clusters
    void relabel_nodes(Cluster& clster, int id);

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

    double logOneBySize() const { return log_1_by_size;}

    void track_cluster();
    void track_cluster_v2();
    bool isSelfClusterJump() {return _self_cluster_jump;}

    virtual void time_summary();

    size_t clusterCount() const;

    void merge_cluster(size_t link_pos, const Link &lnk, int id_a, int id_b,
                       uint node_b);

    void view_randomized_indices() const ;
};


/***********************************************************
 * Explosive percolation on BA network
 ***********************************************************/
class NetworkBApercolationExplosive : public NetworkBApercolation{
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive() = default;
    NetworkBApercolationExplosive() = default;
    NetworkBApercolationExplosive(size_t m0, size_t m, size_t size, uint M);

    bool occupyLink();

    bool placeLink(); // link occupation

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

    /**
     * Sum rule and Product rule
     */
    size_t link_for_min_cluster_sum_rule();
    size_t link_for_min_cluster_product_rule();
    // TODO write function that take a lambda function and executes as it says
    size_t selectLink(char rule);
};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveSum : public NetworkBApercolationExplosive{

public:
    ~NetworkBApercolationExplosiveSum() = default;
    NetworkBApercolationExplosiveSum() = default;
    NetworkBApercolationExplosiveSum(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive(m0, m, size, M) {};

    bool placeLink();
    bool occupyLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_sum_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveProduct: public NetworkBApercolationExplosive{

public:
    ~NetworkBApercolationExplosiveProduct() = default;
    NetworkBApercolationExplosiveProduct() = default;
    NetworkBApercolationExplosiveProduct(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive(m0, m, size, M) {};

    bool occupyLink();
    bool placeLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_product_m0_";
        ss << net.get_m0() << "_m_" << net.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};


/***********************************************************
 * Explosive percolation on BA network
 ***********************************************************/
class NetworkBApercolationExplosive_v2 : public NetworkBApercolation_v2{
private:
    // time measurement variable
    double _time_selectLink{};
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive_v2() = default;
    NetworkBApercolationExplosive_v2() = default;
    NetworkBApercolationExplosive_v2(size_t m0, size_t m, size_t size, uint M);

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
    size_t link_for_min_cluster_sum_rule();
    size_t link_for_min_cluster_product_rule();
    // TODO write function that take a lambda function and executes as it says
    size_t selectLink(char rule);

    void time_summary();
};

/**
 * version 3 : inherits NetworkBApercolation_v3
 *
 * with m, N, M = 2, 1000000, 9 => time elapsed = 18.774518 sec
 */
class NetworkBApercolationExplosive_v3 : public NetworkBApercolation_v3{
private:
    // time measurement variable
    double _time_selectLink{}, _time_link_for_min_cluster_sum_rule {};
    size_t _count_link_for_min_cluster_sum_rule_a{}, _count_link_for_min_cluster_sum_rule_b{}, _count_link_for_min_cluster_sum_rule_c{};
protected:
    uint _M{2}; // number of link to choose for product rule or sum rule
public:
    static const int MAX_M_VALUE = 20;

    ~NetworkBApercolationExplosive_v3() = default;
    NetworkBApercolationExplosive_v3() = default;
    NetworkBApercolationExplosive_v3(size_t m0, size_t m, size_t size, uint M);

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
    size_t link_for_min_cluster_sum_product_v2(size_t start_at=0); // both sum and product rule and randomly chooses from _randomized_indices
    // TODO write function that take a lambda function and executes as it says
    size_t selectLink(char rule);
    size_t selectLink_v2();

    void time_summary();
};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveSum_v2 : public NetworkBApercolationExplosive_v2{

public:
    ~NetworkBApercolationExplosiveSum_v2() = default;
    NetworkBApercolationExplosiveSum_v2() = default;
    NetworkBApercolationExplosiveSum_v2(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive_v2(m0, m, size, M) {};

    bool placeLink();
    bool occupyLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_sum_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};

/**
 * Explosive percolation on BA network
 */
class NetworkBApercolationExplosiveProduct_v2: public NetworkBApercolationExplosive_v2{

public:
    ~NetworkBApercolationExplosiveProduct_v2() = default;
    NetworkBApercolationExplosiveProduct_v2() = default;
    NetworkBApercolationExplosiveProduct_v2(size_t m0, size_t m, size_t size, uint M)
            : NetworkBApercolationExplosive_v2(m0, m, size, M) {};

    bool occupyLink();
    bool placeLink();

    std::string get_signature() {
        std::stringstream ss;
        ss << "netrowk_BA_percolation_explosive_product_m0_";
        ss << _network_frame.get_m0() << "_m_" << _network_frame.get_m() << "_size_" << _network_size << "_M_" << _M << '-';
        return ss.str();
    }

};




#endif //NETWORK_NETWORK_PERCOLATION_H // TODO




