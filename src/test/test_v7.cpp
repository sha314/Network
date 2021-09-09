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
//    test_percolation(argc, argv);


//    run_v7_percolation(argc, argv);
//    run_v7_percolation_jump(argc, argv);
//    run_v7_percolation_jump_avg(argc, argv);

//    run_v7_percolation_near_tc(argc, argv);

//    run_v7_percolation_old_susceptibility(argc, argv);
//    run_v7_percolation_1st_order_check(argc, argv);
//    run_v7_percolation_1st_order_check_2(argc, argv);

    run_v7_percolation_delta_powder_keg(argc, argv);
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


    auto* net = new NetworkMDA_v7(m);
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
    fout << "#k = degree" << endl;
    fout << "#P(k) = repetition of k on average" << endl;
    fout << "# from P(k) to get degree distribution divide it by maximum degree" << endl;
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
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
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


    auto* net = new NetworkBA_v7(m);
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
        percolation.reset(k % 25 == 0); // every 100 step. reset the network
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
            percolation.jump();
//            percolation.viewClusters();
//            percolation.viewListOfLinkIndices();
//            double H1 = percolation.entropy_v1();
//            double H2 = percolation.entropy_v2();
//            cout << H1 << "\t";
//            cout << H2 << "\t";
//            cout << percolation.largestClusterSize() << endl;

//            _network_frame.viewClusterExtended();
//            if(abs(H1 - H2) > 1e-5){
//                cerr << "problem : line " << __LINE__ << endl;
//            }

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
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
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


    auto* net = new NetworkBA_v7(m);
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


void run_v7_percolation_1st_order_check_2(int argc, char **argv){
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
    size_t rebuild_each = 100; // cycle

    auto* net = new NetworkBA_v7(m);
//    auto* net = new NetworkMDA_v7(m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    size_t limit = nodeCount * 3;
//    double entropy_jump{}, order_jump{};
    vector<long> t1s, t2s;
    long t1=0, t2=0;
    long dS_limit = 10, S_max;
    cout << "dS_limit " << dS_limit << endl;
    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
        percolation.reset(k % rebuild_each == 0); // every nn step. reset the network
        size_t i{};

//        cout << "sqrt N " << S1 << endl;
        t2 = t1 = 0;
        long S0 = percolation.largestClusterSize();
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;

            S_max = percolation.largestClusterSize();
//            double t = percolation.relativeLinkDensity();

            long dS = S_max - S0;

//            cout << "t= " << t << " S_max= " << S_max << " dS = " << dS << endl;

            S0 = S_max;


            if ((dS > dS_limit) & (t1 == 0)){
                // first time jump is less than limit
                t1 = i;
//                cout << "t1= " << i <<  " t= " << t << " S_max= " << S_max << " dS = " << dS << endl;
            }

            if ((dS > dS_limit) & (t1 != 0)){
                // last time jump is greater than limit
                t2 = i;
//                cout << "t2= " << i <<  " t= " << t << " S_max= " << S_max << " dS = " << dS << endl;
            }
//            if(dS < dS_limit){
//                cout << "else " <<  " t= " << t << " S_max= " << S_max << " dS = " << dS << endl;
//            }

            ++i;
//            if (t > 1.05) break;
//            if ((S_max >= S1) & (t1 == 0)){
//                t1 = i;
//                t1s.push_back(t1);
////                cout << "S1 at " << i << endl;
//                S1 = 0; // so that it does not enter this loop second time
//            }
//            if(S_max >= S2){
//                t2 = i;
//                t2s.push_back(t2);
////                cout << "S2 at " << i << endl;
//                break;
//            }
            if (S_max == nodeCount){
                cout << "maximum cluster size reached. breaking loop " << endl;
                break;
            }
            if (i > linkCount) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
//        cout << "t2 " << t2 << endl;
//        cout << "t1 " << t1 << endl;
        t1s.push_back(t1);
        t2s.push_back(t2);

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
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"network_seed":)*" << net->get_m0()
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"datetime":")*" << tm << "\""
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



    string filename_entropy_order = signature + "_1st-order-check_" + tm + ".txt";
    ofstream fout(filename_entropy_order);
    fout << '#' << ss.str() << endl;
    fout << "# limit of jump of largest cluster size, dS_limit = " << dS_limit << endl;
    fout << "# t1=number of link required to get jump of largest cluster to be dS_limit for the first time" << endl;
    fout << "# t2=number of link required to get jump of largest cluster to be dS_limit for the last time" << endl;
    fout << "# dt = t2 - t1" << endl;
    fout << "#<t1> <t2>" << endl;
