//
// Created by shahnoor on 10/30/19.
//

#include "network_percolation_v7.h"
#include <iostream>
#include <algorithm>

using namespace std;


NetworkPercolation_v7::NetworkPercolation_v7(Network_v2 *net) {
    _net = net;
    _link_count = _net->getLinkCount();
    _network_size = _net->getNodeCount();
    setRandomState(_net->getRandomState());
    list_of_link_indices.resize(_link_count);
    for(uint i{}; i < list_of_link_indices.size(); ++i) {
        list_of_link_indices[i]=i;
    }
    max_link_index = _link_count - 1;
    // initialize cluster
    _cluster_info.resize(_network_size);
};


void NetworkPercolation_v7::setRandomState(size_t seed) {
    _random_state = seed;
    _random_generator.seed(_random_state);
    cout << "random seed NetworkPercolation_v7 " << _random_state << endl;
}



/**
 * Negative value in _cluster_info element means that element is the size of the cluster and
 *      index of that element is the root index.
 * Positive value in _cluster_info element means that this element is not root but the value
 *      of the element will get you one step closer to root
 *
     //// directly find_root_v3 is implemented for better performance
 * @param a
 * @return
 */
int NetworkPercolation_v7::findRoot(int k) {
    //    auto start = std::chrono::system_clock::now(); // time measurement
//    int b = 0;
//    b = find_root_v1(a);
//    b = find_root_v2_test(a);
//    b = find_root_v2(a);
//    b = find_root_v3_test(a, 0);
//    b = find_root_v3(a);

    if(_cluster_info[k]<0) return k;
//    _cluster_info[i]=find_root_v3(_cluster_info[i]);
//    return _cluster_info[i];
    /*
     * int a = 2;
     * cout << (a=10) << endl; // this will print 10
     */
    return _cluster_info[k]=findRoot(_cluster_info[k]); // assign and return at the same time
//    return b;
}

int NetworkPercolation_v7::find_root_v1(int a) {
//    cout << "find_root_v1 : " << __LINE__ << endl;

    //finds root node starting from a
    //clusters : cluster sizes and roots are here
    //a        : the point where finding starts.


    if (_cluster_info[a] < 0){return a;}
//    cout << "starting loop, a = " <<  a << endl;
//    copy(_clusters.begin(), _clusters.end(), ostream_iterator<int>(cout, ","));
    int b;
    while(!(_cluster_info[a] < 0)) {
        b = _cluster_info[a];
//        cout << a << ", " << b << endl;
//        copy(_clusters.begin(), _clusters.end(), ostream_iterator<int>(cout, ","));
        a = b;
    }
//    cout << "loop finished" << endl;
//    copy(_clusters.begin(), _clusters.end(), ostream_iterator<int>(cout, ","));
    return a;
}

/**
 * finds root node starting from a.
 * Also replaces some values so that search from node a is faster next time
 *
 * @param a  : the point where finding starts.
 * @return
 */
int NetworkPercolation_v7::find_root_v2(int a) {

    if (_cluster_info[a] < 0){   return a;}
    vector<int> redirecting_indices;
    int b;
    while (!(_cluster_info[a] < 0)) {
        redirecting_indices.emplace_back(a);
        b = _cluster_info[a];
        a = b;

    }
//# so that next time search is at least one step lesser
    for (auto ri : redirecting_indices) {
        _cluster_info[ri] = a;
    }
    return a;
}

int NetworkPercolation_v7::find_root_v2_test(int a) {

    if (_cluster_info[a] < 0){   return a;}
    vector<int> redirecting_indices;
    int b;
    int depth=0;
    while (!(_cluster_info[a] < 0)) {
        redirecting_indices.emplace_back(a);
        b = _cluster_info[a];
        a = b;
        ++depth;
    }
    if(depth > _max_recursion_depth) _max_recursion_depth = depth;
//# so that next time search is at least one step lesser
    for (auto ri : redirecting_indices) {
        _cluster_info[ri] = a;
    }
    return a;
}
/**
 * Find root recursively
 * Since we never need to traverse more than a few hundreds
 * loop if we keep assiging roor index to all
 * cluster encountered, recursive approach might be better
 *
 * This method is copied from Digonto vai codes
 *
 * @param ptr1
 * @param i
 * @return
 */
//int find_root(int ptr1[],int i)
//{
//    if(ptr1[i]<0) return i;
//
//    return ptr1[i]=find_root_v3(ptr1,ptr1[i]);
//}
int NetworkPercolation_v7::find_root_v3(int i)
{
    if(_cluster_info[i]<0) return i;
//    _cluster_info[i]=find_root_v3(_cluster_info[i]);
//    return _cluster_info[i];
    /*
     * int a = 2;
     * cout << (a=10) << endl; // this will print 10
     */
    return _cluster_info[i]=find_root_v3(_cluster_info[i]); // assign and return at the same time
}

/*
 * Testing for max recursion depth.
 * Result: in terms of depth find_root_v2_test and find_root_v3_test are pretty much the same.
 * but recursive method is faster because we don't need another list to hold and assign values
 * separately
 */
