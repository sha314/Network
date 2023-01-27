//
// Created by shahnoor on 12/19/18.
//

#include "test_network_percolation.h"
#include "../nets/network.h"
#include "../util/time_tracking.h"
#include "../percolation/network_percolation.h"
#include "../percolation/reverse/network_percolation_reverse.h"
#include "../percolation/network_percolation_template.h"
#include "../percolation/network_percolation_v3.h"
#include "../percolation/explosive/network_percolation_explosive.h"
#include "../percolation/network_percolation_v5.h"
#include "../percolation/explosive/network_percolation_explosive_v5.h"
#include "../percolation/explosive/network_percolation_explosive_v3.h"
#include "../percolation/explosive/network_percolation_explosive_inverted.h"
#include "../percolation/explosive/network_percolation_explosive_v6.h"
#include <chrono>
#include <fstream>
#include <thread>
#include <cstdlib>

using namespace std;

///**
// * Date : 2019.05.17
// * @param argc
// * @param argv
// */
//void demarcationLine(int argc, char **argv){
//    int m = atoi(argv[1]);
//    int N = atoi(argv[2]);
//    int M = atoi(argv[3]);
//
////   NetworkBApercolationExplosive_v3 net(m*2, m, N, M);
////     NetworkBApercolation_v3 net(m*2, m, N);
//
////    NetworkBApercolationExplosive_v5 net(m*2, m, N, M);
//    NetworkBApercolation_v5 net(m*2, m, N);
//
//    string signature = net.get_signature();
//    string filename = signature + "jump-" + currentTime() + ".txt";
//
//
//
////    double t;
//    double P{}, P_old{}, dP;
//    bool c{false};
//
//
//    vector<double> dP_list(net.linkCount()), color_list(net.linkCount());
//
//    size_t i{1};
//
//    net.occupyLink();
//    P_old = net.largestClusterSize();
////    t = net.occupationProbability();
////    fout << t << '\t' << 0 << '\t' << c << endl;
//
//    while (net.occupyLink()){
////        cout << " ************* **************** *************" << endl;
//
////        t = net.relativeLinkDensity();
//        P = net.largestClusterSize();
//
//        dP = abs(P- P_old);
//
//        P_old = P;
//        c = net.isSelfClusterJump();
//
//        dP_list[i] = dP;
//        color_list[i] = c;
//        ++i;
//
////        fout << t << '\t' << P << '\t' << c << endl; // takes ~80% of the total time
////        net.viewClusterExtended();
////        net.viewCluster();
//    }
//
//    ofstream fout(filename);
//    stringstream ss;
//    ss   << "{\"signature\":" << "\"" << signature << "\""
//         << ",\"m\":" << m << ",\"network_size\":" << N
//         << ",\"M\":" << M  // in case of explosive percolation
//         << ",\"number_of_links\":" << net.linkCount()
//         << ",\"getNetworkSize\":" << net.nodeCount()
//         << ",\"cols\":" << "[\"t\", \"dP\", \"color\"]" << "}";
//
//    fout << "#" << ss.str() << endl;
//    fout << "#n = number of occupied links" << endl;
//    fout << "#t = n / N" << endl;
//    fout << "#t = relative occupation probability" << endl;
//    fout << "#cluster size = number of nodes in the cluster" << endl;
//    fout << "#P = order parameter = largest cluster size / N" << endl;
//    fout << "#N = network size = number of nodes in it" << endl;
//    fout << "#S_max = largest cluster size" << endl;
//    fout << "#dS = largest clusteter jump" << endl;
//    fout << "#c = 0 or 1 (different cluster jump or self cluster jump)"<< endl;
//
//    size_t sz = net.linkCount();
//    size_t Norm = net.nodeCount();
//    for(size_t i{}; i < sz; ++i){
//        if (dP_list[i] != 0) {
//            fout << (i+1)/double(Norm) << '\t' << dP_list[i] << '\t' << color_list[i] << endl;
//        }
//    }
//
//    fout.close();
//}


/**
 * average of 100 ensemble includeing colors
 * Date : 2019.05.22
 * @param argc
 * @param argv
 */
