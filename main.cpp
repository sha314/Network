#include <iostream>
#include <chrono>
#include "src/util/time_tracking.h"
#include "src/network.h"
#include <algorithm>
#include <fstream>

using namespace std;

void degree_distribution_ba(int argc, char* argv[]){
//    size_t m0 = size_t(atoi(argv[1]));
    if(argc < 1){
        cout << "supply argument " << endl;
        return;
    }

    size_t m0 = 3;

    size_t network_size = size_t(atoi(argv[1]));
    size_t ensemble_size = size_t(atoi(argv[2]));
    cout << "network size " << network_size << endl;
    cout << "ensemble size " << ensemble_size << endl;

    vector<double> degree(network_size + m0);
    NetworkBA net(m0);
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
            degree[i] += degs[i];
        }
//        cout << "line " << __LINE__ << endl;
        auto t1= std::chrono::system_clock::now();
        std::chrono::duration<double> drtion = t1 - t0;
        cout << "Iteration " << en << " : time " << drtion.count() << " sec" << endl;
    }

    // normalization

    string filename =  net.get_signature() + "degree" + currentTime() + ".txt";
    ofstream fout(filename);
    for (size_t i{}; i < degree.size(); ++i) {
        degree[i] /= ensemble_size;
        fout << degree[i] << endl;
    }
}

void run_in_main(int argc, char* argv[]){
    NetworkBA net(3);
    net.view_nodes();
    net.view_links();


    size_t network_size = size_t(atoi(argv[1]));

    cout << net.get_total_degree() << endl;

    for(size_t i{}; i < network_size ; ++i){
        net.add_node();
//        net.view_nodes();
//        net.view_links();
    }

    net.view_nodes();
    net.view_links();
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

    time_t seed = time(nullptr);
    srand(seed);    // seeding

//    run_in_main(argc, argv);
    degree_distribution_ba(argc, argv);

    auto t_end= std::chrono::system_clock::now();
    std::chrono::duration<double> drtion = t_end - t_start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(t_end);
    cout << "Program finished at " << std::ctime(&end_time) << endl;
    std::cout << "Time elapsed "   << getFormattedTime(drtion.count()) << std::endl;
    return 0;

}