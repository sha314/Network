//
// Created by shahnoor on 11/2/19.
//

#include "test_v7.h"



#include <chrono>
#include <fstream>
#include "../percolation/network_percolation_v7.h"
#include "../util/time_tracking.h"
#include "../nets/network.h"
#include "../nets/ER/network_ER.h"
#include "../nets/BA/network_BA_v7.h"
#include "../nets/MDA/network_MDA_v7.h"



using namespace std;


void test_v7(int argc, char **argv) {
//    test_ER();
//    test_ER_Percolation();
//    test_ER_PercolationExplosive();
//    run_ER_percolation(argc, argv);
//    test_BA(argc, argv);
//    test_MDA(argc, argv);
    test_percolation(argc, argv);
}

void test_MDA(int argc, char *argv[]) {
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

    int En = atoi(argv[4]);
    cout << "m=" << m << ",N="<< N << ",M=" << M << ",En="<<En << endl;


    auto* net = new NetworkMDA_v7(m, m);
    net->setRandomState(0, true);
    net->initialize(N);
    vector<size_t> counts;
    for(int e{1}; e <= En; ++e){
        auto t0 = std::chrono::system_clock::now();
        net->rebuild();
        auto dd = net->degreeCounts();
        if(counts.size() < dd.size()){
            counts.resize(dd.size());
        }
        for(size_t i{}; i < dd.size(); ++i){
//        if(dd[i] == 0) continue;
//            cout << "k= " << i << " count=" << dd[i] << endl;
            counts[i] += dd[i];
        }
        auto t1 = std::chrono::system_clock::now();
        std::chrono::duration<double> drtion = t1 - t0;
        cout << "Iteration " << e << " : time " << drtion.count() << " sec" << endl;
    }
//    net->view();
//    net->viewAdjacencyList();
//    net->viewLocal();
    cout << "Done" << endl;
//    auto dd = net->degreeCounts();
//    for(size_t i{}; i < dd.size(); ++i){
//        if(dd[i] == 0) continue;
//        cout << "k= " << i << " count=" << dd[i] << endl;
//    }

    auto arr  = net->get_signature_array();
    string filename = net->getClassName() +  arr[1] + "degree-distribution" + currentTime() + ".txt";
    ofstream fout(filename);
    fout << "#{\"m0\":" << net->get_m0()
         << ",\"m\":" << net->get_m()
         << ",\"N\":" << N
         << ",\"En\":" << En
         << ",\"type\":\"" << arr[0] << "\""
         << "}" << endl;
    fout << "#<k>,<P(k)>" << endl;
    for (size_t i{1}; i < counts.size(); ++i) {
        if(counts[i] == 0) continue;
        fout << i << "\t" << double(counts[i])/En << endl;
    }

    // normalization
    fout.close();
}


void test_ER() {
    Network_v7* net = new NetworkER_v7(0.8);
    net->setRandomState(0, true);
    net->initialize(100);
    net->view();
    net->viewLocal();

}


void test_ER_Percolation() {
    NetworkER_v7* net=new NetworkER_v7(0.4);
    net->setRandomState(0, true);
    net->initialize(10);
//    net->view();
//    net->viewLocal();
    net->viewAdjacencyList();

    NetworkPercolation_v7 percolation(net);
    percolation.initialize();
    percolation.viewClusters();
    while(percolation.occupyLink()){
//        percolation.viewClusters();
//        cout << percolation.entropy_v1() << " , " << percolation.entropy_v2() << endl;
        cout << percolation.largestClusterSize() << endl;
    }
    percolation.viewClusters();

}

void test_ER_PercolationExplosive() {
    auto * net = new NetworkER_v7(0.4);
    net->setRandomState(0, false);
    net->initialize(10);
//    net->view();
//    net->viewLocal();
    net->viewAdjacencyList();

    NetworkPercolationExplosive_v7 percolation(net, 2);
//    NetworkPercolationInverted_v7 percolation(net, 2);

    percolation.initialize();
    percolation.viewClusters();
    while(percolation.occupyLink()){
//        percolation.viewClusters();
//        cout << percolation.entropy_v1() << " , " << percolation.entropy_v2() << endl;
        cout << percolation.largestClusterSize() << endl;
    }
    percolation.viewClusters();

}