//    fout.precision(12);
    fout.precision(numeric_limits<double>::digits10 + 1); // maximum precision
    for(size_t k{}; k < t1s.size() ; ++k){
//        auto t = (k+1)/double(N);
        fout << t1s[k]  << '\t' << t2s[k] << endl;

    }
    fout.close();

}

void run_v7_percolation_1st_order_check(int argc, char **argv){
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
    size_t rebuild_each = 100; // cycle

    auto* net = new NetworkBA_v7(m);
//    auto* net = new NetworkMDA_v7(m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    size_t limit = nodeCount * 3;
//    double entropy_jump{}, order_jump{};
    vector<long> t1s, t2s;
    long t1=0, t2=0;
    long S1=long(sqrt(N)), S2=N/2, S_max;
    cout << "S1 " << S1 << " S2 " << S2 << endl;
    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
        percolation.reset(k % rebuild_each == 0); // every nn step. reset the network
        size_t i{};
        S1=long(sqrt(N));
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;
            S_max = percolation.largestClusterSize();
//            cout << "S_max " << S_max << endl;
            ++i;
            if ((S_max >= S1) & (t1 == 0)){
                t1 = i;
                t1s.push_back(t1);
//                cout << "S1 at " << i << endl;
                S1 = 0; // so that it does not enter this loop second time
            }
            if(S_max >= S2){
                t2 = i;
                t2s.push_back(t2);
//                cout << "S2 at " << i << endl;
                break;
            }
            if (i > linkCount) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
//        cout << "t2 " << t2 << endl;
//        cout << "t1 " << t1 << endl;
//        t1s.push_back(t1);
//        t2s.push_back(t2);
        t2 = t1 = 0;
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
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"network_seed":)*" << net->get_m0()
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"datetime":")*" << tm << "\""
       << R"*(,"compile_date":")*" << __DATE__ << "\""
       << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



    string filename_entropy_order = signature + "_1st-order-check_" + tm + ".txt";
    ofstream fout(filename_entropy_order);
    fout << '#' << ss.str() << endl;
    fout << "# t1=number of link required to get the largest cluster to a size of sqrt(N)" << endl;
    fout << "# t2=number of link required to get the largest cluster to a size of N/2" << endl;
    fout << "# dt = t2 - t1" << endl;
    fout << "#<t1> <t2>" << endl;
//    fout.precision(12);
    fout.precision(numeric_limits<double>::digits10 + 1); // maximum precision
    for(size_t k{}; k < t1s.size() ; ++k){
//        auto t = (k+1)/double(N);
        fout << t1s[k]  << '\t' << t2s[k] << endl;

    }
    fout.close();

}

void run_v7_percolation(int argc, char **argv) {
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
    size_t rebuild_each = 100; // cycle

    auto* net = new NetworkBA_v7(m);
//    auto* net = new NetworkMDA_v7(m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t limit = nodeCount * 2;
    if (limit > linkCount) limit = linkCount;
//    double entropy_jump{}, order_jump{};
    vector<long double> entropy(linkCount), dHs(linkCount);
    vector<double> order_param(linkCount); // entropy and order parameter
    vector<long> dPs(linkCount);
    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
        percolation.reset(k % rebuild_each == 0); // every nn step. reset the network
        size_t i{};
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;
//            auto H = percolation.entropy_v1(); // slow. for debugging purposes
//            percolation.viewClusters();
//            percolation.sumClusters();
            auto H = percolation.entropy();
            if(isnan(H)){
                cout << "H is nan " << endl;
                exit(-1);
            }
            entropy[i] += H;
            order_param[i] += percolation.largestClusterSize();
            percolation.jump();

            dHs[i] += percolation.jump_entropy();
            dPs[i] += percolation.jump_largest_cluster();

            ++i;
            if (i >= limit) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
//        entropy_jump += percolation.largestEntropyJump();
//        order_jump += percolation.largestOrderJump();

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
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"network_seed":)*" << net->get_m0()
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"datetime":")*" << tm << "\""
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



    string filename_entropy_order = signature + "_entropy-order_" + tm + ".txt";
    ofstream fout(filename_entropy_order);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "#<t>\t<H>\t<P>" << endl;
