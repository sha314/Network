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
    _m0 = m0;
    _m = m;
    if (m0 <= m){ _m0 += m;}
    if (_m0 < 3){_m0=3;}
//    initialize();
    initialize_v2();

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
    _m_nodes.resize(_m);

}
/**
 * link label follow a convention. for example (a,b) link means that a < b
 */
void Network::initialize_v2() {
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
        if(k < i) {
            _links.push_back({k, i});
        }else{
            _links.push_back({i, k});
        }
    }
    _m_nodes.resize(_m);

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
 * v1 is faster method than v2
 */
void Network::add_node_v1() {
    size_t sz = _nodes.size();
    auto sz2 = uint(sz);
    _nodes.push_back({sz2}); // add node with id equal to the size of the _nodes


//    vector<uint> m_links(_m), node_indices(sz);
    _node_indices.resize(_nodes.size());

    // list of all existing nodes
    for(uint i{}; i < sz; ++i){
        _node_indices[i] = i;
    }

    size_t r{};
    // set m_links such that there is no repetition
    // setting m links in the netrowk
    uint i{};
    for(uint k{}; k < _m; ++k){
        r = _random() % _node_indices.size();
        i = _node_indices[r];
        _node_indices.erase(_node_indices.begin() + r); // so that its not get selected again
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

    auto t0 = chrono::system_clock::now();
    std::shuffle(_node_indices.begin(), _node_indices.end(), _random);
    auto t1 = chrono::system_clock::now();
    shuffle_time += (t1-t0);
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
void Network::viewNodes() {
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

/**
 *
 */
void Network::viewNodesExtended() {
    cout << "id(group id, size):{neighbors,...}" << endl;
    for(uint i{}; i < _nodes.size(); ++i){
        cout << _nodes[i].get_id()
             << "(" << _nodes[i].get_group_id()
             << "," << _nodes[i].neighborCount() << "):{";
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

vector<uint> Network::degrees() {
    vector<uint> degs(_nodes.size());

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

double Network::size_in_MB() {

    double sz = 0;
//    sz += sizeof(std::vector<Node>) + sizeof(std::vector<Link>);
    sz += sizeof(Link) * _links.capacity(); // most important


//    sz += sizeof(std::vector<uint>) + sizeof(std::vector<uint>);
    sz += sizeof(uint) * _node_indices.capacity() + sizeof(uint) * _m_nodes.capacity(); // most important
//    sz += sizeof(uint)*2 + sizeof(double);
//    sz += sizeof(std::mt19937);
//    sz += sizeof(std::chrono::duration<double>);

//    sz += sizeof(Network); // network class itself

    //    sz += sizeof(Node) * _nodes.capacity(); // most important

    sz /= (1024 * 1024);
    for(size_t i{}; i < _nodes.size(); ++i){ // most important
        sz += _nodes[i].size_in_MB();
    }
    return sz ;
}

void Network::shrink_to_fit() {
    _nodes.shrink_to_fit();
    for(size_t i{}; i < _nodes.size(); ++i){
        _nodes[i].shrink_to_fit();
    }
    _links.shrink_to_fit();
    _node_indices.shrink_to_fit();
    _m_nodes.shrink_to_fit();
}

void Network::setRandomState(size_t seed, bool g) {
    _random_state = seed;
    if(g){
        std::random_device rd;
        _random_state = rd();
    }
    cout << "seed Network: " << _random_state << endl;
    _random.seed(_random_state); // seeding
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
//    _randomized_link_indices.resize(_link_count);
//    _randomized_link_indices.resize(_link_count);
//    for(uint i = 0; i < _link_count; ++i){
//        _randomized_link_indices[i] = i;
//    }
////    cout << _link_indices.size() << " : " << _link_indices << endl;
//    randomize_v1();
//}
//
//void NetworkBApercolation::initialize_network() {
//    _network_frame = NetworkBA(_m0, _m);
//    uint i=0;
//    while (i < _network_size){
//        _network_frame.add_node_v1();
//        ++i;
//    }
//}
//
//
//void NetworkBApercolation::randomize_v0() {
//
//    _randomized_link_indices = _randomized_link_indices;
//
//    uint tmp, r;
//    for(uint i{}; i < _link_count; ++i) {
//        r = rand() % _link_count;
//        tmp = _randomized_link_indices[i];
//        _randomized_link_indices[i] = _randomized_link_indices[r];
//        _randomized_link_indices[r] = tmp;
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
//    _randomized_link_indices = _randomized_link_indices;
//
//    std::random_device _random_device;
//    std::mt19937 g(_random_device());
//
////    cout << "before " << _randomized_indices << endl;
//    std::shuffle(_randomized_link_indices.begin(), _randomized_link_indices.end(), g);
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
//    size_t last_link_pos_in_randomized = _randomized_link_indices[index_var];
//    ++index_var;
//    _last_lnk = _network_frame.getLink(last_link_pos_in_randomized);
//    _network_frame.activateLink(last_link_pos_in_randomized); // activating the link
//
//    // occupy that link
//    ++_number_of_occupied_links;
//
//    // cluster management
//    manage_cluster_v0(last_link_pos_in_randomized);
//
//    return true;
//}
//
///**
// *
// * @param pos : radnomized position of the link index
// */
//void NetworkBApercolation::manage_cluster_v0(size_t pos) {
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
//        _cluster[sz].add_node_v1(node_a);
//        _cluster[sz].add_node_v1(node_b);
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
//        _cluster[index].add_node_v1(node_b); // since node_a is already in the cluster
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
//        _cluster[index].add_node_v1(node_a); // since node_a is already in the cluster
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
//    size_t pos = _randomized_link_indices[r];
//    _randomized_link_indices.erase(_randomized_link_indices.begin() + r); // must erase the used value
//    _last_lnk = _network_frame.getLink(pos);
//    _network_frame.activateLink(pos); // activating the link
//
//    // occupy that link
//    ++_number_of_occupied_links;
//
//    // cluster management
//    manage_cluster_v0(pos);
//
//    return true;
//}
//
///**
// * For minimizing cluster sizes using sum rule
//
// * @return index of the link in the _randomized_link_indices for which cluster size become minimum
// */
//size_t NetworkBApercolationExplosive::link_for_min_cluster_sum_rule() {
//    size_t index_randomized_link;
//
//    size_t tmp_lnk_index, index1, index2;
//    int id1, id2;
//    size_t sum{}, n_nodes;
//    Link tmp_lnk;
//    for(size_t i{}; i < _M; ++i){
//        if(i >= _randomized_link_indices.size()){
//            cout << "not enough free link : line " << __LINE__ << endl;
//            break;
//        }
//        id1 = -1;
//        id2 = -1;
//        tmp_lnk_index = _randomized_link_indices[i];
//        tmp_lnk = _network_frame.getLink(tmp_lnk_index);
//        if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
//             // since we are minimizing cluster sizes
//            index_randomized_link = i;
//            cout << "got link " << _network_frame.getLink(_randomized_link_indices[i]) << " id = " << id1 << " and " << id2 << endl;
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
//        cout << "checking link " << _network_frame.getLink(_randomized_link_indices[i])
//             << " id = " << id1 << " and " << id2 << " sum= " << sum << endl;
//    }
//    if(index_randomized_link >= _randomized_link_indices.size()){
//        cout << "out of bound : line " << __LINE__ << endl;
//    }
//    cout << "selected link " << _network_frame.getLink(_randomized_link_indices[index_randomized_link])
//            << " id = " << id1 << " and " << id2 << " sum= " << sum << endl;
//    return index_randomized_link;
//}
//
//
///**
// * For minimizing cluster sizes using sum rule
//
// * @return index of the link in the _randomized_link_indices for which cluster size become minimum
// */
//size_t NetworkBApercolationExplosive::link_for_min_cluster_product_rule() {
//    size_t index_randomized_link;
//
//    size_t tmp_lnk_index, index1, index2;
//    int id1, id2;
//    size_t prod{1}, n_nodes;
//    Link tmp_lnk;
//    for(size_t i{}; i < _M; ++i){
//        if(i >= _randomized_link_indices.size()){
//            cout << "not enough free link : line " << __LINE__ << endl;
//            break;
//        }
//        id1 = -1;
//        id2 = -1;
//        tmp_lnk_index = _randomized_link_indices[i];
//        tmp_lnk = _network_frame.getLink(tmp_lnk_index);
//        if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
//            // since we are minimizing cluster sizes
//            index_randomized_link = i;
//            cout << "got link " << _network_frame.getLink(_randomized_link_indices[i]) << " id = " << id1 << " and " << id2 << endl;
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
//        cout << "checking link " << _network_frame.getLink(_randomized_link_indices[i])
//             << " id = " << id1 << " and " << id2 << " sum= " << prod << endl;
//    }
//    if(index_randomized_link >= _randomized_link_indices.size()){
//        cout << "out of bound : line " << __LINE__ << endl;
//    }
//    cout << "selected link " << _network_frame.getLink(_randomized_link_indices[index_randomized_link])
//         << " id = " << id1 << " and " << id2 << " sum= " << prod << endl;
//    return index_randomized_link;
//}
//
//
//


/******************************
 *
 * class Network_v2
 ***************************/
void Network_v7::setRandomState(size_t seed, bool g) {
    _random_state = seed;
    if(g){
        std::random_device rd;
        _random_state = rd();
    }
    _gen.seed(_random_state);
    cout << "random seed Network_v7 " << _random_state << endl;
}

void Network_v7::view() {
    cout << "Network_v7::view __LINE__" << __LINE__ << endl;
    cout << "Link[i] => nodeA , nodeB" << endl;
    for(size_t i{}; i < _network_map_A.size(); ++i){
        cout << "Link[" << i << "] => " << _network_map_A[i] << " , " << _network_map_B[i] << endl;
    }
    cout << "Netowrk size " << _nodes.size() << endl;
    cout << "Link count " << _network_map_A.size() << endl;
    cout << "Max link " << _nodes.size() *( _nodes.size()-1)/2 << endl;

}

void Network_v7::viewAdjacencyList() {
    cout << "Network_v7::viewAdjacencyList " << endl;
    unsigned long sz;
    for(size_t i{}; i < _adjacency_list.size(); ++i){
        sz = _adjacency_list[i].size();
        cout << "node[" << i << "]=( "<< sz <<" ){";

        for(size_t j{}; j < sz; ++j){
            cout << _adjacency_list[i][j] << ",";
        }
        cout << "}" << endl;
    }
}

std::vector<uint> Network_v7::degreeCounts() {
//    cout << "To be implemented  : line " << __LINE__ << endl;
    std::vector<uint> degree_distro;
    size_t k;
    for(size_t i{}; i < degree_count.size(); ++i){
        k = degree_count[i];
        if(k >= degree_distro.size()){
            degree_distro.resize(k+1); // since k is going to be used as index we need to add extra size
        }
        degree_distro[k] += 1;
    }
    double a = 0;
    for(size_t i{}; i < degree_count.size(); ++i){
        if(degree_distro[i] == 0) continue;
        a += degree_distro[i] * i;
    }
    cout << "sum = "  << a << " sum/N = " << a/N_size << endl;
    return degree_distro;
}

void Network_v7::reset() {
    _nodes.clear();
    _adjacency_list.clear();
    _network_map_A.clear();
    _network_map_B.clear();
    degree_count.clear();
}

/**
 * Local clustering coefficient
 * Clustering coefficient for k-th node is defined as
 *      cc(k) = (number of available connection between neighbors of k) / (total number of connection possible among neighbors of k)
 *
 * Maximum connection possible between n nodes are nC2 = n!/(2! * (n-1)!) = n(n-1)/2
 * @param i
 * @return
 */
double Network_v7::clusteringCoefficient(int k) {
    if(k >= _adjacency_list.size()){
        cerr << "outside adjacency list" << endl;
    }
    auto neighbors = _adjacency_list[k];
    size_t N = neighbors.size();
    size_t connection{};
    std::vector<int>::iterator it;
    for(size_t i{}; i < neighbors.size(); ++i){

        auto neighbors_i = _adjacency_list[neighbors[i]];
        for(size_t j{i+1}; j < neighbors.size(); ++j ){
            auto b =  neighbors[i];
            it = std::find (neighbors_i.begin(), neighbors_i.end(), b);
            if(it != neighbors_i.end()){
                // found it
                cout << *it << endl;
                ++connection;
            }
        }
    }
    double possible = N*(N-1.0) / 2.0;
    return connection/possible;
}

/**
 * Global clustering coefficient
 * Average of local  clustering coefficient
 * @return
 */
double Network_v7::clusteringCoefficientAvg() {
    double sm{};
    for(auto k: _nodes){
        sm += clusteringCoefficient(k);
    }
    return sm / _nodes.size();
}

