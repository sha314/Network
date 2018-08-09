//
// Created by shahnoor on 8/7/18.
//

#include <chrono>
#include "network.h"

using  namespace std;



/*****************************
 * Network Methods
 ****************************/

/**
 *
 * @param m0 : number of initial nodes
 * @param m  : number of links new nodes are born with
 */
Network::Network(size_t m0, size_t m) {
    if (m0 <= m){
        _m0 = m + 1;
        _m = m;
    }else{
        _m0 = m0;
        _m = m;
    }

    initialize();

}

void Network::initialize() {
    for(size_t i{}; i != _m0; ++i){
        _nodes.push_back({i}); // add node with id equal to the size of the _nodes
    }
    size_t k{};
    for(size_t i{}; i != _m0; ++i){
        k = (i+1) % _m0;
        _nodes[i].add_neighbor(k);
        _nodes[k].add_neighbor(i);
        _total_degree += 2;
        _links.push_back({k, i});
    }
    _m_links.resize(_m);

}


/**
 * Add a new node to the network and connect with with_node nodes
 * @param with_node
 */
void Network::add_node(std::vector<size_t> with_node) {
    size_t sz = {_nodes.size()};
    _nodes.push_back({sz}); // add node with id equal to the size of the _nodes
    for(size_t n: with_node){
        _nodes[sz].add_neighbor(n);
        _nodes[n].add_neighbor(sz);
        _total_degree += 2; // one link  increases degree by 2.
        _links.push_back({sz, n});
    }
}

void Network::add_node(size_t with_node) {
    size_t sz = {_nodes.size()};
    _nodes.push_back({sz}); // add node with id equal to the size of the _nodes
    _nodes[sz].add_neighbor(with_node);
    _nodes[with_node].add_neighbor(sz);
    _total_degree += 2; // one link  increases degree by 2.
    _links.push_back({sz, with_node});
}
/**
 * Adds node randomly
 * randomly select m nodes with which new node will be connected
 */
void Network::add_node() {
    size_t sz = _nodes.size();
    _nodes.push_back({sz}); // add node with id equal to the size of the _nodes


//    vector<size_t> m_links(_m), node_indices(sz);
    _node_indices.resize(_nodes.size());

    // list of all existing nodes
    for(size_t i{}; i < sz; ++i){
        _node_indices[i] = i;
    }
    size_t r{};

    // set m_links such that there is no repetition
    for(size_t j{}; j < _m; ++j){
        r = rand() % _node_indices.size();
        _m_links[j] = _node_indices[r];
        _node_indices.erase(_node_indices.begin() + r); // so that its not get selected again
    }

    // setting m links in the netrowk
    size_t i{};
    for(size_t k{}; k < _m; ++k){
        i = _m_links[k];
        _nodes[i].add_neighbor(sz);
        _nodes[sz].add_neighbor(i);
        _total_degree += 2; // one link  increases degree by 2.
        _links.push_back({sz, i});
    }

}


/**
 *
 */
void Network::view_nodes() {
    cout << "id(size):{neighbors,...}" << endl;
    for(size_t i{}; i < _nodes.size(); ++i){
        cout << _nodes[i].get_id() << "(" << _nodes[i].degree() << "):{";
        auto neighbors = _nodes[i].get_neighbors();
        for(auto n: neighbors){
            cout << n << ',';
        }
        cout <<  "}" << endl;
    }
}

void Network::view_links() {
    cout << _links.size() << " links {" ;
    for(size_t i{}; i < _links.size(); ++i){
        cout << _links[i] << ',';
    }
    cout << "}" << endl;
}

