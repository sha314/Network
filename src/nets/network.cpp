//
// Created by shahnoor on 8/7/18.
//

#include <chrono>
#include <random>
#include "network.h"
#include "../util/printer.h"

using  namespace std;



/*****************************
 * Network Methods
 ****************************/

/**
 *
 * @param m0 : number of initial nodes
 * @param m  : number of links new nodes are born with
 */
Network::Network(uint m0, uint m) {
    if (m0 <= m){
        _m0 = m + 1;
        _m = m;
    }else{
        _m0 = m0;
        _m = m;
    }
    if(_m0 < 3){_m0=3;}

    std::random_device _random_device;
    unsigned long random_seed  = 1;
//    random_seed  = _random_device();
    cout << "Random seed : " << random_seed << endl;
    _random_generator.seed(random_seed); // seeding

    initialize();

}

void Network::initialize() {
    for(uint i{}; i != _m0; ++i){
        _nodes.push_back({i}); // add node with id equal to the size of the _nodes
    }
    uint k{};
    _node_indices.resize(_m0);
    for(uint i{}; i != _m0; ++i){
        _node_indices[i] = i;
        k = (i+1) % _m0;
        _nodes[i].addNeighbor(k);
        _nodes[k].addNeighbor(i);
        _total_degree += 2;
        _links.push_back({k, i});
    }
    _m_links.resize(_m);

}


/**
 * Add a new node to the network and connect with with_node nodes
 * @param with_node
 */
void Network::add_node(const std::vector<uint>& with_node) {
    size_t sz = {_nodes.size()};
    uint sz2 = uint(sz);
    _nodes.push_back({sz2}); // add node with id equal to the size of the _nodes
    for(uint n: with_node){
        _nodes[sz].addNeighbor(n);
        _nodes[n].addNeighbor(sz2);
        _total_degree += 2; // one link  increases neighborCount by 2.
        _links.push_back({sz2, n});
    }
}

void Network::add_node(uint with_node) {
    size_t sz = {_nodes.size()};
    uint sz2 = uint(sz);
    _nodes.push_back({sz2}); // add node with id equal to the size of the _nodes
    _nodes[sz].addNeighbor(with_node);
    _nodes[with_node].addNeighbor(sz2);
    _total_degree += 2; // one link  increases neighborCount by 2.
    _links.push_back({sz2, with_node});
}

/**
 * Adds node randomly
 * randomly select m nodes with which new node will be connected
 */
void Network::add_node() {
    size_t sz = _nodes.size();
    auto sz2 = uint(sz);
    _nodes.push_back({sz2}); // add node with id equal to the size of the _nodes


//    vector<uint> m_links(_m), node_indices(sz);
    _node_indices.resize(_nodes.size());
    //std::shuffle(randomized_index.begin(), randomized_index.end(), _random_generator);
    // list of all existing nodes
    for(uint i{}; i < sz; ++i){
        _node_indices[i] = i;
    }
    uint r{};

    // set m_links such that there is no repetition
    for(uint j{}; j < _m; ++j){
        r = rand() % _node_indices.size();
        _m_links[j] = _node_indices[r];
        _node_indices.erase(_node_indices.begin() + r); // so that its not get selected again
    }

    // setting m links in the netrowk
    uint i{};
    for(uint k{}; k < _m; ++k){
        i = _m_links[k];
        _nodes[i].addNeighbor(sz2);
        _nodes[sz].addNeighbor(i);
        _total_degree += 2; // one link  increases neighborCount by 2.
        _links.push_back({sz2, i});
    }

}

void Network::add_node_v2() {
    size_t sz = _nodes.size();
    auto sz2 = uint(sz);
    _nodes.push_back({sz2}); // add node with id equal to the size of the _nodes


    std::shuffle(_node_indices.begin(), _node_indices.end(), _random_generator);

    // set m_links such that there is no repetition
//    for(uint j{}; j < _m; ++j){
//        _m_links[j] = _node_indices[j];
//    }

    // setting m links in the netrowk
    uint i{};
    for(uint k{}; k < _m; ++k){
//        i = _m_links[k];
        i = _node_indices[k];
        _nodes[i].addNeighbor(sz2);
        _nodes[sz].addNeighbor(i);
        _total_degree += 2; // one link  increases neighborCount by 2.
        _links.push_back({sz2, i});
    }
    _node_indices.push_back(sz2); // getting indices ready for next iteration

}


