//
// Created by Shahnooor on 8/22/2018.
//


#include <cmath>
#include <random>
#include "../nets/network.h"
#include "network_percolation_v4.h"


using namespace std;


/*************************
 * Percolation on BA network
 */
NetworkBApercolation_v4::NetworkBApercolation_v4(size_t m0, size_t m, size_t size)
{
    initiate(m0, m, size);
//    size_t seed = 0;
//    cerr << "automatic seeding is turned off : line " << __LINE__ << endl;
    auto seed = _random_device();
    _random_generator.seed(seed);
    cout << "random seed NetworkBApercolation_v4 " << seed << endl;
    //    cout << _randomized_link_indices.size() << " : " << _randomized_link_indices << endl;
    randomize_v1();
    log_1_by_size = std::log(1.0 / _network_size);
}

void NetworkBApercolation_v4::initiate(size_t m0, size_t m, size_t size) {
    _m0 = m0;
    _m = m;
    _network_size = size+_m0;

    initialize_network();
    initialize_cluster();

    _link_count = _network_frame.getNumberOfLinks();
    _randomized_link_indices.resize(_link_count);
    _randomized_link_indices.resize(_link_count);
    for(uint i = 0; i < _link_count; ++i){
        _randomized_link_indices[i] = i;
    }
    _randomized_link_indices = _randomized_link_indices;

}
/**
 * Must be called after the network is fully functional
 */
void NetworkBApercolation_v4::initialize_cluster() {
    size_t N = _network_frame.number_of_nodes();
    for(uint i{}; i < N ; ++i){
        _cluster.push_back({});
        _cluster[i].add_node(i);
        _cluster[i].set_group_id(group_id_count);
        _network_frame.set_node_group_id(i, group_id_count);
        ++group_id_count;
    }
}

