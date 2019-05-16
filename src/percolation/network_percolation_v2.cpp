//
// Created by Shahnooor on 8/22/2018.
//


#include <cmath>
#include <random>
#include "../nets/network.h"
#include "network_percolation.h"

using namespace std;


/*************************
 * Percolation on BA network
 */
NetworkBApercolation_v2::NetworkBApercolation_v2(size_t m0, size_t m, size_t size)
{
    initiate(m0, m, size);
    size_t seed = 0;
    cerr << "automatic seeding is turned off : line " << __LINE__ << endl;
//    auto seed = _random_device();
    _random_generator.seed(seed);
    //    cout << _link_indices.size() << " : " << _link_indices << endl;
    randomize_v1();
    log_1_by_size = std::log(1.0 / _network_size);
}

void NetworkBApercolation_v2::initiate(size_t m0, size_t m, size_t size) {
    _m0 = m0;
    _m = m;
    _network_size = size+_m0;

    initialize_network();

    _link_count = _network_frame.getNumberOfLinks();
    _link_indices.resize(_link_count);
    _randomized_indices.resize(_link_count);
    for(uint i = 0; i < _link_count; ++i){
        _link_indices[i] = i;
    }
    _randomized_indices = _link_indices;

}

void NetworkBApercolation_v2::initialize_network() {
    cout << "Initializing Network ... " << std::flush;
    _network_frame = NetworkBA(_m0, _m);
    uint i= _m0;
    while (i < _network_size){ // sp that number of nodes is the network size
        _network_frame.addNode();
        ++i;
    }
    cout << "done." << endl;
}


/**
 * Uses Better Random number generator from standard library
 */
void NetworkBApercolation_v2::randomize_v1() {

//    cout << "before " << _randomized_indices << endl;
    std::shuffle(_randomized_indices.begin(), _randomized_indices.end(), _random_generator);
//    cout << "after " << _randomized_indices << endl;

}

bool NetworkBApercolation_v2::occupyLink(){
//    return placeLink(); // added 2018.08.21
    size_t pos  = selectLink();  // 2018.08.21
    return placeSelectedLink(pos); // 2018.08.21
}

bool NetworkBApercolation_v2::placeLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
//    cout << index_var << " ==? " << _randomized_indices.size();
    // select a link randomly
    size_t last_link_pos_in_randomized = _randomized_indices[index_var];
    ++index_var;
    _last_lnk = _network_frame.getLink(last_link_pos_in_randomized);
    _network_frame.activateLink(last_link_pos_in_randomized); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    subtract_entropy(_last_lnk);
    manage_cluster_v0(last_link_pos_in_randomized);
    add_entropy(_last_lnk);

    return true;
}

void NetworkBApercolation_v2::manageCluster(size_t position) {
//    manage_cluster_v0(position);
    manage_cluster_v1(position);
}

/**
 *
 * @param pos : radnomized position of the link index
 */
