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
#include "../nets/BA/network_BA.h"
#include "../nets/MDA/network_MDA.h"

using namespace std;

void test_network_BA(int argc, char * argv[]){

//    size_t m = 1;

    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
//    int M = atoi(argv[3]);

    NetworkBA net(m, m);
    net.setRandomState(0, false);
    net.grow(N);
    net.timeElapsed();
//    double sz = net.size_in_MB();
//    cout << "NetworkBA size " << sz << " MB" << endl;
//    net.viewLinks();

//    net.viewNodes();
//    net.view_links();
//    size_t i=0;
//
//    while(i < N) {
//        net.addNode();
////        net.viewNodes();
//        ++i;
//    }
//    net.shrink_to_fit();
//    net.viewNodes();
//    net.view_links();
//
//    net.clear_preferentially();
//    net.addNode();
//    net.reset();
//    net.viewNodes();
//    net.addNode();
//    net.viewNodes();
//    sz = net.size_in_MB();
//    cout << "Last NetworkBA size " << sz << " MB" << endl;
//    cout << "NetworkBA " << sizeof(NetworkBA) << endl;
//    cin.get(); // pausing program

}

void test_network_MDA(){
    NetworkMDA net(10, 5);

//    net.viewNodes();
//    net.view_links();
    size_t i=0;

    while(i < 10000) {
        net.addNode();
//        net.viewNodes();
        ++i;
    }

//    net.viewNodes();
//    net.view_links();

}

void degree_distribution_BA(int argc, char **argv) {
//    size_t m0 = size_t(atoi(argv[1]));
    if (argc < 2) {
        cout << "argv[1] = m" << endl;
        cout << "argv[2] = NetworkSize" << endl;
        cout << "argv[3] = EnsembleSize" << endl;
        return;
    }

    size_t m = size_t(atoi(argv[1]));
    size_t N = size_t(atoi(argv[2]));
    size_t En = size_t(atoi(argv[3]));

    cout << "network size " << N << endl;
    cout << "ensemble size " << En << endl;

    NetworkBA net(m, m);
    cout << "m0 " << net.get_m0() << ", m " << net.get_m() << endl;

    string filename = net.get_signature() + "size_" + to_string(N)
                      + "-degree-" + currentTime() + ".txt";
    ofstream fout(filename);
    fout << "#{\"m0\":" << net.get_m0()
         << ",\"m\":" << net.get_m()
         << ",\"N\":" << N
         << ",\"En\":" << En
         << "}" << endl;

    for (size_t en{}; en < En; ++en) {
        auto t0 = std::chrono::system_clock::now();
        net.reset();

        for (size_t i{}; i < N; ++i) {
            net.addNode();
        }
//        _network_frame.view_nodes();

//        cout << "line " << __LINE__ << endl;

        vector<uint> degs = net.degrees();
        std::sort(degs.begin(), degs.end());

//        cout << degs.size() << " and  " << neighborCount.size() << endl;
        for (size_t i{}; i < degs.size(); ++i) {
            fout << degs[i] << ',';
        }
        fout << endl;
//        cout << "line " << __LINE__ << endl;
        auto t1 = std::chrono::system_clock::now();
        std::chrono::duration<double> drtion = t1 - t0;
        cout << "Iteration " << en << " : time " << drtion.count() << " sec" << endl;
    }

    // normalization
    fout.close();

}


void degree_distribution_BA_v2(int argc, char **argv) {
//    size_t m0 = size_t(atoi(argv[1]));
    if (argc < 2) {
        cout << "argv[1] = m" << endl;
        cout << "argv[2] = NetworkSize" << endl;
        cout << "argv[3] = EnsembleSize" << endl;
        return;
    }

    size_t m = size_t(atoi(argv[1]));
    size_t N = size_t(atoi(argv[2]));
    size_t En = size_t(atoi(argv[3]));

    cout << "network size " << N << endl;
    cout << "ensemble size " << En << endl;

    NetworkBA net(m, m);
    cout << "m0 " << net.get_m0() << ", m " << net.get_m() << endl;

    string filename = net.get_signature() + "size_" + to_string(N)
                      + "-degree_distribution-" + currentTime() + ".txt";


    vector<size_t> count(10);

    for (size_t en{}; en < En; ++en) {
        auto t0 = std::chrono::system_clock::now();
        net.reset();

        for (size_t i{}; i < N; ++i) {
            net.addNode();
        }
//        _network_frame.view_nodes();

//        cout << "line " << __LINE__ << endl;

        vector<uint> degs = net.degrees();
//        std::sort(degs.begin(), degs.end());

//        cout << degs.size() << " and  " << neighborCount.size() << endl;

        for (size_t i{}; i < degs.size(); ++i) {
            if(degs[i] >= count.size()){
                count.resize(degs[i]);
            }
            ++count[degs[i]];
        }


//        cout << "line " << __LINE__ << endl;
        auto t1 = std::chrono::system_clock::now();
        std::chrono::duration<double> drtion = t1 - t0;
        cout << "Iteration " << en << " : time " << drtion.count() << " sec" << endl;
    }

    ofstream fout(filename);
    fout << "#{\"m0\":" << net.get_m0()
         << ",\"m\":" << net.get_m()
         << ",\"N\":" << N
         << ",\"En\":" << En
         << "}" << endl;

    for (size_t i{1}; i < count.size(); ++i) {
        fout << i << "\t" << double(count[i])/En << endl;
    }

    // normalization
    fout.close();

}
