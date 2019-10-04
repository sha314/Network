//
// Created by shahnoor on 12/24/18.
//


#include "../network.h"

using  namespace std;

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
void NetworkBA::addNode() {
    if(_static){
        cout << "network is frozen since preferentially is empty : line " << __LINE__ << endl;
        return;
    }

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
    select_m_nodes_preferentially_v1(sz);

    auto t1 = chrono::_V2::system_clock::now();
    chrono::duration<double> drtion = t1 - t0;
    time_select_m_nodes_preferentially_v3 += drtion.count();
    // adding new node
    _nodes.push_back({sz});


    // connecting new node with m preferentially selected links

    connect_with_m_nodes_v1(sz);
}

void NetworkBA::connect_with_m_nodes_v1(uint sz) {
    uint i{};
    for(uint k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_nodes[k];
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
void NetworkBA::select_m_nodes_preferentially_v1(uint sz) {
    double p;
    double tmp{};
    for(uint j{}; j < _m; ++j){
        p = rand() / double(RAND_MAX);
        for(uint k{}; k < sz; ++k) {
            tmp += _nodes[_node_indices[k]].neighborCount() / _total_degree;
            if(tmp >= p) {
                _m_nodes[j] = _node_indices[k];
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
    select_m_nodes_preferentially_v2(sz);
//    select_m_links_preferentially_v3_omp(sz);
    auto t1 = chrono::_V2::system_clock::now();
    chrono::duration<double> drtion = t1 - t0;
    time_select_m_nodes_preferentially_v3 += drtion.count();

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
    select_m_nodes_preferentially_v3(sz);

    auto t1 = chrono::_V2::system_clock::now();
    chrono::duration<double> drtion = t1 - t0;
    time_select_m_nodes_preferentially_v3 += drtion.count();

    // adding new node
    _nodes.push_back({sz});

    // connecting new node with m preferentially selected links
    t0 = chrono::_V2::system_clock::now();
    connect_with_m_nodes_v3(sz);
    t1 = chrono::_V2::system_clock::now();
    drtion = t1 - t0;
    time_connect_with_m_nodes_v3 += drtion.count();

}


void NetworkBA::connect_with_m_nodes_v2(uint sz) {
    uint i{};
    uint old_size = _links.size();
    _links.resize(old_size + _m);
    for(uint k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_nodes[k];
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
        i = _m_nodes[k];
        _nodes[sz].addNeighbor(i);
        _nodes[i].addNeighbor(sz);

        _total_degree += 2; // one link  increases total degree by 2.
        _links[link_old_size + k] = Link(i, sz);

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
        i = _m_nodes[k];
        _nodes[sz].addNeighbor(i);
        _nodes[i].addNeighbor(sz);

        _total_degree += 2; // one link  increases neighborCount by 2.
        _links[link_old_size + k] = Link(i, sz);

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
void NetworkBA::select_m_nodes_preferentially_v2(uint sz) {
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
                _m_nodes[j] = k;
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
void NetworkBA::select_m_nodes_preferentially_v3(uint sz) {
    size_t r, index;
    for(size_t j=0; j < _m; ++j){
//        r = rand() % _preferentially.size();
        r = _random() % _preferentially.size();
        index = _preferentially[r];
        if(_nodes[index].get_current_group() == sz){
//            cout << index << " id already selected " << __LINE__ << endl;
            --j; // to do this step again
            continue;
        }
//        cout << "selecting " << index << endl;
        _m_nodes[j] = index;
//        _nodes[index].set_current_group(sz); // is it necessary??
    }

//    cout << "m_links : line " << __LINE__ << endl;
//    cout << "{";
//    for(uint i{}; i < _m; ++i){
//        cout << _m_links[i] << ',';
//    }
//    cout << '}' << endl;
}

void NetworkBA::select_m_nodes_preferentially_v4(uint sz) {
    size_t r, index;
    std::uniform_int_distribution<size_t> dis(0, _preferentially.size()-1);
    for(size_t j=0; j < _m; ++j){
        r =  dis(_random);
        index = _preferentially[r];
        if(_nodes[index].get_current_group() == sz){
//            cout << index << " id already selected " << __LINE__ << endl;
            --j; // to do this step again
            continue;
        }
//        cout << "selecting " << index << endl;
        _m_nodes[j] = index;
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

void NetworkBA::reset() {
    Network::reset();
    _preferentially.clear();
    _static = false;
    initialize_preferential();
}

double NetworkBA::size_in_MB() {

    double sz = 0;
//    sz += sizeof(double) + sizeof(bool) + sizeof(std::vector<uint>);
    sz += sizeof(uint) * _preferentially.capacity();
    sz /= (1024 * 1024);  // converting to MB
    sz += Network::size_in_MB();
    return sz;
}

void NetworkBA::shrink_to_fit() { // frequently calling this function will reduce performance
    _preferentially.shrink_to_fit();
    Network::shrink_to_fit();
}


bool NetworkBA::grow(size_t netowk_size) {
    if(netowk_size <= _N_size){
        cout << "cannot grow" << endl;
        return false;
    }
    for(size_t i{_N_size}; i < netowk_size; ++i){
        addNode();
    }
<<<<<<< HEAD
    N = netowk_size;
=======
    _N_size = netowk_size;
>>>>>>> v5
    return true;
}

void NetworkBA::timeElapsed() {
    cout << "time_select_m_nodes_preferentially_v3 " << time_select_m_nodes_preferentially_v3 << " sec" << endl;
    cout << "time_connect_with_m_nodes_v3 " << time_connect_with_m_nodes_v3 << " sec" << endl;
    cout << "Total " << time_select_m_nodes_preferentially_v3 +  time_connect_with_m_nodes_v3 << " sec" << endl;
}
