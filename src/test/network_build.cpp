//
// Created by shahnoor on 12/19/18.
//

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include "network_build.h"
#include "../nets/network.h"
#include "../util/time_tracking.h"

using namespace std;

void test_network(){
    Network net(3, 1);

    net.view_nodes();
    net.view_links();

    net.add_node_v2();

    net.view_nodes();
    net.view_links();

}

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

    string filename =  net.get_signature() + "size_" + to_string(network_size)
                       + "-neighborCount" + currentTime() + ".txt";
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
//        _network_frame.view_nodes();

//        cout << "line " << __LINE__ << endl;

        vector<double> degs = net.degrees();
        std::sort(degs.begin(), degs.end());

//        cout << degs.size() << " and  " << neighborCount.size() << endl;
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