vector<double> Network::degrees() {
    vector<double> degs(_nodes.size());

    for(size_t i{}; i < _nodes.size(); ++i){
        degs[i] = _nodes[i].degree();
    }
//    cout << degs.size() << " : line " << __LINE__ << endl;
//    cout << "line " << __LINE__ << endl;
    return degs;
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
    size_t sz = _nodes.size();
//    cout << "probability " << p << endl;
    for(size_t i{}; i < sz; ++i){
        tmp +=  _nodes[i].degree() / get_total_degree();
        if(tmp >= p){
//            cout << "selected node " << i << " with " << tmp << endl;
            // add node
            _nodes.push_back({sz});
            _nodes[i].add_neighbor(sz);
            _nodes[sz].add_neighbor(i);
            _total_degree += 2; // one link  increases degree by 2.
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

    size_t sz = _nodes.size();

    _node_indices.resize(sz); // 0.0072 sec when total time is 18.11 sec. 0.0398 % of total time

    // list of all existing nodes
    for(size_t i{}; i < sz; ++i){ // 2.43 sec when total time is 20.32 sec. ~1% of total time
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

void NetworkBA::connect_with_m_nodes_v1(size_t sz) {
    size_t i{};
    for(size_t k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].add_neighbor(i);
        _nodes[i].add_neighbor(sz);

        _total_degree += 2; // one link  increases degree by 2.
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
void NetworkBA::select_m_links_preferentially_v1(size_t sz) {
    double p;
    double tmp{};
    for(size_t j{}; j < _m; ++j){
        p = rand() / double(RAND_MAX);
        for(size_t k{}; k < sz; ++k) {
            tmp += _nodes[_node_indices[k]].degree() / _total_degree;
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
    size_t sz = _nodes.size();
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
void NetworkBA::add_node_v3() {
    size_t sz = _nodes.size();
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


void NetworkBA::connect_with_m_nodes_v2(size_t sz) {
    size_t i{};
    size_t old_size = _links.size();
    _links.resize(old_size + _m);
    for(size_t k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].add_neighbor(i);
        _nodes[i].add_neighbor(sz);

        _total_degree += 2; // one link  increases degree by 2.
        _links[old_size + k] = Link(sz, i);
//        _links.push_back({sz, i});
    }
}

/**
 *
 * @param sz
 */
void NetworkBA::connect_with_m_nodes_v3(size_t sz) {
    /*
     * updates _preferentially here
     */
    size_t i{};

    size_t link_old_size = _links.size();
    _links.resize(link_old_size + _m);
    for(size_t k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].add_neighbor(i);
        _nodes[i].add_neighbor(sz);

        _total_degree += 2; // one link  increases degree by 2.
        _links[link_old_size + k] = Link(sz, i);

        _preferentially.push_back(sz);
        _preferentially.push_back(i);

    }
}


/**
 *
 * @param sz
 */
void NetworkBA::connect_with_m_nodes_v4(size_t sz) {
    /*
     * updates _preferentially here
     */
    size_t i{};

    size_t prep_size = _preferentially.size();
    _preferentially.resize(prep_size + _m*2);

    size_t link_old_size = _links.size();
    _links.resize(link_old_size + _m);
    for(size_t k{}; k < _m; ++k){ // 0.058 sec when total time is 19.05 sec. ~0.25 % of total time
        i = _m_links[k];
        _nodes[sz].add_neighbor(i);
        _nodes[i].add_neighbor(sz);

        _total_degree += 2; // one link  increases degree by 2.
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
void NetworkBA::select_m_links_preferentially_v2(size_t sz) {
    double p;
    double tmp{};
    for(size_t j{}; j < _m; ++j){
        p = rand() / double(RAND_MAX);
        for(size_t k{}; k < sz; ++k) {
            tmp += _nodes[k].degree() / _total_degree;
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
 * 10   1000,000        8.22383 sec     17.452118 sec       ~600 MB
 * 50   1000,000
 *
 * set m_links such that there is no repetition.
 * selecting m links preferentially.
 * Much efficient than previous versions
 */
void NetworkBA::select_m_links_preferentially_v3(size_t sz) {
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
//    for(size_t i{}; i < _m; ++i){
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
        d = _nodes[i].degree();
        sz = _preferentially.size();
        _preferentially.resize(sz + d);
        for(size_t j{}; j < d; ++j) {
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

/*************************
 * Percolation on BA network
 */
NetworkBApercolation::NetworkBApercolation(size_t m0, size_t m, size_t size)
        : NetworkBA(m0, m), _network_size{size}
{
    size_t i=0;
    while (i < _network_size){
        NetworkBA::add_node();
        ++i;
    }

    _link_count = _links.size();
    _link_indices.resize(_link_count);
    _randomized_indices.resize(_link_count);
    for(size_t i{}; i < _link_count; ++i){
        _link_indices[i] = i;
    }

    randomize();
}

void NetworkBApercolation::randomize() {
    size_t r;
    for(size_t i{}; i < _link_count; ++i) {
        r = rand() % _link_count;
        _randomized_indices[i] = _link_indices[r];
        _randomized_indices[r] = _link_indices[i];
    }
}

bool NetworkBApercolation::occupy_link() {
    if (index_var > _link_count){
        return false;
    }
    // select a link randomly
    _last_lnk = &_links[_randomized_indices[index_var]];
    ++index_var;
    _last_lnk->activate(); // activating the link

    // occupy that link

    // cluster management
    return false;
}
