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
NetworkBApercolation_v3::NetworkBApercolation_v3(size_t m0, size_t m, size_t size)
{
    initiate(m0, m, size);
    size_t seed = 0;
    cerr << "automatic seeding is turned off : line " << __LINE__ << endl;
//    auto seed = _random_device();
    _random_generator.seed(seed);
    cout << "random seed NetworkBApercolation_v3 " << seed << endl;
    //    cout << _link_indices.size() << " : " << _link_indices << endl;
    randomize_v1();
    log_1_by_size = std::log(1.0 / _network_size);
}

void NetworkBApercolation_v3::initiate(size_t m0, size_t m, size_t size) {
    _m0 = m0;
    _m = m;
    _network_size = size+_m0;

    initialize_network();
    initialize_cluster();

    _link_count = _network_frame.getNumberOfLinks();
    _link_indices.resize(_link_count);
    _randomized_indices.resize(_link_count);
    for(uint i = 0; i < _link_count; ++i){
        _link_indices[i] = i;
    }
    _randomized_indices = _link_indices;

}
/**
 * Must be called after the network is fully functional
 */
void NetworkBApercolation_v3::initialize_cluster() {
    size_t N = _network_frame.number_of_nodes();
    for(uint i{}; i < N ; ++i){
        _cluster.push_back({});
        _cluster[i].add_node(i);
        _cluster[i].set_group_id(group_id_count);
        _network_frame.set_node_group_id(i, group_id_count);
        ++group_id_count;
    }
}

void NetworkBApercolation_v3::initialize_network() {
    cout << "Initializing Network ... " << std::flush;
    _network_frame = NetworkBA(_m0, _m);
    uint i= _m0;
    while (i < _network_size){ // sp that number of nodes is the network size
        _network_frame.addNode();
        ++i;
    }
    cout << "done." << endl;
    _network_frame.clear_preferentially(); // to clear some memory
}


/**
 * Uses Better Random number generator from standard library
 */
void NetworkBApercolation_v3::randomize_v1() {

//    cout << "before " << _randomized_indices << endl;
    _randomized_indices = _link_indices; // must assign. in case _randomized_indices is not suitable
    std::shuffle(_randomized_indices.begin(), _randomized_indices.end(), _random_generator);
//    cout << "after " << _randomized_indices << endl;

}

bool NetworkBApercolation_v3::occupyLink(){
//    return placeLink(); // added 2018.08.21
    size_t pos  = selectLink();  // 2018.08.21
    return placeSelectedLink(pos); // 2018.08.21
}