void run_ER_percolation(int argc, char **argv){
    if(argc < 5 ){
        cout << "argv[1] == N" << endl;
        cout << "argv[2] == p" << endl;
        cout << "argv[3] == M" << endl;
        cout << "argv[4] == Ensemble" << endl;
        return;
    }
    int N = atoi(argv[1]);
    double p = atof(argv[2]);
    int M = atoi(argv[3]);
    int ensemble_size = atoi(argv[4]);
    cout << "N="<< N << ",p=" << p << ",M=" << M << ",En="<<ensemble_size << endl;

    auto * net=new NetworkER_v7(p);
    net->setRandomState(0, true);
    net->initialize(N);

//    NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    net.viewNetwork();
//    net.viewListOfLinkIndices();
    double entropy_jump{}, order_jump{};
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    for (int k{0}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        percolation.reset(0); // every 25 step. reset the network
//        cout << net->getLinkCount() << endl;
        size_t i{};
//        net.viewClusters();
//        net.viewListOfLinkIndices();
//        net.viewNetwork();
//        cout << "entering to while" << endl;
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;
            entropy[i] += percolation.entropy();
            order_param[i] += percolation.largestClusterSize();
//            net.viewClusters();
//            cout << net.entropy_v1()  << "\t";
//            cout << net.entropy_v2() << endl;
//            cout << net.largestClusterSize() << endl;
            percolation.jump();
//            _network_frame.viewClusterExtended();
            ++i;
            if (i >= linkCount) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
        entropy_jump += percolation.largestEntropyJump();
        order_jump += percolation.largestOrderJump();

//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    auto tm = currentTime();
//    string signature = percolation.get_signature();
    string signature = net->get_signature_array()[0] +"_"+ percolation.getClassName()  + "_N_" + to_string(N) + "_p_";
    std::ostringstream out;
    out.precision(3);
    out << std::fixed << p;
    signature +=  out.str() + "_M_" + to_string(M);
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"network_class":")*" << net->getClassName() << "\""
       << R"*(,"percolation_class":")*" << percolation.getClassName() << "\""
       << R"*(,"p":)*" << p
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << ensemble_size
       << R"*(,"date":")*" << tm << "\""
       << "}";



    string filename = signature + "_entropy-order_" + tm + ".txt";
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
    for(size_t k{}; k < entropy.size() ; ++k){
        auto t = (k+1)/double(N);
        if (t > 5) {
            cout << "breaking at t=" << t << endl;
            break;
        }
        fout << t
             << "\t" << entropy[k]/ensemble_size
             << "\t" << order_param[k]/(ensemble_size*double(N)) << endl;
    }
    fout.close();
}