int NetworkPercolation_v7::find_root_v3_test(int i, int depth)
{
    if(_cluster_info[i]<0) {
        if (depth > _max_recursion_depth)  _max_recursion_depth = depth;
//        cout << "max depth " << depth << endl;
        return i;
    }
    /*
    _cluster_info[i]=find_root_v3(_cluster_info[i]);
    return _cluster_info[i];
     */
    /*
     * int a = 2;
     * cout << (a=10) << endl; // this will print 10
     */
    return _cluster_info[i]=find_root_v3_test(_cluster_info[i],depth+1); // assign and return at the same time
}

void  NetworkPercolation_v7::mergeClusters(int root_a, int root_b) {
//#     print("both are roots, ", root_a, ", ", root_b)
    if (root_a == root_b) return;
//# join two roots
//#     print("before merging ")
#ifdef DEBUG_FLAG
    cout << "cluster size increasing " << endl;
#endif
    _cluster_info[root_a] = _cluster_info[root_a] + _cluster_info[root_b]; //# sizes must add up
    _cluster_info[root_b] = root_a; //# now clusters[b] points to a
//#     print("after merging ")
//#     print(clusters)
}

/**
 * number of negative values in clusters is the number of cluster
 */
size_t NetworkPercolation_v7::clusterCount(){
    size_t sm = 0;
    for (auto i : _cluster_info) {
        if (i < 0) sm += 1;
    }
    return sm;
}


size_t NetworkPercolation_v7::clusterSizeSum(){
    size_t sm = 0;
    for (auto i : _cluster_info) {
        if (i < 0) sm += -i;
    }
    return sm;
}

int NetworkPercolation_v7::clusterSize(int a) {
    int root = findRoot(a);
    return _cluster_info[root] * -1;
}

bool NetworkPercolation_v7::occupyLink() {
    if(occupied_link_count >= _link_count) return false;
    uint i = list_of_link_indices[occupied_link_count];
    return placeSelectedLink(i);
//     print(clusters)
}

bool NetworkPercolation_v7::placeSelectedLink(uint i) {// selecting sequentially from suffled list_of_link_indices is like selecting randomly
//     print(i)
//     print("number of remaining clusters", clusterCount(clusters))
    int a = _net->getNodeA(i);
    int b = _net->getNodeB(i);
//     cout << a << " and " << b << endl;
//     print(clusters)
// node a and b will be connected together
    int root_a = findRoot(a);
    int root_b = findRoot(b);
#ifdef DEBUG_FLAG
    cout << "roots " << root_a << ", " << root_b << endl;
#endif
    subtract_entropy(root_a, root_b);
    mergeClusters(root_a, root_b);
    add_entropy(root_a);
    track_largest_cluster(root_a);
    occupied_link_count++;
// make both cluster point to root cluster. so that search can be faster later
    return true;
}

/**
 * View the clusters
 */
void NetworkPercolation_v7::viewClusters() {
    cout << "clusters : (index) -> ((-size) or (ref to root)) (root or not?){" << endl;
//    copy(_clusters.begin(), _clusters.end(), ostream_iterator<int>(cout, ","));
    for(size_t i{}; i < _cluster_info.size(); ++i){
        cout << "[" << i << "] -> " << _cluster_info[i];
        if(_cluster_info[i] < 0) cout << " *";// root cluster
        cout << endl;
    }
    cout << "}" << endl;

}

/**
 * TODO : group together assignment that must be done once and that must be done every time
 */
void NetworkPercolation_v7::initialize() {
    cout << "Initializing Network ... " << std::flush;
    // initialize link indices

    initialize_cluster();
    cout << "done." << endl;
    occupied_link_count = 0;
    _entropy_val = log(_network_size);// initial entropy
    _largest_jump_entropy=0;
    _previous_entropy=0;
    _entropy_jump_tc=0;
    largest_cluster_size=0;
    largest_jump_cluster_size=0;
    _previous_cluster_size=0;
    _max_recursion_depth = 0;
    _findRoot_time=0;

    randomize_indices(list_of_link_indices);
    _randomized_indices = list_of_link_indices;
}

void NetworkPercolation_v7::initialize_cluster() {
    for(size_t i{}; i < _network_size; ++i){
        _cluster_info[i] = -1; // all clusters are of size 1 and all of them are root cluster
    }
}


void NetworkPercolation_v7::reset(int i) {
//    cout << "reset NetworkBApercolation_v3: line " <<__LINE__ << endl;

//    occupied_link_count = 0;
//    _entropy_val = log(_network_size);// initial entropy
//    _largest_jump_entropy=0;
//    _previous_entropy=0;
//    _entropy_jump_tc=0;
//    largest_cluster_size=0;
//    largest_jump_cluster_size=0;
//    _previous_cluster_size=0;
//    _max_recursion_depth = 0;
//    _findRoot_time=0;
//    viewNetwork();
    if(i == 1){
        _net->rebuild();
        // initialize the network again
    }
//    viewNetwork();
    initialize();
}