/**
 *
 */
void Network::view_nodes() {
    cout << "id(size):{neighbors,...}" << endl;
    for(uint i{}; i < _nodes.size(); ++i){
        cout << _nodes[i].get_id() << "(" << _nodes[i].neighborCount() << "):{";
        auto neighbors = _nodes[i].get_neighbors();
        for(auto n: neighbors){
            cout << n << ',';
        }
        cout <<  "}" << endl;
    }
}

void Network::view_links() {
    cout << "Total " << _links.size() << " links :{" ;
    for(uint i{}; i < _links.size(); ++i){
        cout << _links[i] << ',';
    }
    cout << "}" << endl;
}

vector<double> Network::degrees() {
    vector<double> degs(_nodes.size());

    for(uint i{}; i < _nodes.size(); ++i){
        degs[i] = _nodes[i].neighborCount();
    }
//    cout << degs.size() << " : line " << __LINE__ << endl;
//    cout << "line " << __LINE__ << endl;
    return degs;
}

Link Network::getLink(size_t pos) const {
    if(pos < _links.size())
        return _links[pos];
    else {
        cout << "out of bound : line " << __LINE__ << endl;
        return {};
    }
}

Node Network::getNode(size_t pos) const {
    if(pos < _nodes.size())
        return _nodes[pos];
    else {
        cout << "out of bound : line " << __LINE__ << endl;
        exit(-2);
    }
}

void Network::set_link_group_id(size_t pos, int id) {
    if(pos < _links.size()){
        _links[pos].set_group_id(id);
    }else{
        cout << "Out of bound : line " << __LINE__ << endl;
    }
}

void Network::set_node_group_id(size_t pos, int id) {
    if(pos < _nodes.size()){
        _nodes[pos].set_group_id(id);
    }else{
        cout << "Out of bound : line " << __LINE__ << endl;
    }
}

int Network::get_link_group_id(size_t pos) {
    if(pos < _links.size()){
        return _links[pos].get_group_id();
    }else{
        cout << "Out of bound : line " << __LINE__ << endl;
        return -2;
    }
}

int Network::get_node_group_id(size_t pos) {
    if(pos < _nodes.size()){
        return _nodes[pos].get_group_id();
    }else{
        cout << "Out of bound : line " << __LINE__ << endl;
        return -2;
    }
}

void Network::activateLink(size_t pos) {
    if(pos < _links.size()){
        _links[pos].activate();
    }else{
        cout << "Out of bound : line " << __LINE__ << endl;
    }
}

void Network::activateNode(size_t pos) {
    if(pos < _nodes.size()){
        cout << "Not implemented : line " << __LINE__ << endl;
//        _nodes[pos].activate();
    }else{
        cout << "Out of bound : line " << __LINE__ << endl;
    }
}

/**************************************
 * NetworkBA methods
 **************************************/

/**
 * Add nodes preferentially.
 * Each node comes with 1 links.
 */
void NetworkBA::add_node_v0() {

    double p = rand() / double(RAND_MAX);

    double tmp{};
    uint sz = _nodes.size();
//    cout << "probability " << p << endl;
    for(uint i{}; i < sz; ++i){
        tmp += _nodes[i].neighborCount() / get_total_degree();
        if(tmp >= p){
//            cout << "selected node " << i << " with " << tmp << endl;
            // add node
            _nodes.push_back({sz});
            _nodes[i].addNeighbor(sz);
            _nodes[sz].addNeighbor(i);
            _total_degree += 2; // one link  increases neighborCount by 2.
            _links.push_back({sz, i});
            break;
        }
    }
}

/**
 * Add nodes preferentially.
 * Each node comes with m links.
 * m links are selected preferentially with respect to their degree.
 * 17.685811 sec for 100000 nodes
 */
void NetworkBA::add_node() {

//    add_node_v1();
//    add_node_v2();
    add_node_v3();

}

