//
// Created by shahnoor on 10/30/19.
//

#include "network_percolation_v7.h"
#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;


NetworkPercolation_v7::NetworkPercolation_v7(Network_v7 *net) {
    _net = net;
    _link_count = _net->getLinkCount();
    _network_size = _net->getNodeCount();
    setRandomState(_net->getRandomState());
    list_of_link_indices.resize(_link_count);
    for(uint i{}; i < _link_count; ++i) {
        list_of_link_indices[i]=i;
    }
    max_link_index = _link_count - 1;

    _number_of_clusters = _network_size;

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
//    b = find_root_v3(k);

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
    while(_cluster_info[a] >= 0) {
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

int  NetworkPercolation_v7::mergeClusters(int root_a, int root_b) {
//    return merge_cluster_v1(root_a, root_b);
    return merge_cluster_v2(root_a, root_b);
}

int NetworkPercolation_v7::merge_cluster_v1(int root_a, int root_b)  {//#     print("both are roots, ", root_a, ", ", root_b)
    if (root_a == root_b) return root_a;
//# join two roots
//#     print("before merging ")
#ifdef DEBUG_FLAG
    cout << "cluster size increasing " << endl;
#endif
    _cluster_info[root_a] = _cluster_info[root_a] + _cluster_info[root_b]; //# sizes must add up
    _cluster_info[root_b] = root_a; //# now clusters[b] points to a
//#     print("after merging ")
//#     print(clusters)
    return root_a;
}

/**
 * select larger cluster as root
 * @param root_a
 * @param root_b
 */
int NetworkPercolation_v7::merge_cluster_v2(int root_a, int root_b)  {
//#     print("both are roots, ", root_a, ", ", root_b)
    if (root_a == root_b) return root_a;
//# join two roots
//#     print("before merging ")
#ifdef DEBUG_FLAG
    cout << "cluster size increasing " << endl;
#endif
    if(abs(_cluster_info[root_a]) > abs(_cluster_info[root_b])) {
        _cluster_info[root_a] = _cluster_info[root_a] + _cluster_info[root_b]; //# sizes must add up
        _cluster_info[root_b] = root_a; //# now clusters[b] points to a
        return root_a;
    }else{
        _cluster_info[root_b] = _cluster_info[root_a] + _cluster_info[root_b]; //# sizes must add up
        _cluster_info[root_a] = root_b; //# now clusters[a] points to b
        return root_b;
    }
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

size_t NetworkPercolation_v7::clusterCountFast(){
    return _number_of_clusters;
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
    uint i = _randomized_indices[occupied_link_count];
    return placeSelectedLink(i);
//     print(clusters)
}

bool NetworkPercolation_v7::placeSelectedLink(uint i) {
//    if(occupied_link_count >= _link_count) return false;
// selecting sequentially from suffled list_of_link_indices is like selecting randomly
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
    auto root = mergeClusters(root_a, root_b);
    add_entropy(root);
    track_largest_cluster(root);
    track_cluster_count(root_a, root_b);
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
    int sum=0;
    for(size_t i{}; i < _cluster_info.size(); ++i){
        cout << "[" << i << "] -> " << _cluster_info[i];
        if(_cluster_info[i] < 0) {
            cout << " *"; // root cluster
            sum += _cluster_info[i];
        }
        cout << endl;
    }
    cout << "}" << endl;
    cout << "Network size from cluster " << abs(sum) << endl;
    if(abs(sum) != _network_size){
        cout << "size mismatched" << endl;
    }
}

void NetworkPercolation_v7::sumClusters() {
//    cout << "clusters : (index) -> ((-size) or (ref to root)) (root or not?){" << endl;
//    copy(_clusters.begin(), _clusters.end(), ostream_iterator<int>(cout, ","));
    int sum=0;
    for(size_t i{}; i < _cluster_info.size(); ++i){
        if(_cluster_info[i] < 0) {
            sum += _cluster_info[i];
        }
    }
    cout << "Network size from cluster " << abs(sum) << endl;
    if(abs(sum) != _network_size){
        cout << "size mismatched" << endl;
    }
}

/**
 * group together assignment that must be done once and that must be done every time
 */
void NetworkPercolation_v7::initialize() {
//    cout << "Initializing Network ... " << std::flush;
    // initialize link indices

    initialize_cluster();
//    cout << "initialize_cluster() done." << endl;
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
    _number_of_clusters=_network_size;
    _randomized_indices = list_of_link_indices;
    randomize_indices(_randomized_indices);
//    cout << "max link index " << max_link_index << endl;

}

void NetworkPercolation_v7::initialize_cluster() {
    // initialize cluster
    _cluster_info.resize(_network_size);
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
        cout << "rebuilding..." << endl;
        _net->rebuild();
        // initialize the network again
        _net->clearAdjacency(); // not required for percolation
    }
//    viewNetwork();
    initialize();
//    cout << "initialize() done." << endl;
}

long double NetworkPercolation_v7::entropy_v1() {
//    cout << "NetworkPercolation_v7::entropy_v1" << endl;
    long double mu{}, H{};
    for(size_t i{}; i < _cluster_info.size(); ++i){
        mu = -1*_cluster_info[i]/double(_network_size); // negative values of a clusters are sizes
        if(mu > 0){
//            H += mu * log(mu);
            H += mu * logl(mu);
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
    long double H{};
#ifdef  DEBUG_FLAG
    cout << "subtracting entropy for root {" << root_a;
    if (root_a != root_b){
        cout << "," << root_b;
    }
    cout << "}" << endl;
#endif
//    H += mu_a * log(mu_a);
    H += mu_a * logl(mu_a);
    if (root_a != root_b){
//        H += mu_b * log(mu_b);
        H += mu_b * logl(mu_b);
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
#ifdef  DEBUG_FLAG
    cout << "adding entropy for root {" << root_a << "}" << endl;
#endif
    double H{};
    if(mu_a > 0){
//        H += mu_a * log(mu_a);
        H += mu_a * logl(mu_a);
    }
//    cout << "add entropy " << H << endl;
    _entropy_val += -H; // since adding
}

long double NetworkPercolation_v7::entropy_v2() {
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

    if(occupied_link_count > 1){
        delta_H = _entropy_val - _previous_entropy;
        delta_P = largest_cluster_size - _previous_cluster_size;
    }
    _previous_entropy = _entropy_val; // be ready for next step
    _previous_cluster_size = largest_cluster_size;
//    cout << "jump_v2 delta_H " << delta_H << endl;
    if(abs(delta_H) > abs(_largest_jump_entropy)){
        _largest_jump_entropy = delta_H;
//        _entropy_jump_tc = relativeLinkDensity();
    }
    if(abs(delta_P) > abs(largest_jump_cluster_size)){
        largest_jump_cluster_size = delta_P;
    }
}

long double NetworkPercolation_v7::entropy() {
//    return entropy_v1();
    return entropy_v2();
}

void NetworkPercolation_v7::summary() {
    cout << "_max_recursion_depth " << _max_recursion_depth << endl;
    cout << "_findRoot_time " << _findRoot_time << " sec" << endl;
}

long double NetworkPercolation_v7::jump_entropy() {
    return delta_H;
}

long NetworkPercolation_v7::jump_largest_cluster() {
    return delta_P;
}

void NetworkPercolation_v7::track_cluster_count(int root_a, int root_b) {
    if (root_a != root_b){
        // different clusters will be merged
        if(_number_of_clusters == 0){
            cerr << "cannot decrease more : line " << __LINE__ << endl;
        }
        --_number_of_clusters;
    }
}

/*******************************
 *  Inverted explosive
 *  sum rule or product rule, activate links which maximizes cluster size
 *****************************/
NetworkPercolationInverted_v7::NetworkPercolationInverted_v7(Network_v7 *net, int M)
        :NetworkPercolation_v7(net) {
    if(M < 1){
        cout << "M cannot be < 1 : " << __LINE__ << endl;
    }
    _M_link = M;
}

bool NetworkPercolationInverted_v7::occupyLink() {
    if(occupied_link_count >= _link_count) return false;
    uint i = link_for_max_cluster_sum_product(occupied_link_count);
    return NetworkPercolation_v7::placeSelectedLink(i);
}

uint NetworkPercolationInverted_v7::link_for_max_cluster_sum_product(size_t start_at) {
//    auto start = std::chrono::system_clock::now(); // time measurement
    size_t index_randomized_link{0};

    uint tmp_lnk_index;
    int id1{-1}, id2{-1}, root1, root2;
    long n_nodes, prod_sum = 1; // so that it is very big before going into the loop
//    cout << LONG_MAX << " vs " << prod_sum << endl;
//    cout << ULONG_MAX << " vs " << prod_sum << endl;
    size_t r{};
    std::uniform_int_distribution<size_t> distribution(start_at, max_link_index);

    for(int i{0}; i < _M_link; ++i){
        // random number between a and b or rand(a,b) = a + r%(b-a); where r is an arbitrary random number
        // must select radnomly with uniform probability
//        r = start_at + _random_generator() % (_link_count-start_at);
        r = distribution(_random_generator);
        tmp_lnk_index = _randomized_indices[r];

        id1 = _net->getNodeA(tmp_lnk_index);
        id2 = _net->getNodeB(tmp_lnk_index);

        root1 = findRoot(id1);
        root2 = findRoot(id2);

        n_nodes = long(_cluster_info[root1]) * long(_cluster_info[root2]); // product rule. automatically becomes positive
//            n_nodes = abs(_cluster_info[root1] + _cluster_info[root2]); // sum rule
        if(n_nodes > prod_sum ) { // since we are maximize cluster sizes
            prod_sum = n_nodes;
            index_randomized_link = r;
        }
#ifdef DEBUG_FLAG
        cout << "checking link _randomized_indices[" << r << "]=" << tmp_lnk_index << endl;
        cout << " out of  id = " << id1 << " and " << id2 << " prod_sum = " << prod_sum << endl;
#endif
    }

    if(index_randomized_link >= _randomized_indices.size()){
        cout << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "}" << endl;
#ifdef DEBUG_FLAG
    cout << "selected _randomized_indices[" << index_randomized_link << "] = "
         << _randomized_indices[index_randomized_link] << endl;
#endif

    uint pos = _randomized_indices[index_randomized_link];

    _randomized_indices[index_randomized_link] = _randomized_indices[occupied_link_count];
    _randomized_indices[occupied_link_count] = 0; // does not affect the result. use only when debugging so that we can identify easily
#ifdef DEBUG_FLAG
    if(pos > UINT_MAX){
        cerr << "warning ! pos > UINT_MAX :NetworkPercolationInverted_v7::link_for_max_cluster_sum_product: line " << __LINE__ << endl;
    }
#endif
    return pos;
}

//void NetworkPercolationInverted_v7::initialize() {
//    NetworkPercolation_v7::initialize();
//}

/*****************************************
 * Explosive percolation
 * Minimizing cluster size using sum rule or product rule
 **************************************************/


NetworkPercolationExplosive_v7::NetworkPercolationExplosive_v7(Network_v7 *net, int M)
        :NetworkPercolation_v7(net)
{
    _M_link = M;
    cout << getClassName() << endl;

// tc values
    tcs[2] = 0.7272;
    tcs[3] = 0.8847;
    tcs[4] = 0.9444;
    tcs[5] = 0.9712;
    tcs[6] = 0.9844;
    tcs[7] = 0.9914;
    tcs[8] = 0.9951;
    tcs[9] = 0.9971;
    tcs[10] = 0.9983;
}

uint NetworkPercolationExplosive_v7::link_for_min_cluster_sum_product(size_t start_at) {
    //    auto start = std::chrono::system_clock::now(); // time measurement
    size_t index_randomized_link{0};

    uint tmp_lnk_index;
    int id1{-1}, id2{-1}, root1, root2;
    long n_nodes=0;
    long prod_sum = LONG_MAX; // so that it is very big before going into the loop
//    cout << LONG_MAX << " vs " << prod_sum << endl;
//    cout << ULONG_MAX << " vs " << prod_sum << endl;
//    size_t limit = start_at + _M_link;
    size_t r{};
//    cout << "randomly between ("<< start_at <<"," << _link_count << ")={";
    std::uniform_int_distribution<size_t> distribution(start_at, max_link_index);
    for(int i{0}; i < _M_link; ++i){
//        r = start_at + _random_generator() % (_link_count-start_at);
        r = distribution(_random_generator);
        tmp_lnk_index = _randomized_indices[r];

        id1 = _net->getNodeA(tmp_lnk_index);
        id2 = _net->getNodeB(tmp_lnk_index);

        root1 = findRoot(id1);
        root2 = findRoot(id2);

        n_nodes = long(_cluster_info[root1]) * long(_cluster_info[root2]); // product rule. automatically becomes positive
//            n_nodes = abs(_cluster_info[root1] + _cluster_info[root2]); // sum rule

#ifdef UNIT_TEST
        if(n_nodes < 0){
            cerr << "product cannot be negative " << endl
            << "NetworkPercolationExplosive_v7::link_for_min_cluster_sum_product: line "
            << __LINE__ << endl;
            cout << "possible solution : cast value from int to long in order to disolve this" << endl;
            exit(0);
        }
#endif
        if(cluster_extremizing_condition(n_nodes, prod_sum)) { // since we are minimizing cluster sizes
            prod_sum = n_nodes;
            index_randomized_link = r;
        }
#ifdef DEBUG_FLAG
        cout << "checking link _randomized_indices[" << r << "]=" << tmp_lnk_index << endl;
        cout << " out of  id = " << id1 << " and " << id2 << " prod_sum = " << prod_sum << endl;
#endif
    }

    if(index_randomized_link >= _randomized_indices.size()){
        cout << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "}" << endl;
#ifdef DEBUG_FLAG
    cout << "selected _randomized_indices[" << index_randomized_link << "] = "
         << _randomized_indices[index_randomized_link] << endl;
#endif

    uint pos = _randomized_indices[index_randomized_link];

    _randomized_indices[index_randomized_link] = _randomized_indices[occupied_link_count];
    // does not affect the result. use only when debugging so that we can identify easily
    _randomized_indices[occupied_link_count] = 0;
    return pos;
}

bool NetworkPercolationExplosive_v7::cluster_extremizing_condition(long n_nodes, long prod_sum) const { return n_nodes < prod_sum; }


bool NetworkPercolationExplosive_v7::occupyLink() {
    if(occupied_link_count >= _link_count) return false;
    uint i = link_for_min_cluster_sum_product(occupied_link_count);
//    uint i = _randomized_indices[occupied_link_count]; // regular random percolation
    return NetworkPercolation_v7::placeSelectedLink(i);
}
