//
// Created by shahnoor on 12/19/18.
//

#include "test_network_percolation.h"
#include "../nets/network.h"
#include "../util/time_tracking.h"
#include "../percolation/network_percolation.h"
#include "../percolation/network_percolation_reverse.h"
#include "../percolation/network_percolation_template.h"
//#include "../percolation/network_percolation2.h"
#include <chrono>
#include <fstream>
#include <thread>

using namespace std;

/**
 * Date : 2019.05.17
 * @param argc
 * @param argv
 */
void BA_self_jump(int argc, char* argv[]){
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    NetworkBApercolation_v3 net(m, m, N);
    string signature = net.get_signature();
    string filename = signature + "order_parameter-" + currentTime() + ".txt";

    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << ",\"m\":" << m << ",\"network_size\":" << N
         << ",\"M\":" << M  // in case of explosive percolation
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"number_of_nodes\":" << net.nodeCount()
         << ",\"cols\":" << "[\"t\", \"P\", \"color\"]" << "}";

    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#P = order parameter = largest cluster size / N" << endl;
    fout << "#N = network size = number of nodes in it" << endl;
    fout << "#S_max = largest cluster size" << endl;
    fout << "#dS = largest clusteter jump" << endl;
    fout << "#c = 0 or 1 (different cluster jump or self cluster jump)"<< endl;

    double t;
    double P{}, P_old{}, dP;
    bool c{false};


//    net.occupyLink();
//    P_old = net.largestClusterSize();
//    t = net.occupationProbability();
//    fout << t << '\t' << 0 << '\t' << c << endl;

    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;

        t = net.occupationProbability();
        P = net.largestClusterSize();

//        dP = abs(P- P_old);

//        P_old = P;
        c = net.isSelfClusterJump();

        fout << t << '\t' << P << '\t' << c << endl; // takes ~80% of the total time
//        net.viewClusterExtended();
//        net.viewCluster();
    }
    fout.close();
}

void network_percolation(int argc, char* argv[]){

    size_t m = 3;
    size_t N = 10;
    size_t M = 2;
    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    NetworkBApercolation_v3 net(m, m, N);
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
//    net.reset(1);
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();

    size_t i{};
    net.view_randomized_indices();
    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;
//        cout << i << "-th link " << net.lastLink() << endl;
//        cout << "occupationProbability = " << net.occupationProbability() << endl;
//        cout << "largestClusterSize = " << net.largestClusterSize() << endl;
//        cout << "is self jump = " << net.isSelfClusterJump() << endl;
//        cout << "number of clusters " << net.clusterCount() << endl;
//        ++i;
//        net.viewClusterExtended();
//        net.viewCluster();
        net.view_randomized_indices();
    }
//    net.viewNodes();
//    net.viewLinks();
    net.viewClusterExtended();
//    net.reset();
//    net.viewNodes();
//    net.viewLinks();
//    net.viewClusterExtended();
    net.time_summary();
}