void NetworkBApercolation_v2::manage_cluster_v0(size_t pos) {
    Link lnk = _network_frame.getLink(pos);
    // getting all details
//    int id = lnk.get_group_id();
    uint node_a = lnk.get_a();
    int id_a = _network_frame.get_node_group_id(node_a);
    uint node_b = lnk.get_b();
    int id_b = _network_frame.get_node_group_id(node_b);
//    cout << "a " << id_a << " and b " << id_b << endl;
    // if both nodes have id -1. means they are not part of any cluster
    if(id_a == -1 && id_b == -1){
//        cout << "################ new cluster ################ : line " <<__LINE__ << endl;
        size_t  sz = _cluster.size();
        _cluster.push_back({});
        _cluster[sz].add_link(lnk);
        _cluster[sz].add_node(node_a);
        _cluster[sz].add_node(node_b);
        _cluster[sz].set_group_id(group_id_count);
        _number_of_connected_nodes += 2; // 2 node is connected here
        _network_frame.set_node_group_id(node_a, group_id_count);
        _network_frame.set_node_group_id(node_b, group_id_count);
        _network_frame.set_link_group_id(pos, group_id_count); // must set value in the global array. not local
        ++group_id_count;
        if(_number_of_nodes_in_the_largest_cluster < _cluster[sz].numberOfNodes()){
            _number_of_nodes_in_the_largest_cluster = _cluster[sz].numberOfNodes();
        }
    }else if(id_a != -1 && id_b == -1){
        // add to id_a
        size_t index = size_t(id_a);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_b); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_b, id_a);
        _network_frame.set_link_group_id(pos, id_a);
        if(_number_of_nodes_in_the_largest_cluster < _cluster[index].numberOfNodes()){
            _number_of_nodes_in_the_largest_cluster = _cluster[index].numberOfNodes();
        }
    }
    else if(id_a == -1 && id_b != -1){
        // add to id_b
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_a); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_a, id_b);
        _network_frame.set_link_group_id(pos, id_b);
        if(_number_of_nodes_in_the_largest_cluster < _cluster[index].numberOfNodes()){
            _number_of_nodes_in_the_largest_cluster = _cluster[index].numberOfNodes();
        }
    } else if(id_a == id_b){
        // no new nodes
        // not -1 but same then just insert the link since both nodes already belong to same cluster
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _network_frame.set_link_group_id(pos, id_b);
    }
    else{
        /// merge cluster
        // no need to add nodes to the cluster. they are already there
        size_t index_a = size_t(id_a);
        size_t index_b = size_t(id_b);
        // base is the larger cluster (number of nodes in it)
        size_t size_a = _cluster[index_a].numberOfNodes();
        size_t size_b = _cluster[index_b].numberOfNodes();
        if(size_a > size_b){
            // index_a will survive the process
            _cluster[index_a].add_link(lnk);
            _network_frame.set_node_group_id(node_b, id_a);
            _network_frame.set_link_group_id(pos, id_a);
            _cluster[index_a].insert(_cluster[index_b]);
            // relabeling
            relabel_nodes(_cluster[index_b], id_a);
            _cluster[index_b].clear();

        }else{
            // index_b will survive the process
            _cluster[index_b].add_link(lnk);
            _network_frame.set_node_group_id(node_a, id_b);
            _network_frame.set_link_group_id(pos, id_b);
            _cluster[index_b].insert(_cluster[index_a]);
            relabel_nodes(_cluster[index_a], id_b);
            _cluster[index_a].clear();

        }
        if(_number_of_nodes_in_the_largest_cluster < (size_a + size_b)){
            _number_of_nodes_in_the_largest_cluster = size_a + size_b;
        }

    }

}

/**
 * largest cluster is managed after this particular method ends.
 * @param pos : radnomized position of the link index
 */
void NetworkBApercolation_v2::manage_cluster_v1(size_t pos) { // TODO : to be edited
    Link lnk = _network_frame.getLink(pos);
    // getting all details
//    int id = lnk.get_group_id();
    uint node_a = lnk.get_a();
    int id_a = _network_frame.get_node_group_id(node_a);
    uint node_b = lnk.get_b();
    int id_b = _network_frame.get_node_group_id(node_b);
//    cout << "a " << id_a << " and b " << id_b << endl;
    // if both nodes have id -1. means they are not part of any cluster
    if(id_a == -1 && id_b == -1){
//        cout << "################ new cluster ################ : line " <<__LINE__ << endl;
        size_t  sz = _cluster.size();
        _cluster.push_back({});
        _cluster[sz].add_link(lnk);
        _cluster[sz].add_node(node_a);
        _cluster[sz].add_node(node_b);
        _cluster[sz].set_group_id(group_id_count);
        _number_of_connected_nodes += 2; // 2 node is connected here
        _network_frame.set_node_group_id(node_a, group_id_count);
        _network_frame.set_node_group_id(node_b, group_id_count);
        _network_frame.set_link_group_id(pos, group_id_count); // must set value in the global array. not local
        ++group_id_count;

    }else if(id_a != -1 && id_b == -1){
        // add to id_a
        size_t index = size_t(id_a);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_b); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_b, id_a);
        _network_frame.set_link_group_id(pos, id_a);

    }
    else if(id_a == -1 && id_b != -1){
        // add to id_b
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_a); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_a, id_b);
        _network_frame.set_link_group_id(pos, id_b);

    } else if(id_a == id_b){
        // no new nodes
        // not -1 but same then just insert the link since both nodes already belong to same cluster
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _network_frame.set_link_group_id(pos, id_b);
    }
    else{
        /// merge cluster
        // no need to add nodes to the cluster. they are already there
        size_t index_a = size_t(id_a);
        size_t index_b = size_t(id_b);
        // base is the larger cluster (number of nodes in it)
        size_t size_a = _cluster[index_a].numberOfNodes();
        size_t size_b = _cluster[index_b].numberOfNodes();
        if(size_a > size_b){
            // index_a will survive the process
            _cluster[index_a].add_link(lnk);
            _network_frame.set_node_group_id(node_b, id_a);
            _network_frame.set_link_group_id(pos, id_a);
            _cluster[index_a].insert(_cluster[index_b]);
            // relabeling
            relabel_nodes(_cluster[index_b], id_a);
            _cluster[index_b].clear();

        }else{
            // index_b will survive the process
            _cluster[index_b].add_link(lnk);
            _network_frame.set_node_group_id(node_a, id_b);
            _network_frame.set_link_group_id(pos, id_b);
            _cluster[index_b].insert(_cluster[index_a]);
            relabel_nodes(_cluster[index_a], id_b);
            _cluster[index_a].clear();

        }
    }
}