double NetworkPercolation_v7::entropy_v1() {
//    cout << "NetworkPercolation_v7::entropy_v1" << endl;
    double mu{}, H{};
    for(size_t i{}; i < _cluster_info.size(); ++i){
        mu = -1*_cluster_info[i]/double(_network_size); // negative values of a clusters are sizes
        if(mu > 0){
            H += mu * log(mu);
        }
    }
    return  -H;
//    _entropy_val = -H;
//    return _entropy_val;
}

void NetworkPercolation_v7::viewListOfLinkIndices() {
    cout <<"Link indices : (" << list_of_link_indices.size() << "):{";
    for(size_t i{}; i < list_of_link_indices.size(); ++i){
        cout << list_of_link_indices[i] <<",";
    }
    cout <<"}" << endl;
    cout <<"Randomized   : (" << _randomized_indices.size() << "):{";
    for(size_t i{}; i < _randomized_indices.size(); ++i){
        cout << _randomized_indices[i] <<",";
    }
    cout <<"}" << endl;
}

void NetworkPercolation_v7::randomize_indices(std::vector<uint>& a) {
    std::shuffle(a.begin(), a.end(), _random_generator);
}

/**
 * root index must be provided
 * @param root_a
 * @param root_b
 */
void NetworkPercolation_v7::subtract_entropy(int root_a, int root_b) {
    double mu_a = -_cluster_info[root_a]/double(_network_size);
    double mu_b = -_cluster_info[root_b]/double(_network_size);
    if(mu_a < 0 || mu_b < 0){
        cerr << "one of the root is not a root : line " << __LINE__ << endl;
    }
    double H{};

    H += mu_a * log(mu_a);
    if (root_a != root_b){
        H += mu_b * log(mu_b);
    }
    _entropy_val += H; // since subtracting
}

void NetworkPercolation_v7::add_entropy(int root_a) {
    double mu_a = -_cluster_info[root_a]/double(_network_size);
//    if(mu_a < 0){
//        cerr << "not root" << endl;
//        root_a = findRoot(root_a);
//        mu_a = -_cluster_info[root_a]/double(_network_size);
//    }
    double H{};
    if(mu_a > 0){
        H += mu_a * log(mu_a);
    }
//    cout << "add entropy " << H << endl;
    _entropy_val += -H; // since adding
}

double NetworkPercolation_v7::entropy_v2() {
    return _entropy_val;
}

/**
 * @param a : root index of any cluster
 */
void NetworkPercolation_v7::track_largest_cluster(int a) {
    int sz = -_cluster_info[a];
    if( sz > largest_cluster_size){
//        cout << "new largest [" << a << "] -> " << sz << endl;
        largest_cluster_size = sz;
        largest_cluster_index = a;
    }
}

/**
 * This method includes self cluster jump tracking
 * @param a : root index of any cluster
 */
void NetworkPercolation_v7::track_largest_cluster_v2(int a) {
    int sz = -_cluster_info[a];
    if( sz > largest_cluster_size){
//        cout << "new largest [" << a << "] -> " << sz << endl;
        largest_cluster_size = sz;
        _self_cluster_jump = (largest_cluster_index == a); // true only in case of self jump
        largest_cluster_index = a; // now largest_cluster_index
    }
}

/**
 * Must be called after entropy is calculated each time. If not then this function will not work.
 */
void NetworkPercolation_v7::jump() {
//    jump_v1();
    jump_v2();
}


/**
 * Only entropy jump
 */
void NetworkPercolation_v7::jump_v1() {
    double delta_H{};
    if(occupied_link_count == 1){
        _previous_entropy = _entropy_val;
    }else{
        delta_H = _entropy_val - _previous_entropy;
        _previous_entropy = _entropy_val; // be ready for next step
    }
    if(abs(delta_H) > abs(_largest_jump_entropy)){
        _largest_jump_entropy = delta_H;
        _entropy_jump_tc = relativeLinkDensity();
    }
}
/**
 * Entropy and Order parameter jump
 */
void NetworkPercolation_v7::jump_v2() {
    double delta_H{};
    size_t delta_P{};
    if(occupied_link_count > 1){
        delta_H = abs(_entropy_val - _previous_entropy);
        delta_P = largest_cluster_size - _previous_cluster_size;
    }
    _previous_entropy = _entropy_val; // be ready for next step
    _previous_cluster_size = largest_cluster_size;
//    cout << "jump_v2 delta_H " << delta_H << endl;
    if(delta_H > _largest_jump_entropy){
        _largest_jump_entropy = delta_H;
//        _entropy_jump_tc = relativeLinkDensity();
    }
    if(delta_P > largest_jump_cluster_size){
        largest_jump_cluster_size = delta_P;
    }
}

double NetworkPercolation_v7::entropy() {
//    return entropy_v1();
    return entropy_v2();
}

void NetworkPercolation_v7::summary() {
    cout << "_max_recursion_depth " << _max_recursion_depth << endl;
    cout << "_findRoot_time " << _findRoot_time << " sec" << endl;
}


