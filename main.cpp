#include <iostream>
#include <chrono>
#include "src/util/time_tracking.h"
#include "src/nets/network.h"
#include <algorithm>
#include <fstream>
#include <thread>


using namespace std;


void degree_distribution_ba(int argc, char* argv[]){
//    size_t m0 = size_t(atoi(argv[1]));
    if(argc < 1){
        cout << "supply argument " << endl;
        return;
    }

    size_t m = size_t(atoi(argv[1]));
    size_t network_size = size_t(atoi(argv[2]));
    size_t ensemble_size = size_t(atoi(argv[3]));

    cout << "network size " << network_size << endl;
    cout << "ensemble size " << ensemble_size << endl;

    NetworkBA net(m, m);
    cout << "m0 " << net.get_m0() << ", m " << net.get_m() << endl;

    string filename =  net.get_signature() + "size_" + to_string(network_size) + "-degree" + currentTime() + ".txt";
    ofstream fout(filename);
    fout << "{\"m0\":" << net.get_m0()
         << ",\"m\""<< net.get_m()
         << ",\"network_size\":" << network_size
         << ",\"ensemble_size\":" << ensemble_size
         << "}" << endl;

    for(size_t en{}; en < ensemble_size; ++en) {
        auto t0= std::chrono::system_clock::now();
        net.reset();

        for (size_t i{}; i < network_size; ++i) {
            net.add_node();
        }
//        net.view_nodes();

//        cout << "line " << __LINE__ << endl;

        vector<double> degs = net.degrees();
        std::sort(degs.begin(), degs.end());

//        cout << degs.size() << " and  " << degree.size() << endl;
        for (size_t i{}; i < degs.size(); ++i) {
            fout << degs[i] <<',';
        }
        fout << endl;
//        cout << "line " << __LINE__ << endl;
        auto t1= std::chrono::system_clock::now();
        std::chrono::duration<double> drtion = t1 - t0;
        cout << "Iteration " << en << " : time " << drtion.count() << " sec" << endl;
    }

    // normalization

    fout.close();

}

void run_in_main(int argc, char* argv[]){
    NetworkBA net(3, 50);

    net.view_nodes();
    net.view_links();
    net.view_preferentially();

    size_t network_size = 1000000;
    for(size_t i{}; i < network_size; ++i) {
//        cout << "total degree " << net.get_total_degree() << endl;
        net.add_node();
//        net.view_preferentially();
//        net.view_nodes();
//        net.view_links();
//        net.view_preferentially();
    }
//    net.view_nodes();
//    net.view_links();
    cout << "total degree " << net.get_total_degree() << endl;
    net.timeElapsed();

}


void network_percolation(int argc, char* argv[]){

    NetworkBApercolation net(3, 2, 10);
    net.viewNodes();
    net.viewLinks();
    net.viewClusterExtended();
    net.reset(1);
    net.viewNodes();
    net.viewLinks();
    net.viewClusterExtended();

    size_t i{};
    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;
//        cout << i << "-th link " << net.lastLink() << endl;
//        cout << "p = " << net.occupationProbability() << endl;
//        cout << "P = " << net.largestClusterSize() << endl;
        ++i;
//        net.viewClusterExtended();
    }
    net.viewNodes();
    net.viewLinks();
    net.viewClusterExtended();
    net.reset();
    net.viewNodes();
    net.viewLinks();
    net.viewClusterExtended();

}



void explosive_percolation_sum(uint m, uint network_size, uint M, uint ensemble_size, size_t th_id) {
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
//        net.viewNodes();
//        net.viewLinks();
//        net.viewClusterExtended();

        size_t i{};
        while (net.occupyLink()) {
//            cout << " ************* **************** *************" << endl;
//            cout << i << "-th link " << net.lastLink() << endl;
//            cout << "p = " << net.occupationProbability() << endl;
//            cout << "P = " << net.largestClusterSize() << endl;
            largest_cluster_size[i] += net.largestClusterSize();
            entropy1[i] += net.entropy_v1();
            entropy2[i] += net.entropy_v2();
            net.jump();
//            cout << net.largestEntropyJump() << " at " << net.largestEntropyJump_pc() << endl;
//            cout << entropy1[i] << " ==? " << entropy2[i] << endl;

            ++i;
//            net.viewClusterExtended();
        }
        entropy_jump[k] = net.largestEntropyJump();
        entropy_jump_pc[k] = net.largestEntropyJump_pc();
//        cout << net.largestEntropyJump() << " at " << net.largestEntropyJump_pc() << endl;
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

    fout << ss.str() << endl;
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


void network_percolation_to_file(int argc, char* argv[]){
    uint m = atoi(argv[1]);
    uint network_size = atoi(argv[2]);
    uint ensemble_size = atoi(argv[3]);

    NetworkBApercolation net(3, m, network_size);
    net.reset();
    net.viewNodes();
    net.viewLinks();
    net.viewCluster();
    size_t i{};
    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;
//        cout << i << "-th link " << net.lastLink() << endl;
//        cout << "p = " << net.occupationProbability() << endl;
        ++i;
        net.viewClusterExtended();
    }

}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    std::cout << "Netork" << std::endl;
    cout << currentTime() << endl;
    cout << "Compiled on " << __DATE__ << "\t at " << __TIME__ << endl;
    auto t_start = std::chrono::system_clock::now();
    if(sizeof(uint) < 4){
        cout << "size of unsigned int is less than 4 byte : line " <<__LINE__ << endl;
        return 0;
    }
    time_t seed = time(nullptr);
//    srand(seed);    // seeding


//    run_in_main(argc, argv);
//    degree_distribution_ba(argc, argv);
//      network_percolation(argc, argv);
//    network_percolation_explosive(argc, argv);

//    cout << "cores " << thread::hardware_concurrency() << endl;
    explosive_percolation_sum(3, 5000, 5, 100, 0); // testing only

    auto t_end= std::chrono::system_clock::now();
    std::chrono::duration<double> drtion = t_end - t_start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(t_end);
    cout << "Program finished at " << std::ctime(&end_time) << endl;
    std::cout << "Time elapsed "   << getFormattedTime(drtion.count()) << std::endl;
    return 0;

}