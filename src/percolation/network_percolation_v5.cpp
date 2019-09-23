//
// Created by shahnoor on 7/1/19.
//

#include "network_percolation_v5.h"
#include <algorithm>

using namespace std;

NetworkBApercolation_v5::NetworkBApercolation_v5(size_t m0, size_t m, size_t size) {
    _network_frame = NetworkBA_v2(m0, m);
    _N = size;
    one_by_size = 1.0/_N;
    log_1_by_size = log(one_by_size);
}

void NetworkBApercolation_v5::setRandomState(size_t seed, bool g) {
    _random_state = seed;
    if(g){
        std::random_device rd;
        _random_state = rd();
    }
    _random.seed(_random_state);
}

void NetworkBApercolation_v5::init(bool g) {
    // only need to do once
    _network_frame.setRandomState(_random_state, g);
    _network_frame.grow(_N); // network construction

    list_of_link_indices.resize(_network_frame.getLinkCount());
    for(uint i{}; i < list_of_link_indices.size(); ++i){
        list_of_link_indices[i] = i;
    }

    // need to do when resetting
    _clusters = vector<int>(_N, -1);
    occupied_link_count = 0;
    // shuffle the value of the list
    shuffle(list_of_link_indices.begin(), list_of_link_indices.end(), _random);
}

int NetworkBApercolation_v5::findRoot(int a) {
    int b = 0;
//    b = find_root_v1(a);
    b = find_root_v2(a);
    return b;
}

int NetworkBApercolation_v5::find_root_v1(int a) {
//    cout << "find_root_v1 : " << __LINE__ << endl;

    //finds root node starting from a
    //clusters : cluster sizes and roots are here
    //a        : the point where finding starts.


    if (_clusters[a] < 0){return a;}
//    cout << "starting loop, a = " <<  a << endl;
//    copy(_clusters.begin(), _clusters.end(), ostream_iterator<int>(cout, ","));
    int b;
    while(!(_clusters[a] < 0)) {
        b = _clusters[a];
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
int NetworkBApercolation_v5::find_root_v2(int a) {

    if (_clusters[a] < 0){   return a;}
    vector<int> redirecting_indices;
    int b;
    while (!(_clusters[a] < 0)) {
        redirecting_indices.emplace_back(a);
        b = _clusters[a];
        a = b;

    }
//# so that next time search is at least one step lesser
    for (auto ri : redirecting_indices) {
        _clusters[ri] = a;
    }
    return a;
}

void  NetworkBApercolation_v5::mergeClusters(int root_a, int root_b) {
//#     print("both are roots, ", root_a, ", ", root_b)
    if (root_a == root_b) return;
//# join two roots
//#     print("before merging ")
//#     print(clusters)
    _clusters[root_a] = _clusters[root_a] + _clusters[root_b]; //# sizes must add up
    _clusters[root_b] = root_a; //# now clusters[b] points to a
//#     print("after merging ")
//#     print(clusters)
}

/**
 * number of negative values in clusters is the number of cluster
 */
size_t NetworkBApercolation_v5::clusterCount(){
    size_t sm = 0;
    for (auto i : _clusters) {
        if (i < 0) sm += 1;
    }
    return sm;
}


size_t NetworkBApercolation_v5::clusterSizeSum(){
    size_t sm = 0;
    for (auto i : _clusters) {
        if (i < 0) sm += -i;
    }
    return sm;
}

int NetworkBApercolation_v5::clusterSize(int a) {
    int root = findRoot(a);
    return _clusters[root] * -1;
}

bool NetworkBApercolation_v5::placeLink() {
    if(occupied_link_count >= _network_frame.getLinkCount()) return false;
    uint i = list_of_link_indices[occupied_link_count];

// selecting sequentially from suffled list_of_link_indices is like selecting randomly
//     print(i)
//     print("number of remaining clusters", clusterCount(clusters))
    int a = _network_frame.fromNetworkMapA(i);
    int b = _network_frame.fromNetworkMapB(i);
//     print(a, " and ", b)
//     print(clusters)
// node a and b will be connected together
    int root_a = findRoot(a);
    int root_b = findRoot(b);
    mergeClusters(root_a, root_b);
    occupied_link_count++;
// make both cluster point to root cluster. so that search can be faster later
//     print(clusters)
    return true;
}

/**
 * View the clusters
 */
void NetworkBApercolation_v5::viewCLusters() {
    cout << "{";
//    copy(_clusters.begin(), _clusters.end(), ostream_iterator<int>(cout, ","));
    for(size_t i{}; i < _clusters.size(); ++i){
        cout << i << " -> " << _clusters[i];
        if(_clusters[i] < 0) cout << " *";// root cluster
        cout << endl;
    }
    cout << "}" << endl;

}

void NetworkBApercolation_v5::initialize_network() {
    cout << "Initializing Network ... " << std::flush;
//    _network_frame.grow(_N);
    cout << "done." << endl;
}


void NetworkBApercolation_v5::reset(int i) {
//    cout << "reset NetworkBApercolation_v3: line " <<__LINE__ << endl;


    if(i == 1){
        // initialize the network again
        _network_frame.reset(); // this will do the trick
        _network_frame.grow(_N);
//        initialize_network();
    }

}