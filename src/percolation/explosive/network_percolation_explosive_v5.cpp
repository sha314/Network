//
// Created by shahnoor on 9/29/2019.
//

#include "network_percolation_explosive_v5.h"



/**
 *
 * @param m0
 * @param m
 * @param size
 * @param M
 */
NetworkBApercolationExplosive_v5::NetworkBApercolationExplosive_v5(size_t m0, size_t m, size_t size, uint M)
        : NetworkBApercolation_v5(m0, m, size)
{
    if (M > 2) {
        if(M > MAX_M_VALUE){
            cout << "M exceeds maximum allowed value : line " << __LINE__ << endl;
            _M_link = MAX_M_VALUE;
        }
        _M_link = M;
    }
}


/**
 *
 * @param rule : specify sum rule or product rule
 *               values : 's' for sum rule and 'p' for product rule
 * @return
 */
uint NetworkBApercolationExplosive_v5::selectLink_v2() {
    if(occupied_link_count >= _link_count){
        return _link_count+1;
    }
    auto start = std::chrono::system_clock::now(); // time measurement

    uint pos = link_for_min_cluster_sum_product_v2(occupied_link_count);
    auto end = std::chrono::system_clock::now(); // time measurement
    std::chrono::duration<double> elapsed_seconds = end-start; // time measurement
    _time_selectLink += elapsed_seconds.count();
    return pos;
}


/**
 * For minimizing cluster sizes using sum rule
 * version 1:
 *  (a) this function starts to read _M values of _randomized_indices array starting at start_at
 *  (b) both sum rule and product rule is present here but one of them is commented
 * @return value of the link in the _randomized_indices for which cluster size become minimum
 */