void  explosive_percolation_sum(uint m, uint network_size, uint M, uint ensemble_size, size_t th_id) {
    NetworkBApercolationExplosiveSum net(m + 1, m, network_size, M);

    size_t sz = net.linkCount();
    double node_count = net.nodeCount();
    cout << sz << " " << node_count << endl;
    vector<size_t> largest_cluster_size(sz);
    vector<double> entropy1(sz), entropy2(sz);
    vector<double> entropy_jump(ensemble_size), entropy_jump_pc(ensemble_size);
    for (size_t k{}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();

        net.reset();
//        _network_frame.viewNodes();
//        _network_frame.viewLinks();
//        _network_frame.viewClusterExtended();

        size_t i{};
        while (net.occupyLink()) {
//            cout << " ************* **************** *************" << endl;
//            cout << i << "-th link " << _network_frame.lastLink() << endl;
//            cout << "p = " << _network_frame.occupationProbability() << endl;
//            cout << "P = " << _network_frame.largestClusterSize() << endl;
            largest_cluster_size[i] += net.largestClusterSize();
            entropy1[i] += net.entropy_v1();
            entropy2[i] += net.entropy_v2();
            net.jump();
//            cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
//            cout << entropy1[i] << " ==? " << entropy2[i] << endl;

            ++i;
//            _network_frame.viewClusterExtended();
        }
        entropy_jump[k] = net.largestEntropyJump();
        entropy_jump_pc[k] = net.largestEntropyJump_pc();
//        cout << _network_frame.largestEntropyJump() << " at " << _network_frame.largestEntropyJump_pc() << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    string signature = net.get_signature();
    string filename = signature + currentTime() + "_t" + to_string(th_id);
    string filename_jump = signature + "_entropy_jump_" + currentTime() + "_t" + to_string(th_id);
    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << ",\"m\":" << m << ",\"network_size\":" << network_size
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"number_of_nodes\":" << net.nodeCount()
         << ",\"M\":" << M << ",\"ensemble_size\":" << ensemble_size << "}";

    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#P = order parameter = largest cluster size / N" << endl;
    fout << "#N = network size = number of nodes in it" << endl;
    fout << "#S_max = largest cluster size" << endl;
    fout << "#H1 = entropy slow method" << endl;
    fout << "#H2 = entropy fast method" << endl;
    fout << "#<n>\t<S_max>\t<H1>\t<H2>"<< endl;
    for(size_t i{}; i < sz ; ++i){
        fout << (i+1)
             << '\t' << largest_cluster_size[i] / double(ensemble_size)
             << '\t' << entropy1[i] / double(ensemble_size)
             << '\t' << entropy2[i] / double(ensemble_size)
             << endl;
    }

    fout.close();

    ofstream fout_jump(filename_jump);
    fout_jump << ss.str() << endl;
    fout_jump << "#<largest entropy jump>\t<p>" << endl;
    for(size_t k{}; k < ensemble_size ; ++k){
        fout_jump << entropy_jump[k] << '\t' << entropy_jump_pc[k] << endl;
    }
    fout_jump.close();
}


void network_percolation_explosive(int argc, char* argv[]) {
    uint m = atoi(argv[1]);
    uint network_size = atoi(argv[2]);
    uint M = atoi(argv[3]);
    uint ensemble_size = atoi(argv[4]);
    cout << "IF ensemble_size == 100 " << " four network each with 25 iteration" << endl;
    cout << "m " << m << endl;
    cout << "N " << network_size << endl;
    cout << "M " << M << endl;
    cout << "ensemble size " << ensemble_size << endl;
    int number_of_thread = std::thread::hardware_concurrency();
    vector<thread> ths(number_of_thread);
    for(size_t i{}; i < number_of_thread ; ++i) {
        ths[i] = thread(
                explosive_percolation_sum,
                m,
                network_size,
                M,
                ensemble_size / number_of_thread,
                i
        );
        cout << "thread " << ths[i].get_id() << " started" << endl;
    }
    for(size_t i{}; i < number_of_thread ; ++i) {
        if(ths[i].joinable()){
            cout << "thread " << ths[i].get_id() << " finished" << endl;
            ths[i].join();

        }
    }


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
    NetworkPercolation<NetworkMDA> networkPercolation(10, 1, 1000000);
    cout << networkPercolation.get_signature() << endl;
//    networkPercolation.viewNodes();
//    networkPercolation.viewLinks();
//    networkPercolation.viewCluster();
//    networkPercolation.viewClusterExtended();

    int i=0;
    while(networkPercolation.occupyLink()){
//        cout << "iteration " << ++i << "****************************" << endl;
//        cout << "last link " << networkPercolation.lastLink() << endl;
//        networkPercolation.viewNodes();
//        networkPercolation.viewClusterExtended();
    }
    cout << "time " << networkPercolation.time() << " sec" << endl;

}

void network_percolationReverse_global(){
    NetworkPercolationReverse<NetworkBA> networkPercolation(3, 1, 10);
    cout << networkPercolation.get_signature() << endl;
    networkPercolation.viewNodes();
    networkPercolation.viewLinks();
    networkPercolation.viewCluster();
//    networkPercolation.viewClusterExtended();

    int i=0;
    while(networkPercolation.occupyLink()){
//        cout << "iteration " << ++i << "****************************" << endl;
//        cout << "last link " << networkPercolation.lastLink() << endl;
//        networkPercolation.viewNodes();
//        networkPercolation.viewClusterExtended();
    }
    cout << "time " << networkPercolation.time() << " sec" << endl;

}