void BA_entropy_jump_ensemble(int argc, char **argv){
    if(argc < 5){
        cerr << "need more argument value" << endl;
        cout << "m = atoi(argv[1]);"<< endl;
        cout << "N = atoi(argv[2]);"<< endl;
        cout << "M = atoi(argv[3]);"<< endl;
        cout << "En = atoi(argv[4]);" << endl;
        exit(1);
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int En = atoi(argv[4]);
    NetworkBApercolationExplosive_v3 net(m, m, N, M);
    net.setRandomState(0, true);
//    NetworkBApercolation_v3 net(m, m, N);


//    double t;
//    double P{}, P_old{}, dP;
//    bool c{false};

    double entropy_jmp{0}, entropy_jmp_avg{}, tmp{};
//    double tmp_old{};
    double delta_H, previous_entropy;

//    size_t i{};
    for(int en{}; en < En; ++en) {
        cout << "iteration " << en+1 ;
//        auto t_start = chrono::_V2::system_clock::now();
        net.reset(en % 10 == 0); // reset network in every 10 interval

        net.occupyLink();

        entropy_jmp = 0;
        previous_entropy = net.maxEntropy();
        while (net.occupyLink()) {
//        cout << " ************* **************** *************" << endl;
            tmp = net.entropy_v2();
            delta_H = previous_entropy - tmp;
            previous_entropy = tmp; // be ready for next step

            if(abs(delta_H) > abs(entropy_jmp)){
                entropy_jmp = delta_H;
            }
        }
        entropy_jmp_avg += entropy_jmp;


//        auto t_end = chrono::_V2::system_clock::now();
//        chrono::duration<double> drtion = t_end - t_start;
//        cout << " : time elapsed " << drtion.count() << " sec";
        cout << endl;
    }

    string signature = net.get_signature();
    string filename = signature + "entropy-jump-" + currentTime() + ".txt";

    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << ",\"m\":" << m << ",\"network_size\":" << N
         << ",\"M\":" << M  // in case of explosive percolation
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"getNetworkSize\":" << net.nodeCount()
         << ",\"entropy_jump_avg\":" << entropy_jmp_avg/En
         << ",\"cols\":" << "[\"N\", \"m\", \"M\", \"jump\"]" << "}";

    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#<N><m><M><Avg Entropy Jump>" << endl;
    fout << net.nodeCount() << "\t" << m << "\t" << M << "\t" << entropy_jmp_avg/En << endl;

    fout.close();
}

void network_percolation(int argc, char* argv[]){

    int m = 2;
    int N = 10;
    int M = 2;

    if(argc > 3) {
        m = atoi(argv[1]);
        N = atoi(argv[2]);
        M = atoi(argv[3]);
    }


    NetworkBApercolation_v3 net(m, m, N);
//    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
//    net.reset(1);
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();

    size_t i{};
//    net.view_randomized_indices();
//    net.viewLinks();
    net.sizeSummary_in_MB();
    cout << "node count " << net.nodeCount() << endl;
    cout << "max entropy " << net.maxEntropy() << endl;
    cout << "current entropy " << net.entropy_v2() << endl;
    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;
//        cout << i << "-th link " << net.lastLink() << endl;
//        cout << "relativeLinkDensity, t = " << net.relativeLinkDensity() << endl;
//        cout << "largestClusterSize = " << net.largestClusterSize() << endl;
//        cout << "largestClusterID " << net.largestClusterID() << endl;
//        cout << "is self jump = " << net.isSelfClusterJump() << endl;
//        cout << "number of clusters " << net.clusterCount() << endl;
        ++i;
//        net.viewClusterExtended();
//        net.viewCluster();
//        net.view_randomized_indices();
        cout << net.entropy_v2() << endl;
//        break;
    }
    net.sizeSummary_in_MB();
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
//    net.reset();
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
//    net.time_summary();
//    cin.get(); // pausing program
}


//
//void  explosive_percolation_sum(uint m, uint network_size, uint M, uint ensemble_size, size_t th_id) {
//    NetworkBApercolationExplosive_v3 net(m * 2, m, network_size, M);
//
//    size_t sz = net.linkCount();
//    double node_count = net.nodeCount();
//    cout << sz << " " << node_count << endl;
//    vector<size_t> largest_cluster_size(sz);
//    vector<double> entropy1(sz), entropy2(sz);
//    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
//    for (size_t k{}; k < ensemble_size; ++k) {
//        auto t_start= chrono::_V2::system_clock::now();
//
//        net.reset();
////        _network_frame.viewNodes();
////        _network_frame.viewLinks();
////        _network_frame.viewClusterExtended();
//
//        size_t i{};
//        while (net.occupyLink()) {
////            cout << " ************* **************** *************" << endl;
////            cout << i << "-th link " << _network_frame.lastLink() << endl;
////            cout << "p = " << _network_frame.occupationProbability() << endl;
////            cout << "P = " << _network_frame.largestClusterSize() << endl;
//            largest_cluster_size[i] += net.largestClusterSize();
//            entropy1[i] += net.entropy_v1();
//            entropy2[i] += net.entropy_v2();
//            net.jump_v1();
////            cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
////            cout << entropy1[i] << " ==? " << entropy2[i] << endl;
//
//            ++i;
////            _network_frame.viewClusterExtended();
//        }
//        entropy_jump[k] = net.largestEntropyJump();
//        entropy_jump_pc[k] = net.largestEntropyJump_pc();
////        cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
//        auto t_end= chrono::_V2::system_clock::now();
//        chrono::duration<double> drtion = t_end - t_start;
//        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
//    }
//
//    string signature = net.get_signature();
//    string filename = signature + currentTime() + "_t" + to_string(th_id);
//    string filename_jump = signature + "_entropy_jump_" + currentTime() + "_t" + to_string(th_id);
//    ofstream fout(filename);
//    stringstream ss;
//    ss   << "{\"signature\":" << "\"" << signature << "\""
//         << ",\"m\":" << m << ",\"network_size\":" << network_size
//         << ",\"number_of_links\":" << net.linkCount()
//         << ",\"getNetworkSize\":" << net.nodeCount()
//         << ",\"M\":" << M << ",\"ensemble_size\":" << ensemble_size << "}";
//
//    fout << "#" << ss.str() << endl;
//    fout << "#n = number of occupied links" << endl;
//    fout << "#t = n / N" << endl;
//    fout << "#cluster size = number of nodes in the cluster" << endl;
//    fout << "#P = order parameter = largest cluster size / N" << endl;
//    fout << "#N = network size = number of nodes in it" << endl;
//    fout << "#S_max = largest cluster size" << endl;
//    fout << "#H1 = entropy slow method" << endl;
//    fout << "#H2 = entropy fast method" << endl;
//    fout << "#<n>\t<S_max>\t<H1>\t<H2>"<< endl;
//    for(size_t i{}; i < sz ; ++i){
//        fout << (i+1)
//             << '\t' << largest_cluster_size[i] / double(ensemble_size)
//             << '\t' << entropy1[i] / double(ensemble_size)
//             << '\t' << entropy2[i] / double(ensemble_size)
//             << endl;
//    }
//
//    fout.close();
//
//    ofstream fout_jump(filename_jump);
//    fout_jump << ss.str() << endl;
//    fout_jump << "#<largest entropy jump>\t<p>" << endl;
//    for(size_t k{}; k < ensemble_size ; ++k){
//        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
//    }
//    fout_jump.close();
//}