void NetworkBApercolation_v2::viewCluster() {
    for(size_t i{}; i < _cluster.size(); ++i){
        if(_cluster[i].empty()){
            continue;
        }
        cout << "cluster[" << i << "] : id " << _cluster[i].get_group_id() << "{" << endl;
        cout << "  Nodes (" << _cluster[i].numberOfNodes() << "): ";
        _cluster[i].viewNodes();
        cout << endl;
        cout << "  Links (" << _cluster[i].numberOfLinks() << "): ";
        _cluster[i].viewLinks();
        cout << endl;
        cout << "}" << endl;
    }

}

void NetworkBApercolation_v2::viewClusterExtended() {
    for(size_t i{}; i < _cluster.size(); ++i){
        if(_cluster[i].empty()){
            continue;
        }
        cout << "cluster[" << i << "] : id " << _cluster[i].get_group_id() << "{" << endl;
        cout << "  Nodes (" << _cluster[i].numberOfNodes() << "): ";
        auto nds = _cluster[i].getNodes();
        cout << "(index, id)->";
        for(auto n: nds){
            cout << "(" << n << "," << _network_frame.get_node_group_id(n) << "),";
        }
        cout << endl;
        cout << "  Links (" << _cluster[i].numberOfLinks() << "): ";
//        _cluster[i].viewLinks();
        _cluster[i].viewLinksExtended();
        cout << endl;
        cout << "}" << endl;
    }

}

void NetworkBApercolation_v2::relabel_nodes(Cluster& clstr, int id) {
    auto nds = clstr.getNodes();
    for(size_t i{}; i < nds.size(); ++i){
        _network_frame.set_node_group_id(nds[i],id);
    }
}

/**
 * Calculates shanon entropy
 * mu = (number of nodes in a cluster) / (total number of nodes)
 * H = - sum mu_i log(mu_i)
 * @return
 */
double NetworkBApercolation_v2::entropy_v1() {
    size_t count{}, n;
    double mu{}, H{};
    for(size_t i{}; i < _cluster.size(); ++i){
        n = _cluster[i].numberOfNodes();
        count += n;
        mu = n / double(_network_size);
        H += mu * std::log(mu); // shanon entropy
    }
    if(_network_size != _network_frame.number_of_nodes()){
        cout << "_network_size != _network_frame.number_of_nodes() ; line " << __LINE__ << endl;
    }
    size_t remaining = _network_size - count;
    H += remaining * logOneBySize() / double(_network_size); // for the clusters of size one
    _current_entropy = -H;
    return _current_entropy;
}

double NetworkBApercolation_v2::entropy_v2() {
    size_t isolated_node = _network_size - _number_of_connected_nodes;
//    double lg = std::log(1.0 / _network_size);
//    cout << logOneBySize() << endl;
    double H = - (logOneBySize() * isolated_node) / _network_size;
    _current_entropy = H + _entropy;
    return _current_entropy;
}

/**
 * Subtract entropy for two nodes of the lnk link
 * @param lnk
 */
void NetworkBApercolation_v2::subtract_entropy(Link &lnk) {
    size_t a = lnk.get_a();
    size_t b = lnk.get_b();
    size_t n{};
    double H{};
    double mu {};
    int id_a = _network_frame.getNode(a).get_group_id();
    int id_b = _network_frame.getNode(b).get_group_id();
//    cout << "node " << a << " id " << id_a << endl;
//    cout << "node " << b << " id " << id_b << endl;

    if(id_a == id_b && id_a != -1){
        // belongs to same cluster
        n = _cluster[id_a].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
        _entropy -= H;
        return; // no need to go further
    }
    if(id_a != -1){
        n = _cluster[id_a].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
    }
    if(id_b != -1){
        n = _cluster[id_b].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
    }
    _entropy -= H;
}