void NetworkBA::add_node_v1() {

    uint sz = _nodes.size();

    _node_indices.resize(sz); // 0.0072 sec when total time is 18.11 sec. 0.0398 % of total time

    // list of all existing nodes
    for(uint i{}; i < sz; ++i){ // 2.43 sec when total time is 20.32 sec. ~1% of total time
        _node_indices[i] = i;
    }

    auto t0 = chrono::_V2::system_clock::now();
    select_m_links_preferentially_v1(sz);

    auto t1 = chrono::_V2::system_clock::now();
    chrono::duration<double> drtion = t1 - t0;
    time += drtion.count();
    // adding new node
    _nodes.push_back({sz});


    // connecting new node with m preferentially selected links

    connect_with_m_nodes_v1(sz);
}

void NetworkBA::connect_with_m_nodes_v1(uint sz) {
    uint i{};
    for(uint k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].addNeighbor(i);
        _nodes[i].addNeighbor(sz);

        _total_degree += 2; // one link  increases neighborCount by 2.
        _links.push_back({sz, i});
    }
}


/**
 * m    Network_size    Time        Total_Time      percentage
 * 1    10,000          14.68 sec   19.12 sec       76%
 * 10   10,000          58.41 sec   61.77 sec       94.5%
 * 50   10,000          215.94 sec  220.84 sec      97.7%
 *
 * selecting m links preferentially.
 * set m_links such that there is no repetition.
 */
void NetworkBA::select_m_links_preferentially_v1(uint sz) {
    double p;
    double tmp{};
    for(uint j{}; j < _m; ++j){
        p = rand() / double(RAND_MAX);
        for(uint k{}; k < sz; ++k) {
            tmp += _nodes[_node_indices[k]].neighborCount() / _total_degree;
            if(tmp >= p) {
                _m_links[j] = _node_indices[k];
                _node_indices.erase(_node_indices.begin() + k);
                break;
            }
        }
    }

}


/**
 * Add nodes preferentially.
 * Each node comes with m links.
 * m links are selected preferentially with respect to their degree.
 * Much efficient than add_node_v2() especially for large m
 */
void NetworkBA::add_node_v2() {
    uint sz = _nodes.size();
    auto t0 = chrono::_V2::system_clock::now();

    // set m_links such that there is no repetition
    // selecting m links preferentially
    select_m_links_preferentially_v2(sz);
//    select_m_links_preferentially_v3_omp(sz);
    auto t1 = chrono::_V2::system_clock::now();
    chrono::duration<double> drtion = t1 - t0;
    time += drtion.count();

    // adding new node
    _nodes.push_back({sz});

    // connecting new node with m preferentially selected links
    connect_with_m_nodes_v2(sz);
}


/**
 * Add nodes preferentially.
 * Each node comes with m links.
 * m links are selected preferentially with respect to their degree.
 * Much efficient than add_node_v3()
 */
/**
 * Time statistics
 * m    Network_size        Total_Time          Memory_required
 * 1    1000,000            1.45916 sec
 * 2    1000,000            1.39551 sec
 * 10   1000,000            5.0823 sec          ~350 MB (run from IDE)
 * 20   1000,000            7.2886 sec          ~650 MB (run from IDE)
 * 30   1000,000            13.5757 sec         ~1.1 GB (run from IDE)
 * 40   1000,000            32.079 sec          ~1.3 GB (run from IDE)
 * 50	1000,000			18.71 sec 			~1.7 GB (run from terminal directly)
 * set m_links such that there is no repetition.
 * selecting m links preferentially.
 * Much efficient than previous versions
 */
void NetworkBA::add_node_v3() {
    uint sz = _nodes.size();
    auto t0 = chrono::_V2::system_clock::now();

    // set m_links such that there is no repetition
    // selecting m links preferentially
    select_m_links_preferentially_v3(sz);
    auto t1 = chrono::_V2::system_clock::now();
    chrono::duration<double> drtion = t1 - t0;
    time += drtion.count();

    // adding new node
    _nodes.push_back({sz});

    // connecting new node with m preferentially selected links
    connect_with_m_nodes_v3(sz);
}


void NetworkBA::connect_with_m_nodes_v2(uint sz) {
    uint i{};
    uint old_size = _links.size();
    _links.resize(old_size + _m);
    for(uint k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].addNeighbor(i);
        _nodes[i].addNeighbor(sz);

        _total_degree += 2; // one link  increases neighborCount by 2.
        _links[old_size + k] = Link(sz, i);
//        _links.push_back({sz, i});
    }
}

/**
 *
 * @param sz
 */