void test_percolation(int argc, char **argv) {
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

    int En = atoi(argv[4]);
    cout << "m=" << m << ",N="<< N << ",M=" << M << ",En="<<En << endl;


    auto* net = new NetworkMDA_v7(m, m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//    net->view();
//    net->viewAdjacencyList();
//    net->viewLocal();
//    cout << "Done" << endl;
//    auto dd = net->degreeDistribution();
//    for(size_t i{}; i < dd.size(); ++i){
//        cout << "k= " << i << " count=" << dd[i] << endl;
//    }

//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t limit = nodeCount * 2;
//    percolation.viewNetwork();
//    percolation.viewListOfLinkIndices();
    double entropy_jump{}, order_jump{};
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        percolation.reset(k % 100 == 0); // every 100 step. reset the network
//        cout << net->getLinkCount() << endl;
        size_t i{};
//        percolation.viewClusters();
//        percolation.viewListOfLinkIndices();
//        percolation.viewNetwork();
//        cout << "entering to while" << endl;
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;
            entropy[i] += percolation.entropy();
            order_param[i] += percolation.largestClusterSize();
//            percolation.viewClusters();
//            percolation.viewListOfLinkIndices();
//            cout << percolation.entropy_v1()  << "\t";
//            cout << percolation.entropy_v2()  << "\t";
//            cout << percolation.largestClusterSize() << endl;
            percolation.jump();
//            _network_frame.viewClusterExtended();
            ++i;

            if (i >= limit) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
        entropy_jump += percolation.largestEntropyJump();
        order_jump += percolation.largestOrderJump();

//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    auto tm = currentTime();
//    string signature = percolation.get_signature();
    string signature = net->get_signature_array()[0] +"_"+ percolation.getClassName()
                       + "_N_" + to_string(N)
                       + "_m_" + to_string(m)
                       + "_M_" + to_string(M);
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"network_class":")*" << net->getClassName() << "\""
       << R"*(,"percolation_class":")*" << percolation.getClassName() << "\""
       << R"*(,"m":)*" << m
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"date":")*" << tm << "\""
       << "}";



    string filename = signature + "_entropy-order_" + tm + ".txt";
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
    for(size_t k{}; k < limit ; ++k){
        auto t = (k+1)/double(N);
        fout << t
             << "\t" << entropy[k]/En
             << "\t" << order_param[k]/(En*double(N)) << endl;
    }
    fout.close();

    string filename_jump = signature + "_jump_" + tm + ".txt";

    ofstream fout_jump(filename_jump);
    fout_jump << '#' << ss.str() << endl;
    fout_jump << "#<m><N><M><En><largest entropy jump><largest order parameter jump>" << endl;
    fout_jump << m << "\t" << N << "\t" << M << "\t" << En
              << "\t" << abs(entropy_jump)/En << '\t' << order_jump/En << endl;
    fout_jump.close();
}

void test_BA(int argc, char* argv[]) {
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

    int En = atoi(argv[4]);
    cout << "m=" << m << ",N="<< N << ",M=" << M << ",En="<<En << endl;


    auto* net = new NetworkBA_v7(m, m);
    net->setRandomState(0, true);
    net->initialize(N);
    vector<size_t> counts;
    for(int e{1}; e <= En; ++e){
        auto t0 = std::chrono::system_clock::now();
        net->rebuild();
        auto dd = net->degreeCounts();
        if(counts.size() < dd.size()){
            counts.resize(dd.size());
        }
        for(size_t i{}; i < dd.size(); ++i){
//        if(dd[i] == 0) continue;
//            cout << "k= " << i << " count=" << dd[i] << endl;
            counts[i] += dd[i];
        }
        auto t1 = std::chrono::system_clock::now();
        std::chrono::duration<double> drtion = t1 - t0;
        cout << "Iteration " << e << " : time " << drtion.count() << " sec" << endl;
    }
//    net->view();
//    net->viewAdjacencyList();
//    net->viewLocal();
    cout << "Done" << endl;
//    auto dd = net->degreeCounts();
//    for(size_t i{}; i < dd.size(); ++i){
//        if(dd[i] == 0) continue;
//        cout << "k= " << i << " count=" << dd[i] << endl;
//    }

    auto arr  = net->get_signature_array();
    string filename = net->getClassName() +  arr[1] + "degree-distribution" + currentTime() + ".txt";
    ofstream fout(filename);
    fout << "#{\"m0\":" << net->get_m0()
         << ",\"m\":" << net->get_m()
         << ",\"N\":" << N
         << ",\"En\":" << En
         << "}" << endl;
    fout << "#<k>,<P(k)>" << endl;
    for (size_t i{1}; i < counts.size(); ++i) {
        if(counts[i] == 0) continue;
        fout << i << "\t" << double(counts[i])/En << endl;
    }

    // normalization
    fout.close();

}