//    fout.precision(12);
    cout << "Writting data to file" << endl;
    fout.precision(numeric_limits<double>::digits10 + 1); // maximum precision
    for(size_t k{}; k < limit ; ++k){
        auto t = (k+1)/double(N);
        fout << t
             << "\t" << entropy[k]/En
             << "\t" << order_param[k]/(En*double(N)) << endl;
    }
    fout.close();


    string filename_specific_heat_susceptibility = signature + "_specific_heat-susceptibility_" + tm + ".txt";
    ofstream fout2(filename_specific_heat_susceptibility);
    fout2 << '#' << ss.str() << endl;
    fout2 << "# t=relative link density" << endl;
    fout2 << "#<t>\t<C>\t<X>" << endl;
//    fout2.precision(12);
    fout2.precision(numeric_limits<double>::digits10 + 1); // maximum precision
    for(size_t k{}; k < limit ; ++k){
        auto t = (k+1)/double(N);
        fout2 << t
             << "\t" << -1.0*(1.0-t)*dHs[k]*N/En
             << "\t" << dPs[k]/double(En) << endl;
    }
    fout2.close();

}

/**
 * I needed largest cluster size data near tc. But generally tc is unknown, I took data for all t
 * then while analyzing data I took what I needed
 * @param argc
 * @param argv
 */
void run_v7_percolation_near_tc(int argc, char **argv) {
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

    int reset_step = 100;
//    auto* net = new NetworkBA_v7(m);
    auto* net = new NetworkER_v7(N, N);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t limit = nodeCount * 2;
//    double entropy_jump{}, order_jump{};
    vector<size_t> largest_cluster; // entropy and order parameter
//    double tc = percolation.get_tc(M);
//    cout << "tc = " << tc << endl;
//    double tc0 = tc-0.1;
//    double tc1 = tc+0.1;

    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
        percolation.reset(k % reset_step == 0); // every 100 step. reset the network
        size_t i{};
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;
//            largest_cluster[i++] += percolation.largestClusterSize();

//            auto t = percolation.relativeLinkDensity();
//            cout << "relativeLinkDensity " << t <<  " 1/N" << 1.0 / nodeCount << " 1/M" << 1.0/ linkCount << endl;
            auto sz = percolation.largestClusterSize();

            // if tc is unknown, take data for all t
            if (i < largest_cluster.size()) {
                largest_cluster[i++] += sz;
            }else{
                largest_cluster.emplace_back(sz);
            }


//            if(t > tc0) {
//                if (i < largest_cluster.size()) {
//                    largest_cluster[i++] += sz;
//                }else{
//                    largest_cluster.emplace_back(sz);
//                }
////            percolation.jump();
//
//            }
//            if(t > tc1){
//                break;
//            }
            if (i >= limit) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
//        entropy_jump += percolation.largestEntropyJump();
//        order_jump += percolation.largestOrderJump();

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
    double dt = 1.0 / nodeCount;
    stringstream ss;
    ss << "{"
       << R"*("signature":")*" << signature << "\""
       << R"*(,"network_class":")*" << net->getClassName() << "\""
       << R"*(,"percolation_class":")*" << percolation.getClassName() << "\""
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
//       << R"*(,"tc":)*" << tc
       << R"*(,"dt":)*" << dt

       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"date":")*" << tm << "\""
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



    cout << "dt = " << dt << endl;
    string filename = signature + "_largest_cluster_near_tc_" + tm + ".txt";
    ofstream fout(filename);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "# S_max = largest cluster" << endl;
    fout << "#<t>\t<S_max>" << endl;
//    fout.precision(12);
    fout.precision(numeric_limits<double>::digits10); // maximum precision

    /// if tc is known