void NetworkBApercolation_v2::add_entropy(Link &lnk) {
    size_t a = lnk.get_a();
    size_t b = lnk.get_b();
    size_t n{};
    double H{}, mu{};
    if(_network_frame.getNode(a).get_group_id() != -1) {
        n = _cluster[_network_frame.getNode(a).get_group_id()].numberOfNodes();
        mu = n / double(_network_size);
        H += - mu * std::log(mu);
    }
    _entropy += H;
}

/**
 * Selects a link randomly
 * @return
 */
size_t NetworkBApercolation_v2::selectLink() {
    if(index_var >= _link_count){
        return _link_count + 1; // so that program crashes or stops
    }
    // select a link randomly
    size_t pos = _randomized_indices[index_var];
    ++index_var;
    return pos;
}

/**
 * place the link whose index in network is passed as argument
 * @param pos
 * @return
 */
bool NetworkBApercolation_v2::placeSelectedLink(size_t pos) {
    if(pos >= _link_count){
//        cout << "pos > link_count : line " << __LINE__ << endl;
        return false;
    }
    _last_lnk = _network_frame.getLink(pos);
    _network_frame.activateLink(pos); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    subtract_entropy(_last_lnk);
    manageCluster(pos);
    add_entropy(_last_lnk);
    track_cluster_v2();

    return true;
}

void NetworkBApercolation_v2::reset(int i) {
    cout << "reset NetworkBApercolation_v2: line " <<__LINE__ << endl;
    index_var = 0;
    _number_of_occupied_links = 0;
    _number_of_connected_nodes = 0;
    _number_of_nodes_in_the_largest_cluster = 0;
    _entropy = 0;
    _largest_jump_entropy =0;
    _previous_entropy = 0;
    _current_entropy = 0;
    group_id_count = 0;
    _last_lnk = {};
    if(i == 1){
        // initialize the network again
        initialize_network();
    }
    _randomized_indices = _link_indices;
    randomize_v1();
    _cluster.clear();
    _network_frame.clear_node_id();

}


/**
 * Must be called after entropy is calculated each time. If not then this function will not work.
 */
void NetworkBApercolation_v2::jump() {
    double delta_H{};
    if(_number_of_occupied_links == 1){
        _previous_entropy = _current_entropy;
    }else{
        delta_H = _current_entropy - _previous_entropy;
        _previous_entropy = _current_entropy; // be ready for next step
    }
    if(abs(delta_H) > abs(_largest_jump_entropy)){
        _largest_jump_entropy = delta_H;
        _entropy_jump_pc = occupationProbability();
    }

}
/**
 * For tracking largest cluster and
 * if the largest cluster is the unchanged from previous step, i.e.,
 * the new largest cluster is the old largest cluster
 */
void NetworkBApercolation_v2::track_cluster() {
    size_t a = _last_lnk.get_a();
    int a_index = _network_frame.get_node_group_id(a);
    if(a_index < 0){
        cerr << "why? : line " << __LINE__ << endl;
        a_index = _network_frame.get_node_group_id(_last_lnk.get_b());
    }
    size_t sz = _cluster[a_index].numberOfNodes();
    if(sz > _number_of_nodes_in_the_largest_cluster){
        _number_of_nodes_in_the_largest_cluster = sz;
    }

    _self_cluster_jump = (_last_largest_cluster_id == a_index); // true only in case of self jump
    _last_largest_cluster_id = a_index;
}

/**
 * For tracking largest cluster and
 * if the largest cluster is the unchanged from previous step, i.e.,
 * the new largest cluster is the old largest cluster.
 *
 * In case of zero cluster size jump
 */
void NetworkBApercolation_v2::track_cluster_v2() {
    size_t a = _last_lnk.get_a();
    int a_index = _network_frame.get_node_group_id(a);
    if(a_index < 0){
        cerr << "why? : line " << __LINE__ << endl;
        a_index = _network_frame.get_node_group_id(_last_lnk.get_b());
    }
    size_t sz = _cluster[a_index].numberOfNodes();
    if(sz > _number_of_nodes_in_the_largest_cluster){
        _number_of_nodes_in_the_largest_cluster = sz;
        _self_cluster_jump = (_last_largest_cluster_id == a_index); // true only in case of self jump
    }else{
        _self_cluster_jump = false;
    }
    _last_largest_cluster_id = a_index;
}


/************************************************************
 * Explosive percolation on BA network
 *************************************************************/
/**
 *
 * @param m0
 * @param m
 * @param size
 * @param M
 */