bool NetworkBApercolation_v3::placeLink() {
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

void NetworkBApercolation_v3::manageCluster(size_t link_pos) {
//    manage_cluster_v0(link_pos);
//    manage_cluster_v1(link_pos);
    manage_cluster_v2(link_pos);
}

/**
 *
 * @param pos : radnomized position of the link index
 */
void NetworkBApercolation_v3::manage_cluster_v0(size_t pos) {
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
 * @param link_pos : radnomized position of the link index
 */
void NetworkBApercolation_v3::manage_cluster_v2(size_t link_pos) { // TODO : to be edited
    Link lnk = _network_frame.getLink(link_pos);
    // getting all details
//    int id = lnk.get_group_id();
    uint node_a = lnk.get_a();
    int id_a = _network_frame.get_node_group_id(node_a);
    uint node_b = lnk.get_b();
    int id_b = _network_frame.get_node_group_id(node_b);
//    cout << "a " << id_a << " and b " << id_b << endl;
    if(id_a == id_b){
        ++manage_cluster_v2_c;
        // no new nodes
        // not -1 but same then just insert the link since both nodes already belong to same cluster
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _network_frame.set_link_group_id(link_pos, id_b);
    }
    else{
        ++manage_cluster_v2_d;
        /// merge cluster
        // no need to add nodes to the cluster. they are already there
        size_t index_a = size_t(id_a);
        size_t index_b = size_t(id_b);
        // base is the larger cluster (number of nodes in it)
        size_t size_a = _cluster[index_a].numberOfNodes();
        size_t size_b = _cluster[index_b].numberOfNodes();
        if(size_a > size_b){
            merge_cluster(link_pos, lnk, id_a, id_b, node_b);

        }else{
            // index_b will survive the process
            merge_cluster(link_pos, lnk, id_b, id_a, node_a);

//            _cluster[index_b].add_link(lnk);
//            _network_frame.set_node_group_id(node_a, id_b);
//            _network_frame.set_link_group_id(link_pos, id_b);
//            _cluster[index_b].insert(_cluster[index_a]);
//            relabel_nodes(_cluster[index_a], id_b);
//            _cluster[index_a].clear();

        }
    }
}

/**
 *
 * @param link_pos : position of the selected link
 * @param lnk : currect link which has two directly connected node with id (id_a and id_b)
 * @param id_a : one of the directly connected node id and surviving group id or base cluster id
 * @param id_b : one of the directly connected node id
 * @param node_b : one of the node the currect link is connected to and which was not part of base cluster
 */
void NetworkBApercolation_v3::merge_cluster(size_t link_pos, const Link &lnk, int id_a, int id_b,
                                            uint node_b) {
    size_t index_a = size_t(id_a);
    size_t index_b = size_t(id_b);
    // index_base will survive the process
    _cluster[index_a].add_link(lnk);
    _network_frame.set_node_group_id(node_b, id_a);
    _network_frame.set_link_group_id(link_pos, id_a);
    _cluster[index_a].insert(_cluster[index_b]);
    // relabeling
    relabel_nodes(_cluster[index_b], id_a);
    _cluster[index_b].clear();
}

/**
 * largest cluster is managed after this particular method ends.
 * @param pos : radnomized position of the link index
 */
void NetworkBApercolation_v3::manage_cluster_v1(size_t pos) { // TODO : to be edited
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
        ++manage_cluster_v2_a;
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
        ++manage_cluster_v2_b;
        // add to id_a
        size_t index = size_t(id_a);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_b); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_b, id_a);
        _network_frame.set_link_group_id(pos, id_a);

    }
    else if(id_a == -1 && id_b != -1){
        ++manage_cluster_v2_b;
        // add to id_b
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _cluster[index].add_node(node_a); // since node_a is already in the cluster
        _number_of_connected_nodes += 1; // 1 node is connected here
        _network_frame.set_node_group_id(node_a, id_b);
        _network_frame.set_link_group_id(pos, id_b);

    } else if(id_a == id_b){
        ++manage_cluster_v2_c;
        // no new nodes
        // not -1 but same then just insert the link since both nodes already belong to same cluster
        size_t index = size_t(id_b);
        _cluster[index].add_link(lnk);
        _network_frame.set_link_group_id(pos, id_b);
    }
    else{
        ++manage_cluster_v2_d;
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

void NetworkBApercolation_v3::viewCluster() {
    cout << "viewCluster : line " << __LINE__ << endl;
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

void NetworkBApercolation_v3::viewClusterExtended() {
    cout << "viewClusterExtended : line " << __LINE__ << endl;
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

void NetworkBApercolation_v3::relabel_nodes(Cluster& clstr, int id) {
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
double NetworkBApercolation_v3::entropy_v1() {
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

double NetworkBApercolation_v3::entropy_v2() {
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
void NetworkBApercolation_v3::subtract_entropy(Link &lnk) {
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

void NetworkBApercolation_v3::add_entropy(Link &lnk) {
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
size_t NetworkBApercolation_v3::selectLink() {
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
 * @param link_pos
 * @return
 */
bool NetworkBApercolation_v3::placeSelectedLink(size_t link_pos) {
    if(link_pos >= _link_count){
//        cout << "link_pos > link_count : line " << __LINE__ << endl;
        return false;
    }
    auto start = std::chrono::system_clock::now(); // time measurement
    _last_lnk = _network_frame.getLink(link_pos);
    _network_frame.activateLink(link_pos); // activating the link

    // occupy that link
    ++_number_of_occupied_links;

    // cluster management
    subtract_entropy(_last_lnk);
    manageCluster(link_pos);
    add_entropy(_last_lnk);
    track_cluster_v2();

    auto end = std::chrono::system_clock::now(); // time measurement
    std::chrono::duration<double> elapsed_seconds = end-start; // time measurement
    _time_placeSelectedLink += elapsed_seconds.count();
    return true;
}

void NetworkBApercolation_v3::reset(int i) {
    cout << "reset NetworkBApercolation_v3: line " <<__LINE__ << endl;
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
    _cluster.clear();
    _network_frame.clear_node_id();


    if(i == 1){
        // initialize the network again
        initialize_network();
    }
    initialize_cluster();
//    _randomized_indices = _link_indices; // assigned inside randomize_v1() function
    randomize_v1();
}


/**
 * Must be called after entropy is calculated each time. If not then this function will not work.
 */
void NetworkBApercolation_v3::jump() {
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
void NetworkBApercolation_v3::track_cluster() {
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
void NetworkBApercolation_v3::track_cluster_v2() {
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

void NetworkBApercolation_v3::time_summary() {
    cout << "_time_placeSelectedLink " << _time_placeSelectedLink << " sec" << endl;

    cout << "manage_cluster_v2_a " << manage_cluster_v2_a << " times" << endl;
    cout << "manage_cluster_v2_b " << manage_cluster_v2_b << " times" << endl;
    cout << "manage_cluster_v2_c " << manage_cluster_v2_c << " times" << endl;
    cout << "manage_cluster_v2_d " << manage_cluster_v2_d << " times" << endl;

}

/**
 * Counting total number of cluster with size greater than 1
 * @return
 */
size_t NetworkBApercolation_v3::clusterCount() const {
    size_t count{};
    for(size_t i{}; i < _cluster.size();++i){
        if(!_cluster[i].empty()) count++;
    }
    return count;
}

/**
 * if _randomized_indices changes as the program occupies new links, it is
 * because of necessity
 */
void NetworkBApercolation_v3::view_randomized_indices() const {
    cout << "{";
    for(auto a: _randomized_indices){
        cout << a << ",";
    }
    cout << "}" << endl;
}



