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

bool NetworkBApercolationExplosive_v3::occupyLink() {
    if (_number_of_occupied_links >= _link_count){
        return false;
    }
    size_t pos = selectLink('s');
    return NetworkBApercolation_v3::placeSelectedLink(pos);
}

void NetworkBApercolationExplosive_v3::time_summary() {
    NetworkBApercolation_v3::time_summary();
    cout << "_time_selectLink " << _time_selectLink << " sec" << endl;
    cout << "_time_link_for_min_cluster_sum_rule " << _time_link_for_min_cluster_sum_rule << " sec" << endl;

}