NetworkBApercolationExplosive_v2::NetworkBApercolationExplosive_v2(size_t m0, size_t m, size_t size, uint M)
        : NetworkBApercolation_v2(m0, m, size)
{
    if (M > 2) {
        if(M > MAX_M_VALUE){
            cout << "M exceeds maximum allowed value : line " << __LINE__ << endl;
            _M = MAX_M_VALUE;
        }
        _M = M;
    }
}

bool NetworkBApercolationExplosive_v2::placeLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
//    cout << index_var << " ==? " << _randomized_indices.size();
    // select M links and occupy that one which makes the cluster size minimum
    size_t pos = selectLink('s');
    _last_lnk = _network_frame.getLink(pos);
    _network_frame.activateLink(pos); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    manage_cluster_v0(pos);

    return true;
}

/**
 *
 * @param rule : specify sum rule or product rule
 *               values : 's' for sum rule and 'p' for product rule
 * @return
 */
size_t NetworkBApercolationExplosive_v2::selectLink(char rule ) {
    if(_number_of_occupied_links >= _link_count){
        return _link_count+1;
    }
    size_t r{};
    if(rule == 's'){
        r = link_for_min_cluster_sum_rule();;
    }else if(rule =='p'){
        r = link_for_min_cluster_product_rule();;
    }else{
        cerr << "bad argument : line " << __LINE__ << endl;
        return _link_count + 1; // to terminate the program
    }

    size_t pos = _randomized_indices[r];
    _randomized_indices.erase(_randomized_indices.begin() + r); // must erase the used value
    return pos;
}

/**
 * For minimizing cluster sizes using sum rule

 * @return index of the link in the _randomized_indices for which cluster size become minimum
 */
size_t NetworkBApercolationExplosive_v2::link_for_min_cluster_sum_rule() {
    size_t index_randomized_link;

    size_t tmp_lnk_index, index1, index2;
    int id1, id2;
    size_t sum{}, n_nodes;
    Link tmp_lnk;
    for(size_t i{}; i < _M; ++i){
        if(i >= _randomized_indices.size()){
//            cout << "not enough free link : line " << __LINE__ << endl;
            break;
        }
        id1 = -1;
        id2 = -1;
        tmp_lnk_index = _randomized_indices[i];
        tmp_lnk = _network_frame.getLink(tmp_lnk_index);
        if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
            // since we are minimizing cluster sizes
            index_randomized_link = i;
//            cout << "got link " << _network_frame.getLink(_randomized_indices[i]) << " id = " << id1 << " and " << id2 << endl;
            break; // since this is the minimum link case
        }
        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) != -1){
            id1 = _network_frame.get_node_group_id(tmp_lnk.get_b());
            index1 = size_t(id1);
            n_nodes = _cluster[index1].numberOfNodes() + 1; // 1 node would have been added
            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
                sum = n_nodes;
                index_randomized_link = i;
            }
        }
        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) != -1 && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
            index1 = size_t(id1);
            n_nodes = _cluster[index1].numberOfNodes() + 1; // 1 node would have been added
            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
                sum = n_nodes;
                index_randomized_link = i;
            }
        }
        else{

            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
            id2 = _network_frame.get_node_group_id(tmp_lnk.get_b());
            index1 = size_t(id1);
            index2 = size_t(id2);
            if(id1 == id2){
                // if they belong to same cluster. adding new link does not increase cluster size.
                n_nodes = _cluster[index1].numberOfNodes();
//                cout << "same cluster " << endl;
            }else {

                n_nodes = _cluster[index1].numberOfNodes() + _cluster[index2].numberOfNodes();
            }
            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
                sum = n_nodes;
                index_randomized_link = i;
            }
        }
//        cout << "checking link " << _network_frame.getLink(_randomized_indices[i])
//             << " id = " << id1 << " and " << id2 << " sum= " << sum << endl;
    }
    if(index_randomized_link >= _randomized_indices.size()){
        cerr << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "selected link " << _network_frame.getLink(_randomized_indices[index_randomized_link])
//            << " id = " << id1 << " and " << id2 << " sum= " << sum << endl;
    return index_randomized_link;
}


/**
 * For minimizing cluster sizes using sum rule

 * @return index of the link in the _randomized_indices for which cluster size become minimum
 */