//void network_percolation_explosive(int argc, char* argv[]) {
//    uint m = atoi(argv[1]);
//    uint network_size = atoi(argv[2]);
//    uint M = atoi(argv[3]);
//    uint ensemble_size = atoi(argv[4]);
//    cout << "IF ensemble_size == 100 " << " four network each with 25 iteration" << endl;
//    cout << "m " << m << endl;
//    cout << "N " << network_size << endl;
//    cout << "M " << M << endl;
//    cout << "ensemble size " << ensemble_size << endl;
//    int number_of_thread = std::thread::hardware_concurrency();
//    vector<thread> ths(number_of_thread);
//    for(size_t i{}; i < number_of_thread ; ++i) {
//        ths[i] = thread(
//                explosive_percolation_sum,
//                m,
//                network_size,
//                M,
//                ensemble_size / number_of_thread,
//                i
//        );
//        cout << "thread " << ths[i].get_id() << " started" << endl;
//    }
//    for(size_t i{}; i < number_of_thread ; ++i) {
//        if(ths[i].joinable()){
//            cout << "thread " << ths[i].get_id() << " finished" << endl;
//            ths[i].join();
//
//        }
//    }
//
//
//}

/**
 *
 * @param argc
 * @param argv
 */
void network_percolation_explosive_v2(int argc, char** argv){
    if(argc < 5){
        cerr << "need more argument value" << endl;
        cout << "m = atoi(argv[1]);"<< endl;
        cout << "N = atoi(argv[2]);"<< endl;
        cout << "M = atoi(argv[3]);"<< endl;
        cout << "En = atoi(argv[4]);" << endl;
        exit(1);
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int En = atoi(argv[4]);
    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    cout << net.nodeCount() << endl;
//    cout << net.getNetworkSize() << endl;
    net.setRandomState(0, true);
//    NetworkBApercolation_v3 net(m, m, N);


//    double t;
//    double P{}, P_old{}, dP;
//    bool c{false};

    size_t linkCount = net.linkCount();
    vector<double> entropy(linkCount), order_parameter(linkCount);
    double entropy_jmp{0}, entropy_jmp_avg{}, H{};
//    double tmp_old{};
    double delta_H, previous_entropy;

    size_t k{};
    for(int en{1}; en <= En; ++en) {
        cout << "iteration " << en ;
        auto t_start = chrono::system_clock::now();
        net.reset(en % 25 == 0); // reset network in every 20 interval

        k = 0;
        net.occupyLink();
        entropy[k] += net.entropy_v2();
        order_parameter[k] += net.largestClusterSize();
        ++k;
        entropy_jmp = 0;
        previous_entropy = net.maxEntropy();
        while (net.occupyLink()) {
//        cout << " ************* **************** *************" << endl;
            H = net.entropy_v2();
            entropy[k] += H;
            order_parameter[k] += net.largestClusterSize();
            delta_H = previous_entropy - H;
            previous_entropy = H; // be ready for next step

            if(abs(delta_H) > abs(entropy_jmp)){
                entropy_jmp = delta_H;
            }

            ++k;
        }
        entropy_jmp_avg += entropy_jmp;


        auto t_end = chrono::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << " : time elapsed " << drtion.count() << " sec";
        cout << endl;
    }

    string signature = net.get_signature();
    string filename_jump = signature + "entropy-jump-" + currentTime() + ".txt";
    string filename = signature + "entropy-order_param-" + currentTime() + ".txt";

    ofstream fout_jump(filename_jump);
    ofstream fout(filename);
    stringstream ss;
    ss << "{\"signature\":" << "\"" << signature << "\""
       << ",\"m\":" << m << ",\"network_size\":" << N
       << ",\"M\":" << M  // in case of explosive percolation
         << ",\"linkCount\":" << linkCount
       << ",\"random_state\":" << net.getRandomState()
       << ",\"nodeCount\":" << net.nodeCount();


    fout_jump << "#" << ss.str() << ",\"entropy_jump_avg\":" << entropy_jmp_avg/En
              << ",\"cols\":" << "[\"N\", \"m\", \"M\", \"jump\"]" << "}" << endl;
    fout_jump << "#n = number of occupied links" << endl;
    fout_jump << "#t = n / N" << endl;
    fout_jump << "#cluster size = number of nodes in the cluster" << endl;
    fout_jump << net.nodeCount() << "\t" << m << "\t" << M << "\t" << entropy_jmp_avg/En << endl;
    fout_jump.close();


    fout << "#" << ss.str() << ",\"cols\":" << "[\"t\", \"H\", \"P\"]" << "}" << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    for(size_t i{};i < entropy.size(); ++i){
        fout << (i+1)/double(N) << "\t" << entropy[i]/En << "\t" << order_parameter[i]/(En*N) << endl;
    }
    fout.close();
}
/*
 * m0=3
 * m=1
 * N=100000
 * BA took 0.22 sec
 * MDA took 2.42 sec  ???
 *
 */
void network_percolation_global(){
//    NetworkPercolation<NetworkMDA> networkPercolation(10, 1, 1000000);
//    cout << networkPercolation.get_signature() << endl;
////    networkPercolation.viewNodes();
////    networkPercolation.viewLinks();
////    networkPercolation.viewCluster();
////    networkPercolation.viewClusterExtended();
//
//    int i=0;
//    while(networkPercolation.occupyLink()){
////        cout << "iteration " << ++i << "****************************" << endl;
////        cout << "last link " << networkPercolation.lastLink() << endl;
////        networkPercolation.viewNodes();
////        networkPercolation.viewClusterExtended();
//    }
//    cout << "time " << networkPercolation.time() << " sec" << endl;

}

void network_percolationReverse_global(){
//    NetworkPercolationReverse<NetworkBA> networkPercolation(3, 1, 10);
//    cout << networkPercolation.get_signature() << endl;
//    networkPercolation.viewNodes();
//    networkPercolation.viewLinks();
//    networkPercolation.viewCluster();
////    networkPercolation.viewClusterExtended();
//
//    int i=0;
//    while(networkPercolation.occupyLink()){
////        cout << "iteration " << ++i << "****************************" << endl;
////        cout << "last link " << networkPercolation.lastLink() << endl;
////        networkPercolation.viewNodes();
////        networkPercolation.viewClusterExtended();
//    }
//    cout << "time " << networkPercolation.time() << " sec" << endl;

}

//
//void test_NetworkBApercolationExplosive_v3(int argc, char **argv){
//    if(argc < 5 ){
//        cout << "argv[1] == m" << endl;
//        cout << "argv[2] == N" << endl;
//        cout << "argv[3] == M" << endl;
//        cout << "argv[4] == Ensemble" << endl;
//        return;
//    }
//    int m = atoi(argv[1]);
//    int N = atoi(argv[2]);
//    int M = atoi(argv[3]);
//    int ensemble_size= atoi(argv[4]);
//    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    net.setRandomState(0, false);
//    net.initialize_network();
//
//
//    size_t sz = net.linkCount();
//    double node_count = net.nodeCount();
//    cout << sz << " " << node_count << endl;
//    vector<size_t> largest_cluster_size(sz);
//    vector<double> entropy1(sz), entropy2(sz);
//    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
//    for (size_t k{}; k < ensemble_size; ++k) {
//        auto t_start= chrono::_V2::system_clock::now();
//
//        net.reset();
////        _network_frame.viewNodes();
////        _network_frame.viewLinks();
////        _network_frame.viewClusterExtended();
//
//        size_t i{};
//        while (net.occupyLink()) {
////            cout << " ************* **************** *************" << endl;
////            cout << i << "-th link " << _network_frame.lastLink() << endl;
////            cout << "p = " << _network_frame.occupationProbability() << endl;
////            cout << "P = " << _network_frame.largestClusterSize() << endl;
//            largest_cluster_size[i] += net.largestClusterSize();
//            entropy1[i] += net.entropy_v1();
//            entropy2[i] += net.entropy_v2();
//            net.jump_v1();
////            cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
////            cout << entropy1[i] << " ==? " << entropy2[i] << endl;
//
//            ++i;
////            _network_frame.viewClusterExtended();
//        }
//        entropy_jump[k] = net.largestEntropyJump();
//        entropy_jump_pc[k] = net.jump_tc();
////        cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
//        auto t_end= chrono::_V2::system_clock::now();
//        chrono::duration<double> drtion = t_end - t_start;
//        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
//    }
//
//    string signature = net.get_signature();
//    string filename = signature + currentTime();
//    string filename_jump = signature + "_entropy_jump_" + currentTime();
//    ofstream fout(filename);
//    stringstream ss;
//    ss   << "{\"signature\":" << "\"" << signature << "\""
//         << ",\"m\":" << m << ",\"network_size\":" << N
//         << ",\"number_of_links\":" << net.linkCount()
//         << ",\"number_of_nodes\":" << net.nodeCount()
//         << ",\"M\":" << M << ",\"ensemble_size\":" << ensemble_size << "}";
//
//    fout << "#" << ss.str() << endl;
//    fout << "#n = number of occupied links" << endl;
//    fout << "#t = n / N" << endl;
//    fout << "#cluster size = number of nodes in the cluster" << endl;
//    fout << "#P = order parameter = largest cluster size / N" << endl;
//    fout << "#N = network size = number of nodes in it" << endl;
//    fout << "#S_max = largest cluster size" << endl;
//    fout << "#H1 = entropy slow method" << endl;
//    fout << "#H2 = entropy fast method" << endl;
//    fout << "#<n>\t<S_max>\t<H1>\t<H2>"<< endl;
//    for(size_t i{}; i < sz ; ++i){
//        fout << (i+1)
//             << '\t' << largest_cluster_size[i] / double(ensemble_size)
//             << '\t' << entropy1[i] / double(ensemble_size)
//             << '\t' << entropy2[i] / double(ensemble_size)
//             << endl;
//    }
//
//    fout.close();
//
//    ofstream fout_jump(filename_jump);
//    fout_jump << ss.str() << endl;
//    fout_jump << "#<largest entropy jump>\t<p>" << endl;
//    for(size_t k{}; k < ensemble_size ; ++k){
//        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
//    }
//    fout_jump.close();
//
//}


//
//void test_NetworkBApercolationExplosive_v3_jump(int argc, char **argv){
//    if(argc < 5 ){
//        cout << "argv[1] == m" << endl;
//        cout << "argv[2] == N" << endl;
//        cout << "argv[3] == M" << endl;
//        cout << "argv[4] == Ensemble" << endl;
//        return;
//    }
//    int m = atoi(argv[1]);
//    int N = atoi(argv[2]);
//    int M = atoi(argv[3]);
//    int ensemble_size = atoi(argv[4]);
//    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    net.setRandomState(0, true);
//    net.initializeNetwork();
//
//    cout << net.nodeCount()  << ", " << net.linkCount() << ", " << endl;
//    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
//    for (size_t k{}; k < ensemble_size; ++k) {
//        auto t_start= chrono::_V2::system_clock::now();
//
//        net.reset();
////        _network_frame.viewNodes();
////        _network_frame.viewLinks();
////        _network_frame.viewClusterExtended();
//
//        size_t i{};
//        while (net.occupyLink()) {
//            net.entropy_v2();
//            net.jump_v1();
//            ++i;
////            _network_frame.viewClusterExtended();
//        }
//        entropy_jump[k] = net.largestEntropyJump();
//        entropy_jump_pc[k] = net.largestEntropyJump_pc();
////        cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
//        auto t_end= chrono::_V2::system_clock::now();
//        chrono::duration<double> drtion = t_end - t_start;
//        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
//    }
//
//    auto tm = currentTime();
//    string signature = net.get_signature();
//    string filename_jump = signature + "_entropy_jump_" + tm;
//    stringstream ss;
//    ss << "{"
//         << R"*("signature":")*" << signature << "\""
//         << R"*(,"m":)*" << m
//         << R"*(,"network_size":)*" << N
//         << R"*(,"number_of_links":)*" << net.linkCount()
//         << R"*(,"number_of_nodes":)*" << net.nodeCount()
//         << R"*(,"M":)*" << M
//         << R"*(,"ensemble_size":)*" << ensemble_size
//         << R"*(,"date":")*" << tm << "\""
//         << "}";
//
//    ofstream fout_jump(filename_jump);
//    fout_jump << '#' << ss.str() << endl;
//    fout_jump << "#<largest entropy jump>\t<p>" << endl;
//    for(size_t k{}; k < ensemble_size ; ++k){
//        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
//    }
//    fout_jump.close();
//
//}

// 2019.10.02
void test_NetworkBA_v2(int argc, char **argv){
    if(argc < 3 ){
        cout << "argv[1] == m" << endl;
        cout << "argv[2] == N" << endl;
        return;
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    NetworkBA_v2 net(m, m);
    net.setRandomState(0, false);
    net.grow(N);
    cout << net.getLinkCount() << endl;
    cout << net.getNodeCount() << endl;
    net.view();
}

void test_v5(int argc, char **argv) {
    if(argc < 5 ){
        cout << "argv[1] == m" << endl;
        cout << "argv[2] == N" << endl;
        cout << "argv[3] == M" << endl;
        cout << "argv[4] == Ensemble" << endl;
        return;
    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int ensemble_size = atoi(argv[4]);
    cout << "m=" << m << ",N="<< N << ",M=" << M << ",En="<<ensemble_size << endl;
    if (N <= 2*m) cerr << "Why ?? : line " << __LINE__ << endl;

//    NetworkBApercolation_v5 net(m, m, N);
    NetworkBApercolationExplosive_v5 net(m, m, N, M);
//    NetworkBApercolationExplosive_Inverted_v5 net(m,m,N,M);

    net.setRandomState(0, false);
    net.initializeNetwork();

    size_t linkCount = net.linkCount();
    size_t nodeCount = net.nodeCount();
//    cout << nodeCount << ", " << linkCount << ", " << endl;
//    net.viewNetwork();
//    net.viewListOfLinkIndices();
    vector<double> entropy_jump(ensemble_size), order_jump(ensemble_size);
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    bool flag=true;

    for (int k{0}; k < ensemble_size; ++k) {
        auto t_start= chrono::system_clock::now();
//        net.viewListOfLinkIndices();
        net.reset(k%25 == 0); // every 25 step. reset the network



        size_t i{};
        net.viewClusters();
        net.viewListOfLinkIndices();
        net.viewNetwork();
//        cout << "entering to while" << endl;
        while (true) {
            flag = net.occupyLink();
            if (!flag) break;
//            cout << "i " << i  << endl;
            entropy[i] += net.entropy();
            order_param[i] += net.largestClusterSize();

            net.viewClusters();
            net.viewListOfLinkIndices();
//            cout << net.entropy_v1()  << "\t";
//            cout << net.entropy_v2() << endl;
//            cout << net.largestClusterSize() << endl;
            net.jump();
//            _network_frame.viewClusterExtended();
            ++i;
//            if (i == 9) break;
        }
//        net.summary();
        entropy_jump[k] = net.largestEntropyJump();
        order_jump[k] = net.largestOrderJump();
//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k
         << " : time elapsed " << drtion.count() << " sec"
         << endl;
    }

    auto tm = currentTime();
    string signature = net.get_signature();
    string filename_jump = signature + "_largest_jump_" + tm + ".txt";
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"class":")*" << net.getClassName() << "\""
       << R"*(,"m":)*" << m
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << net.linkCount()
       << R"*(,"number_of_nodes":)*" << net.nodeCount()
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << ensemble_size
       << R"*(,"date":")*" << tm << "\""
       << "}";

//    ofstream fout_jump(filename_jump);
//    fout_jump << '#' << ss.str() << endl;
//    fout_jump << "#<largest entropy jump>\t<largest order parameter jump>" << endl;
//    for(size_t k{}; k < ensemble_size ; ++k){
//        fout_jump << abs(entropy_jump[k]) << '\t' << order_jump[k] << endl;
//    }
//    fout_jump.close();


    string filename = signature + "_entropy-order_" + tm + ".txt";
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
    for(size_t k{}; k < entropy.size() ; ++k){
        fout << (k+1)/double(N)
             << "\t" << entropy[k]/ensemble_size
             << "\t" << order_param[k]/(ensemble_size*double(N)) << endl;
    }
    fout.close();

}

void test_v6(int argc, char **argv) {
//    if(argc < 5 ){
//        cout << "argv[1] == m" << endl;
//        cout << "argv[2] == N" << endl;
//        cout << "argv[3] == M" << endl;
//        cout << "argv[4] == Ensemble" << endl;
//        return;
//    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int ensemble_size = atoi(argv[4]);
    cout << "m=" << m << ",N="<< N << ",M=" << M << ",En="<<ensemble_size << endl;
    if (N <= 2*m) cerr << "Why ?? : line " << __LINE__ << endl;

//    NetworkBApercolation_v6 net(m, m, N);
    NetworkBApercolationExplosive_v6 net(m, m, N, M); // TODO M=2 is problematic
//    NetworkBApercolationExplosive_Inverted_v5 net(m,m,N,M);

    net.setRandomState(0, false);
    net.initializeNetwork();

    size_t linkCount = net.getLinkCount();
    size_t nodeCount = net.getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    net.viewNetwork();
//    net.viewListOfLinkIndices();
    vector<double> entropy_jump(ensemble_size), order_jump(ensemble_size);
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    bool flag=true;
    double H1,H2;
    for (int k{0}; k < ensemble_size; ++k) {
        auto t_start= chrono::system_clock::now();
//        net.viewListOfLinkIndices();
        net.reset(k%25 == 0); // every 25 step. reset the network

        size_t i{};
        net.viewNetwork();
        net.viewClusters();
        net.viewListOfLinkIndices();
//        cout << "entering to while" << endl;
        while (true) {
            flag = net.occupyLink();
            if (!flag) break;
//            cout << "********************* i " << i  << endl;
            entropy[i] += net.entropy();
            order_param[i] += net.largestClusterSize();

//            net.viewClusters();
//            net.viewListOfLinkIndices();
            H1 = net.entropy_v1();
            H2 = net.entropy_v2();
//            cout << H1  << "\t";
//            cout << H2 << "\t";
//            cout << net.largestClusterSize() ;
//            cout << endl;
            if(abs(H1 - H2) > 1e-10){
                cerr << "entropy calculation is problematic"  << endl;
            }
            net.jump();
//            _network_frame.viewClusterExtended();
            ++i;
//            if (i == 9) break;
        }
//        net.summary();
        entropy_jump[k] = net.largestEntropyJump();
        order_jump[k] = net.largestOrderJump();
//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k
             << " : time elapsed " << drtion.count() << " sec"
             << endl;
    }

//    net.viewClusters();
//    net.viewListOfLinkIndices();

    auto tm = currentTime();
    string signature = net.get_signature();
    string filename_jump = signature + "_largest_jump_" + tm + ".txt";
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"class":")*" << net.getClassName() << "\""
       << R"*(,"m":)*" << m
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << net.getLinkCount()
       << R"*(,"number_of_nodes":)*" << net.getNodeCount()
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << ensemble_size
       << R"*(,"date":")*" << tm << "\""
       << "}";

//    ofstream fout_jump(filename_jump);
//    fout_jump << '#' << ss.str() << endl;
//    fout_jump << "#<largest entropy jump>\t<largest order parameter jump>" << endl;
//    for(size_t k{}; k < ensemble_size ; ++k){
//        fout_jump << abs(entropy_jump[k]) << '\t' << order_jump[k] << endl;
//    }
//    fout_jump.close();


    string filename = signature + "_entropy-order_" + tm + ".txt";
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
    for(size_t k{}; k < entropy.size() ; ++k){
        fout << (k+1)/double(N)
             << "\t" << entropy[k]/ensemble_size
             << "\t" << order_param[k]/(ensemble_size*double(N)) << endl;
    }
    fout.close();

}

void test_network_v6(int argc, char **argv) {
//    if(argc < 5 ){
//        cout << "argv[1] == m" << endl;
//        cout << "argv[2] == N" << endl;
//        cout << "argv[3] == M" << endl;
//        cout << "argv[4] == Ensemble" << endl;
//        return;
//    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);
    int ensemble_size = atoi(argv[4]);
    cout << "m=" << m << ",N="<< N << ",M=" << M << ",En="<<ensemble_size << endl;
    if (N <= 2*m) cerr << "Why ?? : line " << __LINE__ << endl;

    NetworkBApercolation_v6 net(m, m, N);
//    NetworkBApercolationExplosive_v6 net(m, m, N, M);
//    NetworkBApercolationExplosive_Inverted_v5 net(m,m,N,M);

    net.setRandomState(0, true);
    net.initializeNetwork();

    size_t linkCount = net.getLinkCount();
    size_t nodeCount = net.getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;

    net.viewNetwork();
    net.viewListOfLinkIndices();


//    net.reset(1);
//    net.viewClusters();
//    net.viewListOfLinkIndices();
//    net.viewNetwork();

}

void test_v3(int argc, char **argv) {
//    if(argc < 5 ){
//        cout << "argv[1] == m" << endl;
//        cout << "argv[2] == N" << endl;
//        cout << "argv[3] == M" << endl;
//        cout << "argv[4] == Ensemble" << endl;
//        return;
//    }
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

    int ensemble_size = atoi(argv[4]);
    cout << "m=" << m << ",N="<< N << ",M=" << M << ",En="<<ensemble_size << endl;

//    NetworkBApercolation_v3 net(m, m, N);
     NetworkBApercolationExplosive_v3 net(m, m, N, M);
//   NetworkBApercolationExplosive_Inverted_v3 net(m, m, N, M);
    net.setRandomState(0, true);
    net.initializeNetwork();

    size_t linkCount = net.linkCount();
    size_t nodeCount = net.nodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    net.viewNetwork();
//    net.viewListOfLinkIndices();
    vector<double> entropy_jump(ensemble_size), order_jump(ensemble_size);
//    double jump_tc{};
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    for (int k{0}; k < ensemble_size; ++k) {
        auto t_start= chrono::system_clock::now();
//        net.viewListOfLinkIndices();
        net.reset(k%25 == 0); // every 25 step. reset the network

        size_t i{};
//        net.viewClusters();
//        net.viewListOfLinkIndices();
//        net.viewNetwork();
//        double t{};
//        cout << "entering to while" << endl;
        while (net.occupyLink()) {
//            cout << "i " << i  << endl;
//            t = net.relativeLinkDensity();
            entropy[i] += net.entropy();
            order_param[i] += net.largestClusterSize();
//            if(t > 0.733484){
//                net.viewCluster();
//            }
//            net.viewClusters();
//            cout << net.entropy_v1()  << "\t";
//            cout << net.entropy_v2() << endl;
//            cout << net.largestClusterSize() << endl;
            net.jump();
//            _network_frame.viewClusterExtended();
            ++i;
//            if (i == 7) break;
        }
        entropy_jump[k] = net.largestEntropyJump();
        order_jump[k] = net.largestOrderJump();
//        cout << "jump tc " << net.jump_tc() << endl;
//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    auto tm = currentTime();
    string signature = net.get_signature();
    string filename_jump = signature + "_largest_jump_" + tm + ".txt";
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"class":")*" << net.getClassName() << "\""
       << R"*(,"m":)*" << m
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << net.linkCount()
       << R"*(,"number_of_nodes":)*" << net.nodeCount()
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << ensemble_size
       << R"*(,"date":")*" << tm << "\""
       << "}";

    ofstream fout_jump(filename_jump);
    fout_jump << '#' << ss.str() << endl;
    fout_jump << "#<largest entropy jump>\t<p>" << endl;
    for(int k{}; k < ensemble_size ; ++k){
        fout_jump << entropy_jump[k] << '\t' << order_jump[k] << endl;
    }
    fout_jump.close();


    string filename = signature + "_entropy-order_" + tm + ".txt";
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
    for(size_t k{}; k < entropy.size() ; ++k){
        fout << (k+1)/double(N)
             << "\t" << entropy[k]/ensemble_size
             << "\t" << order_param[k]/(ensemble_size*double(N)) << endl;
    }
    fout.close();

}

//void clusterSizeDistribution(int argc, char **argv) {
////    size_t m0 = size_t(atoi(argv[1]));
//    if (argc < 4) {
//        cout << "argv[1] = m" << endl;
//        cout << "argv[2] = NetworkSize" << endl;
//        cout << "argv[3] = EnsembleSize" << endl;
//        return;
//    }
//
//    size_t m = size_t(atoi(argv[1]));
//    size_t N = size_t(atoi(argv[2]));
//    size_t En = size_t(atoi(argv[3]));
//
//    cout << "network size " << N << endl;
//    cout << "ensemble size " << En << endl;
//
//    NetworkBApercolation_v3 net(m*2, m, N);
//    net.setRandomState(0, true);
//
//    vector<double> csd(10);
//    double t;
//    for (size_t en{}; en < En; ++en) {
//        auto t0 = std::chrono::system_clock::now();
//        net.reset(en%50 == 0);
//
//        while(net.occupyLink()){
//            t = net.relativeLinkDensity();
//            if (t  > 0.5){
//                auto tmp = net.clusterSizeDistribution();
//                if(csd.size() < tmp.size()){
//                    csd.resize(tmp.size());
//                }
//                for(size_t i{}; i < tmp.size(); ++i){
//                    csd[i] += tmp[i];
//                }
//                break;
//            }
//        }
//
//
////        cout << "line " << __LINE__ << endl;
//        auto t1 = std::chrono::system_clock::now();
//        std::chrono::duration<double> drtion = t1 - t0;
//        cout << "Iteration " << en << " : time " << drtion.count() << " sec" << endl;
//    }
//
//    string tm = currentTime();
//    string filename = net.get_signature() + "N_" + to_string(N)
//                      + "-cluster-size-distribution-" + tm + ".txt";
//    stringstream ss;
//    ss   << "{"
//         << R"("signature":")" << net.get_signature() << "\""
//         << R"(,"m":)" << m
//         << R"(,"N":)" << net.getNetworkSize()
//         << R"(,"number_of_links":)" << net.linkCount()
//         << R"(,"number_of_nodes":)" << net.nodeCount()
////         << R"(,"M":)" << M
//         << R"(,"ensemble_size":)" << En
//         << R"(,"random_state":)" << net.getRandomState()
//         << R"(,"time":")" << tm << "\""
//         << "}";
//
//
//    ofstream fout(filename);
//    fout << "#" << ss.str() << endl;
//    fout << "#{\"m0\":" << net.get_m0()
//         << ",\"m\":" << net.get_m()
//         << ",\"N\":" << N
//         << ",\"En\":" << En
//         << "}" << endl;
//
//    for (size_t i{0}; i < csd.size(); ++i) {
//        if(csd[i] == 0) continue;
//        fout << i << "\t" << csd[i]/(En * net.getNetworkSize()) << endl;
//    }
//
//    // normalization
//    fout.close();
//}