uint NetworkBApercolationExplosive_v5::link_for_min_cluster_sum_product_v2(size_t start_at) {
    size_t index_randomized_link{0};

    int tmp_lnk_index;
    int id1{-1}, id2{-1}, root1, root2;
    int  n_nodes;
    size_t prod_sum = 2* largestClusterSize()*largestClusterSize() + 100; // so that it is very big before going into the loop
//    cout << "prod_sum " << prod_sum << endl;
    size_t limit = start_at + _M_link;
    size_t r{};
//    cout << "randomly between ("<< start_at <<"," << _link_count << ")={";
    for(size_t i{0}; i < _M_link; ++i){
        // todo : what happens when start_at is near to the size of _randomized_link_indices.
        // random number between a and b or rand(a,b) = a + r%(b-a); where r is an arbitrary random number
        r = start_at + _random() % (_link_count-start_at);

        tmp_lnk_index = _randomized_indices[r];


        id1 = _network_frame.getNodeA(tmp_lnk_index);
        id2 = _network_frame.getNodeB(tmp_lnk_index);

        root1 = findRoot(id1);
        root2 = findRoot(id2);

        if(root1 == root2){
            // if they belong to same cluster. adding new link does not increase cluster size.
            n_nodes = -_cluster_info[root1]; // value of clusters element is negative if it is the size
//            cout << "same cluster " << endl;
        }else {
            n_nodes = _cluster_info[root1] * _cluster_info[root2]; // product rule
//            n_nodes = _cluster[index1].numberOfNodes() + _cluster[index2].numberOfNodes(); // sum rule
        }
        if(n_nodes < prod_sum ) { // since we are minimizing cluster sizes
            prod_sum = n_nodes;
            index_randomized_link = r;
        }

//        cout << "checking link " << _network_frame.getLink(_randomized_link_indices[i])
//             << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;
    }
    if(index_randomized_link >= _randomized_indices.size()){
        cout << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "}" << endl;
//    cout << "selected link " << _network_frame.getLink(_randomized_link_indices[index_randomized_link])
//         << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;
//    cout << " at " << index_randomized_link  << " value " << _randomized_link_indices[index_randomized_link] << endl;

    size_t pos = _randomized_indices[index_randomized_link];
    // must assign to a temp variable, otherwise it is replaced before retrurn
    // must erase the used value. // but erasing takes more than 90% of total time
//    _randomized_link_indices.erase(_randomized_link_indices.begin() + r);
    // instead of erasing the index, try swapping it with _number_of_occupied_links index\

    // 2019.05.20 better idea is just to replace the used value of _randomized_link_indices
    // by the first unused value, i.e., value at _number_of_occupied_links

    _randomized_indices[index_randomized_link] = _randomized_indices[occupied_link_count];

    _randomized_indices[occupied_link_count] = 0; // does not affect the result. use only when debugging so that we can identify easily

    return pos;
}

/**
 * For minimizing cluster sizes using sum rule
 * version 3:
 *  (a) version 1 is implied
 *  (b) before tc we do explosive (join two cluster for which sum or product is minimum)
 *      and after tc we do inverted explosive (join two cluster for which sum or product is maximum).
 *
 *
 * @return value of the link in the _randomized_indices for which cluster size become minimum
 */
uint NetworkBApercolationExplosive_v5::link_for_min_cluster_sum_product_v3_adaptive(double tc, size_t start_at) {
    size_t index_randomized_link{0};
    double t = relativeLinkDensity();
    int tmp_lnk_index, root1, root2;
    int id1{-1}, id2{-1};
    size_t prod_sum, n_nodes;
    if (t < tc) {
        prod_sum = 2 * largestClusterSize() * largestClusterSize() +
                   100; // so that it is very big before going into the loop
    }else{
        prod_sum = 1; // smallest possible value for both sum and product
    }
//    cout << "prod_sum " << prod_sum << endl;
    size_t limit = start_at + _M_link;
    size_t r{};
//    cout << "randomly between ("<< start_at <<"," << _link_count << ")={";
    for(size_t i{0}; i < _M_link; ++i){
        // what happens when start_at is near to the size of _randomized_link_indices. ???
        // random number between a and b or rand(a,b) = a + r%(b-a); where r is an arbitrary random number
        r = start_at + _random() % (_link_count-start_at);

        tmp_lnk_index = _randomized_indices[r];
//        cout << r << ":" << tmp_lnk_index << ",";

        id1 = _network_frame.getNodeA(tmp_lnk_index);
        id2 = _network_frame.getNodeB(tmp_lnk_index);
        root1 = findRoot(id1);
        root2 = findRoot(id2);
        if(root1 == root2){
            // if they belong to same cluster. adding new link does not increase cluster size.
            n_nodes = -_cluster_info[root1];
//            cout << "same cluster " << endl;
        }else {
            n_nodes = _cluster_info[root1] * _cluster_info[root2]; // product rule
//            n_nodes = -(_cluster_info[index1] + _cluster_info[index2]); // sum rule
        }

        if(t < tc){
            if(n_nodes < prod_sum){
                prod_sum = n_nodes;
                index_randomized_link = r;
            }
        }else{
            if(n_nodes > prod_sum){
                prod_sum = n_nodes;
                index_randomized_link = r;
            }
        }


//        cout << "checking link " << _network_frame.getLink(_randomized_link_indices[i])
//             << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;
    }
    if(index_randomized_link >= _randomized_indices.size()){
        cout << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "}" << endl;
//    cout << "selected link " << _network_frame.getLink(_randomized_link_indices[index_randomized_link])
//         << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;
//    cout << " at " << index_randomized_link  << " value " << _randomized_link_indices[index_randomized_link] << endl;

    size_t pos = _randomized_indices[index_randomized_link];
    // must assign to a temp variable, otherwise it is replaced before retrurn
    // must erase the used value. // but erasing takes more than 90% of total time
//    _randomized_link_indices.erase(_randomized_link_indices.begin() + r);
    // instead of erasing the index, try swapping it with _number_of_occupied_links index\

    // 2019.05.20 better idea is just to replace the used value of _randomized_link_indices
    // by the first unused value, i.e., value at _number_of_occupied_links

    _randomized_indices[index_randomized_link] = _randomized_indices[occupied_link_count];

    _randomized_indices[occupied_link_count] = 0; // does not affect the result. use only when debugging so that we can identify easily

    return pos;
//    return 0;
}

bool NetworkBApercolationExplosive_v5::occupyLink() {
    if (occupied_link_count >= linkCount()){
        return false;
    }
    uint pos = selectLink_v2();
    return NetworkBApercolation_v5::placeSelectedLink(pos);
}

void NetworkBApercolationExplosive_v5::time_summary() {
//    NetworkBApercolation_v5::time_summary();
    cout << "_time_selectLink " << _time_selectLink << " sec" << endl;
    cout << "_time_link_for_min_cluster_sum_rule " << _time_link_for_min_cluster_sum_rule << " sec" << endl;
    cout << "_count_link_for_min_cluster_sum_rule_a " << _count_link_for_min_cluster_sum_rule_a << " times" << endl;
    cout << "_count_link_for_min_cluster_sum_rule_b " << _count_link_for_min_cluster_sum_rule_b << " times" << endl;
    cout << "_count_link_for_min_cluster_sum_rule_c " << _count_link_for_min_cluster_sum_rule_c << " times" << endl;

}