void NetworkBA::connect_with_m_nodes_v3(uint sz) {
    /*
     * updates _preferentially here
     */
    uint i{};

    size_t link_old_size = _links.size();
    _links.resize(link_old_size + _m);
    for(size_t k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].addNeighbor(i);
        _nodes[i].addNeighbor(sz);

        _total_degree += 2; // one link  increases neighborCount by 2.
        _links[link_old_size + k] = Link(sz, i);

        _preferentially.push_back(sz);
        _preferentially.push_back(i);

    }
}


/**
 *
 * @param sz
 */
void NetworkBA::connect_with_m_nodes_v4(uint sz) {
    /*
     * updates _preferentially here
     */
    uint i{};

    size_t prep_size = _preferentially.size();
    _preferentially.resize(prep_size + _m*2);

    size_t link_old_size = _links.size();
    _links.resize(link_old_size + _m);
    for(uint k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].addNeighbor(i);
        _nodes[i].addNeighbor(sz);

        _total_degree += 2; // one link  increases neighborCount by 2.
        _links[link_old_size + k] = Link(sz, i);

        _preferentially[prep_size + k] = sz;
        _preferentially[prep_size + k + _m] = i;
    }
}

/**
 * Time statistics
 * m    Network_size    Time        Total_Time      percentage(of total time)
 * 1    10,000          13.11 sec   13.208 sec      99.2%
 * 10   10,000          12.02 sec   12.24 sec       89.2%
 * 50   10,000          18.288 sec  19.249 sec      95.01%
 * 1    100,000         25.501 sec  25.863 sec
 * 1    200,000         1.739 min   1.752 min     99.2%
 * 1    300,000         4.93 min    4.967 min
 * 1    400,000
 * 1    500,000
 * set m_links such that there is no repetition.
 * selecting m links preferentially.
 */
void NetworkBA::select_m_links_preferentially_v2(uint sz) {
    double p;
    double tmp{};
    for(uint j{}; j < _m; ++j){
        p = rand() / double(RAND_MAX);
        for(uint k{}; k < sz; ++k) {
            tmp += _nodes[k].neighborCount() / _total_degree;
            if(tmp >= p) {
                if(_nodes[k].get_current_group() == sz){
//                    cout << "already selected " << __LINE__ << endl;
                    continue;
                }
                _m_links[j] = k;
                _nodes[k].set_current_group(sz);
                break;
            }
        }
    }
}

/**
 * Time statistics
 * m    Network_size    Time            Total_Time          Memory_required
 * 1    500,000         0.138717 sec    0.570831 sec
 * 1    1000,000        0.298458 sec    1.190535 sec
 * 2    100,000         0.0505323 sec   0.187127 sec
 * 2    500,000         0.193942 sec    0.714049 sec
 * 2    1000,000        0.456558 sec    1.657095 sec
 * 10   1000,000        8.22383 sec     17.452118 sec
 * 50   1000,000
 *
 * set m_links such that there is no repetition.
 * selecting m links preferentially.
 * Much efficient than previous versions
 */
void NetworkBA::select_m_links_preferentially_v3(uint sz) {
    size_t r, index;
    for(size_t j=0; j < _m; ++j){
        r = rand() % _preferentially.size();
        index = _preferentially[r];
        if(_nodes[index].get_current_group() == sz){
//            cout << index << " id already selected " << __LINE__ << endl;
            --j; // to do this step again
            continue;
        }
//        cout << "selecting " << index << endl;
        _m_links[j] = index;
        _nodes[index].set_current_group(sz);
    }

//    cout << "m_links : line " << __LINE__ << endl;
//    cout << "{";
//    for(uint i{}; i < _m; ++i){
//        cout << _m_links[i] << ',';
//    }
//    cout << '}' << endl;
}

void NetworkBA::select_m_links_preferentially_v4(uint sz) {
    size_t r, index;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with _random_device()
    std::uniform_int_distribution<size_t> dis(0, _preferentially.size()-1);

    for(size_t j=0; j < _m; ++j){
        r =  dis(gen);
        index = _preferentially[r];
        if(_nodes[index].get_current_group() == sz){
//            cout << index << " id already selected " << __LINE__ << endl;
            --j; // to do this step again
            continue;
        }
//        cout << "selecting " << index << endl;
        _m_links[j] = index;
        _nodes[index].set_current_group(sz);
    }

//    cout << "m_links : line " << __LINE__ << endl;
//    cout << "{";
//    for(uint i{}; i < _m; ++i){
//        cout << _m_links[i] << ',';
//    }
//    cout << '}' << endl;
}