size_t NetworkBApercolationExplosive_v2::link_for_min_cluster_product_rule() {
    size_t index_randomized_link;

    size_t tmp_lnk_index, index1, index2;
    int id1, id2;
    size_t prod{1}, n_nodes;
    Link tmp_lnk;
    for(size_t i{}; i < _M; ++i){
        if(i >= _randomized_indices.size()){
            cout << "not enough free link : line " << __LINE__ << endl;
            break;
        }
        id1 = -1;
        id2 = -1;
        tmp_lnk_index = _randomized_indices[i];
        tmp_lnk = _network_frame.getLink(tmp_lnk_index);
        if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1
           && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
            // since we are minimizing cluster sizes
            index_randomized_link = i;
            cout << "got link " << _network_frame.getLink(_randomized_indices[i]) << " id = " << id1 << " and " << id2 << endl;
            break; // since this is the minimum link case
        }
        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1
                && _network_frame.get_node_group_id(tmp_lnk.get_b()) != -1){
            id1 = _network_frame.get_node_group_id(tmp_lnk.get_b());
            index1 = size_t(id1);
            n_nodes = _cluster[index1].numberOfNodes(); // 1 node would have been multiplied
            if(prod == 1 || prod > n_nodes) { // since we are minimizing cluster sizes
                prod = n_nodes;
                index_randomized_link = i;
            }
        }
        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) != -1
                && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
            index1 = size_t(id1);
            n_nodes = _cluster[index1].numberOfNodes(); // 1 node would have been multiplied
            if(prod == 1 || prod > n_nodes) { // since we are minimizing cluster sizes
                prod = n_nodes;
                index_randomized_link = i;
            }
        }
        else{

            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
            id2 = _network_frame.get_node_group_id(tmp_lnk.get_b());
            index1 = size_t(id1);
            index2 = size_t(id2);
            if(id1 == id2){
                // if they belong to same cluster. adding new link does not increase cluster size.
                n_nodes = _cluster[index1].numberOfNodes();
                cout << "same cluster " << endl;
            }else {
                n_nodes = _cluster[index1].numberOfNodes() * _cluster[index2].numberOfNodes(); // product rule
            }
            if(prod == 1 || prod > n_nodes) { // since we are minimizing cluster sizes
                prod = n_nodes;
                index_randomized_link = i;
            }
        }
        cout << "checking link " << _network_frame.getLink(_randomized_indices[i])
             << " id = " << id1 << " and " << id2 << " sum= " << prod << endl;
    }
    if(index_randomized_link >= _randomized_indices.size()){
        cout << "out of bound : line " << __LINE__ << endl;
    }
    cout << "selected link " << _network_frame.getLink(_randomized_indices[index_randomized_link])
         << " id = " << id1 << " and " << id2 << " sum= " << prod << endl;
    return index_randomized_link;
}

bool NetworkBApercolationExplosive_v2::occupyLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
    size_t pos = selectLink('s');
    return NetworkBApercolation_v2::placeSelectedLink(pos);
}

/********************************************************
 * Sum rule
 *****************************************************/
bool NetworkBApercolationExplosiveSum_v2::placeLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
//    cout << index_var << " ==? " << _randomized_indices.size();
    // select M links and occupy that one which makes the cluster size minimum
    size_t r = link_for_min_cluster_sum_rule();
    size_t pos = _randomized_indices[r];
    _randomized_indices.erase(_randomized_indices.begin() + r); // must erase the used value
    _last_lnk = _network_frame.getLink(pos);
    _network_frame.activateLink(pos); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    manage_cluster_v0(pos);

    return true;
}

bool NetworkBApercolationExplosiveSum_v2::occupyLink() {
    size_t r = NetworkBApercolationExplosive_v2::selectLink('s');
    return NetworkBApercolationExplosive_v2::placeSelectedLink(r);
}

/*****************************************
 * Product rule
 ******************************************/
bool NetworkBApercolationExplosiveProduct_v2::occupyLink() {
    size_t r = NetworkBApercolationExplosive_v2::selectLink('p');
    return NetworkBApercolationExplosive_v2::placeSelectedLink(r);
}


bool NetworkBApercolationExplosiveProduct_v2::placeLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
//    cout << index_var << " ==? " << _randomized_indices.size();
    // select M links and occupy that one which makes the cluster size minimum
    size_t r = link_for_min_cluster_product_rule();
    size_t pos = _randomized_indices[r];
    _randomized_indices.erase(_randomized_indices.begin() + r); // must erase the used value
    _last_lnk = _network_frame.getLink(pos);
    _network_frame.activateLink(pos); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    manage_cluster_v0(pos);

    return true;
}

