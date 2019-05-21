//
// Created by shahnoor on 5/20/19.
//


#include "network_percolation.h"

/**
 *
 * @param m0
 * @param m
 * @param size
 * @param M
 */
NetworkBApercolationExplosive_v3::NetworkBApercolationExplosive_v3(size_t m0, size_t m, size_t size, uint M)
        : NetworkBApercolation_v3(m0, m, size)
{
    if (M > 2) {
        if(M > MAX_M_VALUE){
            cout << "M exceeds maximum allowed value : line " << __LINE__ << endl;
            _M = MAX_M_VALUE;
        }
        _M = M;
    }
}

bool NetworkBApercolationExplosive_v3::placeLink() {
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
size_t NetworkBApercolationExplosive_v3::selectLink(char rule ) {
    if(_number_of_occupied_links >= _link_count){
        return _link_count+1;
    }
    auto start = std::chrono::system_clock::now(); // time measurement
    size_t r{};
    if(rule == 's'){
        r = link_for_min_cluster_sum_rule(_number_of_occupied_links);
    }else if(rule =='p'){
        r = link_for_min_cluster_product_rule(_number_of_occupied_links);
    }else{
        cerr << "bad argument : line " << __LINE__ << endl;
        return _link_count + 1; // to terminate the program
    }

    size_t pos = _randomized_indices[r];
    // must erase the used value. // TODO but erasing takes more than 90% of total time
//    _randomized_indices.erase(_randomized_indices.begin() + r);
    // instead of erasing the index, try swapping it with _number_of_occupied_links index\

    // 2019.05.20 better idea is just to replace the used value of _randomized_indices
    // by the first unused value, i.e., value at _number_of_occupied_links

    _randomized_indices[r] = _randomized_indices[_number_of_occupied_links];

    auto end = std::chrono::system_clock::now(); // time measurement
    std::chrono::duration<double> elapsed_seconds = end-start; // time measurement
    _time_selectLink += elapsed_seconds.count();
    return pos;
}

/**
 *
 * @param rule : specify sum rule or product rule
 *               values : 's' for sum rule and 'p' for product rule
 * @return
 */
size_t NetworkBApercolationExplosive_v3::selectLink_v2() {
    if(_number_of_occupied_links >= _link_count){
        return _link_count+1;
    }
    auto start = std::chrono::system_clock::now(); // time measurement

    size_t pos = link_for_min_cluster_sum_product_v2(_number_of_occupied_links);

    auto end = std::chrono::system_clock::now(); // time measurement
    std::chrono::duration<double> elapsed_seconds = end-start; // time measurement
    _time_selectLink += elapsed_seconds.count();
    return pos;
}


/**
 * For minimizing cluster sizes using sum rule
 *
 * version 2: this function starts to read _M values of _randomized_indices array starting at start_at
 *
 * @param start_at
 * * @return index of the link in the _randomized_indices for which cluster size becomes minimum
 */
size_t NetworkBApercolationExplosive_v3::link_for_min_cluster_sum_rule(size_t start_at) {
    auto start = std::chrono::system_clock::now(); // time measurement
    size_t index_randomized_link;

    size_t tmp_lnk_index, index1, index2;
    int id1, id2;
    size_t sum{}, n_nodes;
    Link tmp_lnk;
    size_t limit = _M + start_at;
//    cout << "checking link indices {";
    for(size_t i{start_at}; i < limit; ++i){
        if(i >= _randomized_indices.size()){
//            cout << "not enough free link : line " << __LINE__ << endl;
            break;
        }

        id1 = -1;
        id2 = -1;
        tmp_lnk_index = _randomized_indices[i];
//        cout << tmp_lnk_index << ","; // cout
        tmp_lnk = _network_frame.getLink(tmp_lnk_index);
        if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1
           && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
            // since we are minimizing cluster sizes
            ++_count_link_for_min_cluster_sum_rule_a;
            index_randomized_link = i;
//            cout << "got link " << _network_frame.getLink(_randomized_indices[i]) << " id = " << id1 << " and " << id2 << endl;
            break; // since this is the minimum link case
        }
        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) == -1
                && _network_frame.get_node_group_id(tmp_lnk.get_b()) != -1){
            ++_count_link_for_min_cluster_sum_rule_b;
            id1 = _network_frame.get_node_group_id(tmp_lnk.get_b());
            index1 = size_t(id1);
            n_nodes = _cluster[index1].numberOfNodes() + 1; // 1 node would have been added
            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
                sum = n_nodes;
                index_randomized_link = i;
            }
        }
        else if(_network_frame.get_node_group_id(tmp_lnk.get_a()) != -1
                && _network_frame.get_node_group_id(tmp_lnk.get_b()) == -1){
            ++_count_link_for_min_cluster_sum_rule_b;
            id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
            index1 = size_t(id1);
            n_nodes = _cluster[index1].numberOfNodes() + 1; // 1 node would have been added
            if(sum == 0 || sum > n_nodes) { // since we are minimizing cluster sizes
                sum = n_nodes;
                index_randomized_link = i;
            }
        }
        else{
            ++_count_link_for_min_cluster_sum_rule_c;
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
//    cout << "} => " ;
    if(index_randomized_link >= _randomized_indices.size()){
        cerr << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "selected link " << _network_frame.getLink(_randomized_indices[index_randomized_link])
//            << " id = " << id1 << " and " << id2 << " sum= " << sum << endl;
    auto end = std::chrono::system_clock::now(); // time measurement
    std::chrono::duration<double> elapsed_seconds = end-start; // time measurement
    _time_link_for_min_cluster_sum_rule += elapsed_seconds.count();
//    cout << "selected " << _randomized_indices[index_randomized_link] << endl;
    return index_randomized_link;
}


/**
 * For minimizing cluster sizes using sum rule
 * version 2: this function starts to read _M values of _randomized_indices array starting at start_at
 * @return index of the link in the _randomized_indices for which cluster size become minimum
 */
size_t NetworkBApercolationExplosive_v3::link_for_min_cluster_product_rule(size_t start_at) {
    size_t index_randomized_link;

    size_t tmp_lnk_index, index1, index2;
    int id1, id2;
    size_t prod{1}, n_nodes;
    Link tmp_lnk;
    size_t limit = start_at + _M;
    for(size_t i{start_at}; i < limit; ++i){
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

/**
 * For minimizing cluster sizes using sum rule
 * version 1:
 *  (a) this function starts to read _M values of _randomized_indices array starting at start_at
 *  (b) both sum rule and product rule is present here but one of them is commented
 * @return value of the link in the _randomized_indices for which cluster size become minimum
 */
size_t NetworkBApercolationExplosive_v3::link_for_min_cluster_sum_product(size_t start_at) {
    size_t index_randomized_link{0};

    size_t tmp_lnk_index, index1, index2;
    int id1{-1}, id2{-1};
    size_t prod_sum, n_nodes;
    prod_sum = 2*largestClusterSize()*largestClusterSize() + 100; // so that it is very big before going into the loop
//    cout << "prod_sum " << prod_sum << endl;
    Link tmp_lnk;
    size_t limit = start_at + _M;
    for(size_t i{start_at}; i < limit; ++i){
        if(i >= _randomized_indices.size()){
//            cout << "not enough free link : line " << __LINE__ << endl;
            break;
        }

        tmp_lnk_index = _randomized_indices[i];
        tmp_lnk = _network_frame.getLink(tmp_lnk_index);

        id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
        id2 = _network_frame.get_node_group_id(tmp_lnk.get_b());
        index1 = size_t(id1);
        index2 = size_t(id2);
        if(id1 == id2){
            // if they belong to same cluster. adding new link does not increase cluster size.
            n_nodes = _cluster[index1].numberOfNodes();
//            cout << "same cluster " << endl;
        }else {
            n_nodes = _cluster[index1].numberOfNodes() * _cluster[index2].numberOfNodes(); // product rule
//            n_nodes = _cluster[index1].numberOfNodes() + _cluster[index2].numberOfNodes(); // sum rule
        }
        if(n_nodes < prod_sum ) { // since we are minimizing cluster sizes
            prod_sum = n_nodes;
            index_randomized_link = i;
        }

//        cout << "checking link " << _network_frame.getLink(_randomized_indices[i])
//             << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;
    }
    if(index_randomized_link >= _randomized_indices.size()){
        cout << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "selected link " << _network_frame.getLink(_randomized_indices[index_randomized_link])
//         << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;

    size_t tmp = _randomized_indices[index_randomized_link];
    // must assign to a temp variable, otherwise it is replaced before retrurn
    // must erase the used value. // but erasing takes more than 90% of total time
//    _randomized_indices.erase(_randomized_indices.begin() + r);
    // instead of erasing the index, try swapping it with _number_of_occupied_links index\

    // 2019.05.20 better idea is just to replace the used value of _randomized_indices
    // by the first unused value, i.e., value at _number_of_occupied_links
    _randomized_indices[index_randomized_link] = _randomized_indices[_number_of_occupied_links];
    _randomized_indices[_number_of_occupied_links] = 0; // does not affect the result. use only when debugging so that we can identify easily
    return tmp;
}

/**
 * For minimizing cluster sizes using sum rule
 * version 1:
 *  (a) this function starts to read _M values of _randomized_indices array starting at start_at
 *  (b) both sum rule and product rule is present here but one of them is commented
 * @return value of the link in the _randomized_indices for which cluster size become minimum
 */
size_t NetworkBApercolationExplosive_v3::link_for_min_cluster_sum_product_v2(size_t start_at) {
    size_t index_randomized_link{0};

    size_t tmp_lnk_index, index1, index2;
    int id1{-1}, id2{-1};
    size_t prod_sum, n_nodes;
    prod_sum = 2*largestClusterSize()*largestClusterSize() + 100; // so that it is very big before going into the loop
//    cout << "prod_sum " << prod_sum << endl;
    Link tmp_lnk;
    size_t limit = start_at + _M;
    size_t r{};
//    cout << "randomly between ("<< start_at <<"," << _link_count << ")={";
    for(size_t i{0}; i < _M; ++i){
        // todo : what happens when start_at is near to the size of _randomized_indices.
        // random number between a and b or rand(a,b) = a + r%(b-a); where r is an arbitrary random number
        r = start_at + _random_generator() % (_link_count-start_at);

        tmp_lnk_index = _randomized_indices[r];
//        cout << r << ":" << tmp_lnk_index << ",";
        tmp_lnk = _network_frame.getLink(tmp_lnk_index);

        id1 = _network_frame.get_node_group_id(tmp_lnk.get_a());
        id2 = _network_frame.get_node_group_id(tmp_lnk.get_b());
        index1 = size_t(id1);
        index2 = size_t(id2);
        if(id1 == id2){
            // if they belong to same cluster. adding new link does not increase cluster size.
            n_nodes = _cluster[index1].numberOfNodes();
//            cout << "same cluster " << endl;
        }else {
            n_nodes = _cluster[index1].numberOfNodes() * _cluster[index2].numberOfNodes(); // product rule
//            n_nodes = _cluster[index1].numberOfNodes() + _cluster[index2].numberOfNodes(); // sum rule
        }
        if(n_nodes < prod_sum ) { // since we are minimizing cluster sizes
            prod_sum = n_nodes;
            index_randomized_link = r;
        }

//        cout << "checking link " << _network_frame.getLink(_randomized_indices[i])
//             << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;
    }
    if(index_randomized_link >= _randomized_indices.size()){
        cout << "out of bound : line " << __LINE__ << endl;
    }
//    cout << "}" << endl;
//    cout << "selected link " << _network_frame.getLink(_randomized_indices[index_randomized_link])
//         << " id = " << id1 << " and " << id2 << " prod_sum= " << prod_sum << endl;
//    cout << " at " << index_randomized_link  << " value " << _randomized_indices[index_randomized_link] << endl;

    size_t pos = _randomized_indices[index_randomized_link];
    // must assign to a temp variable, otherwise it is replaced before retrurn
    // must erase the used value. // but erasing takes more than 90% of total time
//    _randomized_indices.erase(_randomized_indices.begin() + r);
    // instead of erasing the index, try swapping it with _number_of_occupied_links index\

    // 2019.05.20 better idea is just to replace the used value of _randomized_indices
    // by the first unused value, i.e., value at _number_of_occupied_links

    _randomized_indices[index_randomized_link] = _randomized_indices[_number_of_occupied_links];

    _randomized_indices[_number_of_occupied_links] = 0; // does not affect the result. use only when debugging so that we can identify easily

    return pos;
}

bool NetworkBApercolationExplosive_v3::occupyLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
//    size_t pos = selectLink('s');
    size_t pos = selectLink_v2();
    return NetworkBApercolation_v3::placeSelectedLink(pos);
}

void NetworkBApercolationExplosive_v3::time_summary() {
    NetworkBApercolation_v3::time_summary();
    cout << "_time_selectLink " << _time_selectLink << " sec" << endl;
    cout << "_time_link_for_min_cluster_sum_rule " << _time_link_for_min_cluster_sum_rule << " sec" << endl;
    cout << "_count_link_for_min_cluster_sum_rule_a " << _count_link_for_min_cluster_sum_rule_a << " times" << endl;
    cout << "_count_link_for_min_cluster_sum_rule_b " << _count_link_for_min_cluster_sum_rule_b << " times" << endl;
    cout << "_count_link_for_min_cluster_sum_rule_c " << _count_link_for_min_cluster_sum_rule_c << " times" << endl;

}
