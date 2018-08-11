#include <iostream>
#include <chrono>
#include "src/util/time_tracking.h"
#include "src/nets/network.h"
#include <algorithm>
#include <fstream>

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
    while (net.occupy_link()){
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
    while (net.occupy_link()){
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
      network_percolation(argc, argv);

    auto t_end= std::chrono::system_clock::now();
    std::chrono::duration<double> drtion = t_end - t_start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(t_end);
    cout << "Program finished at " << std::ctime(&end_time) << endl;
    std::cout << "Time elapsed "   << getFormattedTime(drtion.count()) << std::endl;
    return 0;

}