void NetworkBApercolation_v4::initialize_network() {
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
void NetworkBApercolation_v4::randomize_v1() {

//    cout << "before " << _randomized_link_indices << endl;

    std::shuffle(_randomized_link_indices.begin(), _randomized_link_indices.end(), _random_generator);
//    cout << "after " << _randomized_link_indices << endl;
//    cout << "sizeof(_randomized_link_indices) " << sizeof(std::vector<uint>) + sizeof(uint) * _randomized_link_indices.size() << " bytes" << endl;

}

bool NetworkBApercolation_v4::occupyLink(){
    size_t pos  = selectLink();  // 2018.08.21
    return placeSelectedLink(pos); // 2018.08.21
}



void NetworkBApercolation_v4::manageCluster(size_t link_pos) {
    manage_cluster_v2(link_pos);
}


/**
 * largest cluster is managed after this particular method ends.
 * @param link_pos : radnomized position of the link index
 */
void NetworkBApercolation_v4::manage_cluster_v2(size_t link_pos) {
    Link lnk = _network_frame.getLink(link_pos);
    // getting all details
//    int id = lnk.get_group_id();
    uint node_a = lnk.get_a();
    int id_a = _network_frame.get_node_group_id(node_a);
    uint node_b = lnk.get_b();
    int id_b = _network_frame.get_node_group_id(node_b);
//    cout << "a " << id_a << " and b " << id_b << endl;

    size_t index_a = size_t(id_a);
    size_t index_b = size_t(id_b);
//    cout << "clusters " << id_a << " and " << id_b << endl;

    if(id_a == id_b){
        ++manage_cluster_v2_c;
        // no new nodes
        // not -1 but same then just insert the link since both nodes already belong to same cluster
        _cluster[index_b].add_link(lnk);
        _network_frame.set_link_group_id(link_pos, id_b);
    }
    else{
        ++manage_cluster_v2_d;
        /// merge cluster
        // no need to add nodes to the cluster. they are already there

        // base is the larger cluster (number of nodes in it)
        size_t size_a = _cluster[index_a].numberOfNodes();
        size_t size_b = _cluster[index_b].numberOfNodes();
        if(size_a > size_b){
//            cout << "size_a > size_b : surviving cluster " << id_a << " : " << __LINE__ << endl;
            merge_cluster(link_pos, lnk, id_a, id_b, node_b);

        }else{
//            cout << "size_a <= size_b : surviving cluster " << id_b << " : " << __LINE__ << endl;
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
void NetworkBApercolation_v4::merge_cluster(size_t link_pos, const Link &lnk, int id_a, int id_b,
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


void NetworkBApercolation_v4::viewCluster() {
    size_t count{};
    cout << "viewCluster : line " << __LINE__ << endl;
    for(size_t i{}; i < _cluster.size(); ++i){
        if(_cluster[i].empty()){
            continue;
        }
        ++count;
        cout << "cluster[" << i << "] : id " << _cluster[i].get_group_id() << "{" << endl;
        cout << "  Nodes (" << _cluster[i].numberOfNodes() << "): ";
        _cluster[i].viewNodes();
        cout << endl;
        cout << "  Links (" << _cluster[i].numberOfLinks() << "): ";
//        _cluster[i].viewLinks();
        cout << endl;
        cout << "}" << endl;
    }
    cout << "number of cluster " << count << endl;

}

void NetworkBApercolation_v4::viewClusterExtended() {
    size_t count{};
    cout << "viewClusterExtended : line " << __LINE__ << endl;
    for(size_t i{}; i < _cluster.size(); ++i){
        if(_cluster[i].empty()){
            continue;
        }
        ++count;
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
//        _cluster[i].viewLinksExtended();
        cout << endl;
        cout << "}" << endl;
    }
    cout << "number of cluster " << count << endl;
}

void NetworkBApercolation_v4::relabel_nodes(Cluster_v3& clstr, int id) {
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
double NetworkBApercolation_v4::entropy_v1() {
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

double NetworkBApercolation_v4::entropy_v2() {
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
void NetworkBApercolation_v4::subtract_entropy(Link &lnk) {
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

void NetworkBApercolation_v4::add_entropy(Link &lnk) {
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
size_t NetworkBApercolation_v4::selectLink() {
    if(index_var >= _link_count){
        return _link_count + 1; // so that program crashes or stops
    }
    // select a link randomly

    size_t pos = _randomized_link_indices[index_var];
    ++index_var;
    return pos;
}

/**
 * place the link whose index in network is passed as argument
 * @param link_pos
 * @return
 */
bool NetworkBApercolation_v4::placeSelectedLink(size_t link_pos) {
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

void NetworkBApercolation_v4::reset(int i) {
    cout << "reset NetworkBApercolation_v4: line " <<__LINE__ << endl;
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
//    _randomized_link_indices = _randomized_link_indices; // assigned inside randomize_v1() function
    randomize_v1();
}


/**
 * Must be called after entropy is calculated each time. If not then this function will not work.
 */
void NetworkBApercolation_v4::jump() {
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
void NetworkBApercolation_v4::track_cluster() {
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
void NetworkBApercolation_v4::track_cluster_v2() {
    size_t a = _last_lnk.get_a(); // both a and b belongs to the same cluster
    int a_index = _network_frame.get_node_group_id(a);

    ////
//    size_t b = _last_lnk.get_b();
//    int b_index = _network_frame.get_node_group_id(b);
//
//    if(a_index != b_index){
//        cout << "index should be equal " << __LINE__ << endl;
//    }

    if(a_index < 0){
        cerr << "why? : line " << __LINE__ << endl;
        a_index = _network_frame.get_node_group_id(_last_lnk.get_b());
    }
    size_t sz = _cluster[a_index].numberOfNodes();
    if(sz > _number_of_nodes_in_the_largest_cluster){
        _number_of_nodes_in_the_largest_cluster = sz;
        _self_cluster_jump = (_last_largest_cluster_id == a_index); // true only in case of self jump
        _last_largest_cluster_id = a_index;
    }else{
        _self_cluster_jump = false;
    }

}

void NetworkBApercolation_v4::time_summary() {
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
size_t NetworkBApercolation_v4::clusterCount() const {
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
void NetworkBApercolation_v4::view_randomized_indices() const {
    cout << "{";
    for(auto a: _randomized_link_indices){
        cout << a << ",";
    }
    cout << "}" << endl;
}

double NetworkBApercolation_v4::sizeSummary_in_MB() {

    double sz_cluster = 0;
    for (size_t i{}; i < _cluster.size() ; ++i){
        sz_cluster += _cluster[i].size_in_MB();
    }
    cout << "Total cluster size " << sz_cluster << " MB" << endl;

    double sz_link_list = sizeof(std::vector<uint>) + sizeof(uint) * _randomized_link_indices.size();
    sz_link_list /= (1024*1024);
    cout << "Link list size " << sz_link_list << " MB" << endl;

    double sz_net = _network_frame.size_in_MB();
    cout << "NetworkBA size " << sz_net << " MB" << endl;

    double sz_tot = sz_cluster + sz_link_list + sz_net;
    cout << "Total size " << sz_tot << " MB" << endl;
    return sz_tot;
}