/**
 * Initialize the preferentially array.
 */
void NetworkBA::initialize_preferential() {
    size_t d, sz;
    for(size_t i{}; i < _nodes.size(); ++i) {
        d = _nodes[i].neighborCount();
        sz = _preferentially.size();
        _preferentially.resize(sz + d);
        for(uint j{}; j < d; ++j) {
            // _preferentially uses 4 byte to store element.
            // cannot increase it because ~1,000,000 element will take huge Memory
            _preferentially[sz + j] = i;
        }
    }

}

void NetworkBA::view_preferentially() {
    cout << "preferentially {";
    for(size_t i{}; i < _preferentially.size(); ++i){
        cout << _preferentially[i] << ',';
    }
    cout << '}' << endl;
}

//
///*************************
// * Percolation on BA network
// */
//NetworkBApercolation::NetworkBApercolation(size_t m0, size_t m, size_t size)
//    : _m0{m0}, _m{m}
//{
//    initiate(m0, m, size);
//}
//
//void NetworkBApercolation::initiate(size_t m0, size_t m, size_t size) {
//    _m0 = m0;
//    _m = m;
//    _network_size = size;
//    initialize_network();
//    _link_count = _network_frame.getNumberOfLinks();
//    _link_indices.resize(_link_count);
//    _randomized_indices.resize(_link_count);
//    for(uint i = 0; i < _link_count; ++i){
//        _link_indices[i] = i;
//    }
////    cout << _link_indices.size() << " : " << _link_indices << endl;
//    randomize_v1();
//}
//
//void NetworkBApercolation::initialize_network() {
//    _network_frame = NetworkBA(_m0, _m);
//    uint i=0;
//    while (i < _network_size){
//        _network_frame.add_node();
//        ++i;
//    }
//}
//
//
//void NetworkBApercolation::randomize_v0() {
//
//    _randomized_indices = _link_indices;
//
//    uint tmp, r;
//    for(uint i{}; i < _link_count; ++i) {
//        r = rand() % _link_count;
//        tmp = _randomized_indices[i];
//        _randomized_indices[i] = _randomized_indices[r];
//        _randomized_indices[r] = tmp;
//    }
////    cout << "original " << _link_indices << endl;
////    cout << "randomized " << _randomized_indices << endl;
//}
//
///**
// * Uses Better Random number generator from stdard library
// */
//void NetworkBApercolation::randomize_v1() {
//
//    _randomized_indices = _link_indices;
//
//    std::random_device _random_device;
//    std::mt19937 g(_random_device());
//
////    cout << "before " << _randomized_indices << endl;
//    std::shuffle(_randomized_indices.begin(), _randomized_indices.end(), g);
////    cout << "after " << _randomized_indices << endl;
//
//}
//
//bool NetworkBApercolation::occupy_link() {
//    if (index_var >= _link_count){
//        return false;
//    }
////    cout << index_var << " ==? " << _randomized_indices.size();
//    // select a link randomly
//    size_t last_link_pos_in_randomized = _randomized_indices[index_var];
//    ++index_var;
//    _last_lnk = _network_frame.getLink(last_link_pos_in_randomized);
//    _network_frame.activateLink(last_link_pos_in_randomized); // activating the link
//
//    // occupy that link
//    ++_number_of_occupied_links;
//
//    // cluster management
//    manage_cluster(last_link_pos_in_randomized);
//
//    return true;
//}
//
///**
// *
// * @param pos : radnomized position of the link index
// */
//void NetworkBApercolation::manage_cluster(size_t pos) {
//    Link lnk = _network_frame.getLink(pos);
//    // getting all details
////    int id = lnk.get_group_id();
//    uint node_a = lnk.get_a();
//    int id_a = _network_frame.get_node_group_id(node_a);
//    uint node_b = lnk.get_b();
//    int id_b = _network_frame.get_node_group_id(node_b);
////    cout << "a " << id_a << " and b " << id_b << endl;
//    // if both nodes have id -1. means they are not part of any cluster
//    if(id_a == -1 && id_b == -1){
////        cout << "################ new cluster ################ : line " <<__LINE__ << endl;
//        size_t  sz = _cluster.size();
//        _cluster.push_back({});
//        _cluster[sz].add_link(lnk);
//        _cluster[sz].add_node(node_a);
//        _cluster[sz].add_node(node_b);
//        _cluster[sz].set_group_id(group_id_count);
//        _network_frame.set_node_group_id(node_a, group_id_count);
//        _network_frame.set_node_group_id(node_b, group_id_count);
//        _network_frame.set_link_group_id(pos, group_id_count); // must set value in the global array. not local
//        _cluster_index_from_id.insert(group_id_count);
//        ++group_id_count;
//        if(_number_of_nodes_in_the_largest_cluster < _cluster[sz].numberOfNodes()){
//            _number_of_nodes_in_the_largest_cluster = _cluster[sz].numberOfNodes();
//        }
//    }else if(id_a != -1 && id_b == -1){
//        // add to id_a
//        size_t index = _cluster_index_from_id[id_a];
//        _cluster[index].add_link(lnk);
//        _cluster[index].add_node(node_b); // since node_a is already in the cluster
//        _network_frame.set_node_group_id(node_b, id_a);
//        _network_frame.set_link_group_id(pos, id_a);
//        if(_number_of_nodes_in_the_largest_cluster < _cluster[index].numberOfNodes()){
//            _number_of_nodes_in_the_largest_cluster = _cluster[index].numberOfNodes();
//        }
//    }
//    else if(id_a == -1 && id_b != -1){
//        // add to id_b
//        size_t index = _cluster_index_from_id[id_b];
//        _cluster[index].add_link(lnk);
//        _cluster[index].add_node(node_a); // since node_a is already in the cluster
//        _network_frame.set_node_group_id(node_a, id_b);
//        _network_frame.set_link_group_id(pos, id_b);
//        if(_number_of_nodes_in_the_largest_cluster < _cluster[index].numberOfNodes()){
//            _number_of_nodes_in_the_largest_cluster = _cluster[index].numberOfNodes();
//        }
//    } else if(id_a == id_b){
//        // not -1 but same then just insert the link
//        size_t index = _cluster_index_from_id[id_b];
//        _cluster[index].add_link(lnk);
//        _network_frame.set_link_group_id(pos, id_b);
//    }
//    else{
//        /// merge cluster
//        // no need to add nodes to the cluster. they are already there
//        size_t index_a = _cluster_index_from_id[id_a];
//        size_t index_b = _cluster_index_from_id[id_b];
//        // base is the larger cluster (number of nodes in it)
//        size_t size_a = _cluster[index_a].numberOfNodes();
//        size_t size_b = _cluster[index_b].numberOfNodes();
//        if(size_a > size_b){
//            // index_a will survive the process
//            _cluster[index_a].add_link(lnk);
//            _network_frame.set_node_group_id(node_b, id_a);
//            _network_frame.set_link_group_id(pos, id_a);
//            _cluster[index_a].insert(_cluster[index_b]);
//            // relabeling
//            relabel_nodes(_cluster[index_b], id_a);
//            _cluster.erase(_cluster.begin() + index_b);
//            _cluster_index_from_id.erase(id_b);
//
//        }else{
//            // index_b will survive the process
//            _cluster[index_b].add_link(lnk);
//            _network_frame.set_node_group_id(node_a, id_b);
//            _network_frame.set_link_group_id(pos, id_b);
//            _cluster[index_b].insert(_cluster[index_a]);
//            relabel_nodes(_cluster[index_a], id_b);
//            _cluster.erase(_cluster.begin() + index_a);
//            _cluster_index_from_id.erase(id_a);
//        }
//        if(_number_of_nodes_in_the_largest_cluster < (size_a + size_b)){
//            _number_of_nodes_in_the_largest_cluster = size_a + size_b;
//        }
//
//    }
//
//}
//
//void NetworkBApercolation::viewCluster() {
//    for(size_t i{}; i < _cluster.size(); ++i){
//        cout << "cluster[" << i << "] : id " << _cluster[i].get_group_id() << "{" << endl;
//        cout << "  Nodes (" << _cluster[i].numberOfNodes() << "): ";
//        _cluster[i].viewNodes();
//        cout << endl;
//        cout << "  Links (" << _cluster[i].numberOfLinks() << "): ";
//        _cluster[i].viewLinks();
//        cout << endl;
//        cout << "}" << endl;
//    }
//
//}
//
//void NetworkBApercolation::viewClusterExtended() {
//    for(size_t i{}; i < _cluster.size(); ++i){
//        cout << "cluster[" << i << "] : id " << _cluster[i].get_group_id() << "{" << endl;
//        cout << "  Nodes (" << _cluster[i].numberOfNodes() << "): ";
//        auto nds = _cluster[i].get_nodes();
//        cout << "(index, id)->";
//        for(auto n: nds){
//            cout << "(" << n << "," << _network_frame.get_node_group_id(n) << "),";
//        }
//        cout << endl;
//        cout << "  Links (" << _cluster[i].numberOfLinks() << "): ";
//        _cluster[i].viewLinksExtended();
//        cout << endl;
//        cout << "}" << endl;
//    }
//
//}
//
//void NetworkBApercolation::relabel_nodes(Cluster& clstr, int id) {
//    auto nds = clstr.get_nodes();
//    for(size_t i{}; i < nds.size(); ++i){
//        _network_frame.set_node_group_id(nds[i],id);
//    }
//}
//
///********************************
// * Explosive percolation on BA network
// */
///**
// *
// * @param m0
// * @param m
// * @param size
// * @param M
// */
//NetworkBApercolationExplosive::NetworkBApercolationExplosive(size_t m0, size_t m, size_t size, uint M)
//        : NetworkBApercolation(m0, m, size)
//{
//    if (M > 2) {
//        if(M > MAX_M_VALUE){
//            cout << "M exceeds maximum allowed value : line " << __LINE__ << endl;
//            _M = MAX_M_VALUE;
//        }
//        _M = M;
//    }
//}
//
//bool NetworkBApercolationExplosive::occupy_link() {
//    if (_number_of_occupied_links >= _link_count){
//        return false;
//    }
////    cout << index_var << " ==? " << _randomized_indices.size();
//    // select M links and occupy that one which makes the cluster size minimum
//    size_t r = link_for_min_cluster_sum_rule();
//    size_t pos = _randomized_indices[r];
//    _randomized_indices.erase(_randomized_indices.begin() + r); // must erase the used value
//    _last_lnk = _network_frame.getLink(pos);
//    _network_frame.activateLink(pos); // activating the link
//
//    // occupy that link
//    ++_number_of_occupied_links;
//
//    // cluster management
//    manage_cluster(pos);
//
//    return true;
//}
//
///**
// * For minimizing cluster sizes using sum rule
//
// * @return index of the link in the _randomized_indices for which cluster size become minimum
// */
//size_t NetworkBApercolationExplosive::link_for_min_cluster_sum_rule() {
//    size_t index_randomized_link;
//
//    size_t tmp_lnk_index, index1, index2;
//    int id1, id2;
//    size_t sum{}, n_nodes;
//    Link tmp_lnk;
//    for(size_t i{}; i < _M; ++i){
//        if(i >= _randomized_indices.size()){
//            cout << "not enough free link : line " << __LINE__ << endl;
//            break;
//        }
//        id1 = -1;
//        id2 = -1;
//        tmp_lnk_index = _randomized_indices[i];
//        tmp_lnk = _network_frame.getLink(tmp_lnk_index);
//        if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
//             // since we are minimizing cluster sizes
//            index_randomized_link = i;
//            cout << "got link " << _network_frame.getLink(_randomized_indices[i]) << " id = " << id1 << " and " << id2 << endl;
//            break; // since this is the minimum link case
//        }
//        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) != -1){
//            id1 = _network_frame.get_node_group_id(tmp_lnk.get_b());
//            index1 = _cluster_index_from_id[id1];
//            n_nodes = _cluster[index1].numberOfNodes() + 1; // 1 node would have been added
//            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
//                sum = n_nodes;
//                index_randomized_link = i;
//            }
//        }
//        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) != -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
//            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
//            index1 = _cluster_index_from_id[id1];
//            n_nodes = _cluster[index1].numberOfNodes() + 1; // 1 node would have been added
//            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
//                sum = n_nodes;
//                index_randomized_link = i;
//            }
//        }
//        else{
//
//            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
//            id2 = _network_frame.get_node_group_id(tmp_lnk.get_b());
//            if(id1 == id2){
//                // if they belong to same cluster. adding new link does not increase cluster size.
//                index1 = _cluster_index_from_id[id1];
//                n_nodes = _cluster[index1].numberOfNodes();
//                cout << "same cluster " << endl;
//            }else {
//                index1 = _cluster_index_from_id[id1];
//                index2 = _cluster_index_from_id[id2];
//                n_nodes = _cluster[index1].numberOfNodes() + _cluster[index2].numberOfNodes();
//            }
//            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
//                sum = n_nodes;
//                index_randomized_link = i;
//            }
//        }
//        cout << "checking link " << _network_frame.getLink(_randomized_indices[i])
//             << " id = " << id1 << " and " << id2 << " sum= " << sum << endl;
//    }
//    if(index_randomized_link >= _randomized_indices.size()){
//        cout << "out of bound : line " << __LINE__ << endl;
//    }
//    cout << "selected link " << _network_frame.getLink(_randomized_indices[index_randomized_link])
//            << " id = " << id1 << " and " << id2 << " sum= " << sum << endl;
//    return index_randomized_link;
//}
//
//
///**
// * For minimizing cluster sizes using sum rule
//
// * @return index of the link in the _randomized_indices for which cluster size become minimum
// */
//size_t NetworkBApercolationExplosive::link_for_min_cluster_product_rule() {
//    size_t index_randomized_link;
//
//    size_t tmp_lnk_index, index1, index2;
//    int id1, id2;
//    size_t prod{1}, n_nodes;
//    Link tmp_lnk;
//    for(size_t i{}; i < _M; ++i){
//        if(i >= _randomized_indices.size()){
//            cout << "not enough free link : line " << __LINE__ << endl;
//            break;
//        }
//        id1 = -1;
//        id2 = -1;
//        tmp_lnk_index = _randomized_indices[i];
//        tmp_lnk = _network_frame.getLink(tmp_lnk_index);
//        if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
//            // since we are minimizing cluster sizes
//            index_randomized_link = i;
//            cout << "got link " << _network_frame.getLink(_randomized_indices[i]) << " id = " << id1 << " and " << id2 << endl;
//            break; // since this is the minimum link case
//        }
//        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) != -1){
//            id1 = _network_frame.get_node_group_id(tmp_lnk.get_b());
//            index1 = _cluster_index_from_id[id1];
//            n_nodes = _cluster[index1].numberOfNodes(); // 1 node would have been multiplied
//            if(prod == 1 || prod > n_nodes) { // since we are minimizing cluster sizes
//                prod = n_nodes;
//                index_randomized_link = i;
//            }
//        }
//        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) != -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
//            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
//            index1 = _cluster_index_from_id[id1];
//            n_nodes = _cluster[index1].numberOfNodes(); // 1 node would have been multiplied
//            if(prod == 1 || prod > n_nodes) { // since we are minimizing cluster sizes
//                prod = n_nodes;
//                index_randomized_link = i;
//            }
//        }
//        else{
//
//            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
//            id2 = _network_frame.get_node_group_id(tmp_lnk.get_b());
//            if(id1 == id2){
//                // if they belong to same cluster. adding new link does not increase cluster size.
//                index1 = _cluster_index_from_id[id1];
//                n_nodes = _cluster[index1].numberOfNodes();
//                cout << "same cluster " << endl;
//            }else {
//                index1 = _cluster_index_from_id[id1];
//                index2 = _cluster_index_from_id[id2];
//                n_nodes = _cluster[index1].numberOfNodes() * _cluster[index2].numberOfNodes(); // product rule
//            }
//            if(prod == 1 || prod > n_nodes) { // since we are minimizing cluster sizes
//                prod = n_nodes;
//                index_randomized_link = i;
//            }
//        }
//        cout << "checking link " << _network_frame.getLink(_randomized_indices[i])
//             << " id = " << id1 << " and " << id2 << " sum= " << prod << endl;
//    }
//    if(index_randomized_link >= _randomized_indices.size()){
//        cout << "out of bound : line " << __LINE__ << endl;
//    }
//    cout << "selected link " << _network_frame.getLink(_randomized_indices[index_randomized_link])
//         << " id = " << id1 << " and " << id2 << " sum= " << prod << endl;
//    return index_randomized_link;
//}
//
//
//