//    auto t = tc0;
//    for(size_t k{}; k < largest_cluster.size() ; ++k){
//        fout << t  << "\t" << largest_cluster[k]/double(En) << endl;
//        t += dt;
//    }


    for(size_t k{}; k < largest_cluster.size() ; ++k){
        fout << (k+1.0)/double(nodeCount)  << "\t" << largest_cluster[k]/double(En) << endl;
    }

    fout.close();

}

void run_v7_percolation_jump_avg(int argc, char **argv) {
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


    auto* net = new NetworkBA_v7(m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t limit = nodeCount * 2;
    double entropy_jump{}, order_jump{};

    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        percolation.reset(k % 25 == 0); // every 100 step. reset the network
//        cout << net->getLinkCount() << endl;
        size_t i{};
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;
            percolation.entropy();
            percolation.jump();

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
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"date":")*" << tm << "\""
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



    string filename_jump = signature + "_jump_" + tm + ".txt";

    ofstream fout_jump(filename_jump);
    fout_jump << '#' << ss.str() << endl;
    fout_jump << "#<m><N><M><En><largest entropy jump><largest order parameter jump>" << endl;
    fout_jump << m << "\t" << N << "\t" << M << "\t" << En
              << "\t" << abs(entropy_jump)/En << '\t' << order_jump/En << endl;
    fout_jump.close();
}


void run_v7_percolation_jump(int argc, char **argv) {
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
    size_t rebuild_each = 1; // cycle

    auto* net = new NetworkBA_v7(m);
//    auto* net = new NetworkMDA_v7(m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t limit = nodeCount * 2;
    vector<double> entropy_jump, order_jump;

    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        percolation.reset(k % rebuild_each == 0); // every 100 step. reset the network
//        cout << net->getLinkCount() << endl;
        size_t i{};
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;
            percolation.entropy();
            percolation.jump();

            ++i;
            if (i >= limit) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
        entropy_jump.emplace_back(percolation.largestEntropyJump());
        order_jump.emplace_back(percolation.largestOrderJump());

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
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"date":")*" << tm << "\""
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



    string filename_jump = signature + "_jump_" + tm + ".txt";

    ofstream fout_jump(filename_jump);
    fout_jump << '#' << ss.str() << endl;
    fout_jump << "#<entropy jump><order parameter jump>" << endl;
    for(size_t i{}; i < entropy_jump.size(); ++i){
        fout_jump << entropy_jump[i] << "\t" << order_jump[i] << endl;
    }
    fout_jump.close();
}


void run_v7_percolation_old_susceptibility(int argc, char **argv) {
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
    size_t rebuild_each = 100; // cycle

//    auto* net = new NetworkBA_v7(m);
    auto* net = new NetworkMDA_v7(m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t limit = nodeCount * 2;
//    double entropy_jump{}, order_jump{};

    vector<double> susceptibility(linkCount); // entropy and order parameter

    for (int k{1}; k <= En; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
        percolation.reset(k % rebuild_each == 0); // every nn step. reset the network
        size_t i{};
        while (percolation.occupyLink()) {
//            cout << "i " << i  << endl;

            auto lcs = percolation.largestClusterSize();
            double ccount = percolation.clusterCountFast();
            if(ccount != percolation.clusterCount()) {
                cout << ccount << " vs " << percolation.clusterCount() << endl;
            }
            susceptibility[i] += (nodeCount - lcs)/ccount;


            ++i;
            if (i >= limit) {
                cout << "breaking at " << i <<endl;
                break;
            }
        }
//        entropy_jump += percolation.largestEntropyJump();
//        order_jump += percolation.largestOrderJump();

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
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"network_seed":)*" << net->get_m0()
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"datetime":")*" << tm << "\""
        << R"*(,"compile_date":")*" << __DATE__ << "\""
        << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



    string filename_specific_heat_susceptibility = signature + "_old_susceptibility_" + tm + ".txt";
    ofstream fout(filename_specific_heat_susceptibility);
    fout << '#' << ss.str() << endl;
    fout << "# t=relative link density" << endl;
    fout << "# Susceptibility X " << endl;
    fout << "# X(t) = (N - S_max(t))/C(t) " << endl;
    fout << "# C(t) = number of clusters at t " << endl;
    fout << "#<t>\t<X>" << endl;
//    fout.precision(12);
    fout.precision(numeric_limits<double>::digits10 + 1); // maximum precision
    for(size_t k{}; k < limit ; ++k){
        auto t = (k+1)/double(N);
        fout << t
             << "\t" << susceptibility[k]/En << endl;
    }
    fout.close();


}

/**
 * N : network size
 * n1 : number of links required for the largest cluster size to be grater than or equal to sqrt(N)
 * n2 : number of links required for the largest cluster size to be grater than or equal to N/2
 * delta = (n2 - n1)
 * power-keg graph
 * @param argc
 * @param argv
 */
void run_v7_percolation_delta_powder_keg(int argc, char **argv) {
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
    size_t rebuild_each = 100; // cycle

    auto* net = new NetworkBA_v7(m);
//    auto* net = new NetworkMDA_v7(m);
    net->setRandomState(0, true);
    net->initialize(N);
    net->clearAdjacency();
//
//      NetworkPercolation_v7 percolation(net);
//    NetworkPercolationInverted_v7 percolation(net, M);
    NetworkPercolationExplosive_v7 percolation(net, M);

    percolation.initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t stage_n1 = sqrt(nodeCount);
    size_t stage_n2 = nodeCount/2;
    cout << "stage_n1 " << stage_n1 << " stage_n2 " << stage_n2 << endl;
//    size_t limit = (stage_n1 > stage_n2) ? stage_n1 : stage_n2;
    size_t largest{};

    vector<long> n1_values, n2_values;
    for (int k{1}; k <= En; ++k) {
        bool stage_n1_flag = true;
        bool stage_n2_flag = true;

        auto t_start= chrono::_V2::system_clock::now();
        percolation.reset(k % rebuild_each == 0); // every nn step. reset the network
        size_t link_counter = 0;
        while (percolation.occupyLink()) {
            largest = percolation.largestClusterSize();
            if(stage_n1_flag && largest >= stage_n1){
                stage_n1_flag = false;
                n1_values.emplace_back(link_counter);
            }
            if(stage_n2_flag && largest >= stage_n2){
                stage_n2_flag = false;
                n2_values.emplace_back(link_counter);
                if(!stage_n1_flag) {
                    cout << "breaking at " << link_counter <<endl;
                    break;
                }
            }

//            if(!stage_n1_flag && !stage_n2_flag){
//                cout << "breaking at " << link_counter <<endl;
//                break;
//            }
            ++link_counter;
//            if (link_counter >= limit) {
//                cout << "breaking at " << link_counter <<endl;
//                break;
//            }
        }
//        entropy_jump += percolation.largestEntropyJump();
//        order_jump += percolation.largestOrderJump();

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
       << R"*(,"random_seed":)*" << net->getRandomState()
       << R"*(,"m":)*" << m
       << R"*(,"network_seed":)*" << net->get_m0()
       << R"*(,"N":)*" << N
       << R"*(,"number_of_links":)*" << linkCount
       << R"*(,"number_of_nodes":)*" << nodeCount
       << R"*(,"M":)*" << M
       << R"*(,"ensemble_size":)*" << En
       << R"*(,"En":)*" << En
       << R"*(,"datetime":")*" << tm << "\""
       << R"*(,"compile_date":")*" << __DATE__ << "\""
       << R"*(,"compile_time":")*" << __TIME__ << "\""
       << "}";



//    string filename_delta = signature + "_delta_n_" + tm + ".txt";
    string filename_delta = signature + "_delta_n_powder_keg" + tm + ".txt";
    ofstream fout(filename_delta);
    fout << '#' << ss.str() << endl;
    fout << "#  N : network size\n"
            "# n1 : number of links required for the largest cluster size to be grater than or equal to sqrt(N)\n"
            "# n2 : number of links required for the largest cluster size to be grater than or equal to N/2\n"
            "# delta = (n2 - n1)" << endl;
    fout << "#<n1>\t<n2>" << endl;
//    fout.precision(12);
    fout.precision(numeric_limits<double>::digits10 + 1); // maximum precision
    for(size_t k{}; k < n1_values.size() ; ++k){
        fout << n1_values[k] << "\t"
             << n2_values[k]
             << endl;
    }
    fout.close();

}
