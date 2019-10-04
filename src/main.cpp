#include <iostream>
#include <chrono>
#include "util/time_tracking.h"
#include "nets/network.h"
#include "test/test_network_percolation.h"
#include "test/network_build.h"
#include "percolation/network_percolation.h"
#include "percolation/network_percolation_v2.h"
#include "percolation/network_percolation_v3.h"
#include "percolation/network_percolation_v4.h"
#include "percolation/explosive/network_percolation_explosive.h"
#include "test/test_network_ba_v2.h"
//#include "percolation/network_percolation2.h"
#include <algorithm>
#include <fstream>
#include <thread>


void test_component_sizes();

void test_v5(int argc, char **pString);

using namespace std;





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
//        cout << i << "-th link " << _network_frame.lastLink() << endl;
//        cout << "p = " << _network_frame.occupationProbability() << endl;
        ++i;
        net.viewClusterExtended();
    }

}

void test_sizes()
{
    cout << "size of a link " << sizeof(Link) << " bytes" << endl;
    cout << "size of a node " << sizeof(Node) << " bytes" << endl;
    cout << "size of a LinkIndex " << sizeof(LinkIndex) << " bytes" << endl;
    cout << "size of a Cluster " << sizeof(Cluster) << " bytes" << endl;
    cout << "size of a Cluster_v2 " << sizeof(Cluster_v2) << " bytes" << endl;
    cout << "size of a Cluster_v3 " << sizeof(Cluster_v2) << " bytes" << endl;
    cout << "size of a Network " << sizeof(Network) << " bytes" << endl;
    cout << "size of a NetworkBA " << sizeof(NetworkBA) << " bytes" << endl;
    cout << "size of a NetworkBApercolation_v3 " << sizeof(NetworkBApercolation_v3) << " bytes" << endl;
    cout << "size of a NetworkBApercolation_v4 " << sizeof(NetworkBApercolation_v4) << " bytes" << endl;

    cout << "size of a NetworkBApercolationExplosive_v3 " << sizeof(NetworkBApercolationExplosive_v3) << " bytes" << endl;

}

void test_component_sizes() {
   Node n = Node(0);
   cout << "sizeof Node " << sizeof(n) << " bytes" << endl;
   for (size_t i{1}; i < 1000000; ++i) {
       n.addNeighbor(i);
   }
   n.shrink_to_fit();
   cout << "sizeof Node " << n.size_in_MB() << " MB" << endl;
   // cin.get();
}

/**
 * N : network size
 * m :
 * M :
 * @param argc
 * @param argv
 */
void run_in_main(int argc, char* argv[]){
//    vector<string> arguments{"m","N","M", "Ensemble"};
//    cout << "Arguments " << endl;
//    for(int i{1}; i < argc; ++i){
//        cout << arguments[i-1] << " : " << argv[i] << endl;
//    }

//    test_sizes(); // 2019.05.26
//    test_component_sizes(); // 2019.05.30
//    test_network_BA(argc, argv);
//    test_network_MDA();
//    network_percolation_global();
//    network_percolationReverse_global();

//    network_percolation_to_file(argc, argv);
//    degree_distribution_BA(argc, argv);
//    degree_distribution_BA_v2(argc, argv); // 2019.05.21
//    network_percolation(argc, argv);
//    network_percolation_explosive(argc, argv);
//
//    cout << "cores " << thread::hardware_concurrency() << endl;
//    explosive_percolation_sum(3, 5000, 5, 100, 0); // testing only

//    BA_self_jump(argc, argv);
    // 2019.07.02
//    test_ba_v2();
//    test_ba_v2_2(argc, argv);
//    test_ba_v2_3();
    // 2019.07.20

//    test_NetworkBApercolationExplosive_v3(argc, argv);
//    test_NetworkBApercolationExplosive_v3_jump(argc, argv);
    test_v5(argc,argv);
//    test_NetworkBA_v2(argc, argv);

}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    std::cout << "Network" << std::endl;
    cout << currentTime() << endl;
    cout << "Compiled on " << __DATE__ << "\t at " << __TIME__ << endl;
    auto t_start = std::chrono::system_clock::now();
    if(sizeof(uint) < 4){
        cout << "size of unsigned int is less than 4 byte : line " <<__LINE__ << endl;
        return 0;
    }
//    time_t seed = time(nullptr);
//    srand(seed);    // seeding


    run_in_main(argc, argv);


    auto t_end= std::chrono::system_clock::now();
    std::chrono::duration<double> drtion = t_end - t_start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(t_end);
    cout << "Program finished at " << std::ctime(&end_time) << endl;
    std::cout << "Time elapsed "   << getFormattedTime(drtion.count()) << std::endl;
    return 0;

}