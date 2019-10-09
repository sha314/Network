//
// Created by shahnoor on 10/8/19.
//

#include <fstream>
#include "run_network_ba.h"
#include "../percolation/network_percolation_v3.h"
#include "../percolation/explosive/network_percolation_explosive_v3.h"
#include "../util/time_tracking.h"
#include "../percolation/explosive/network_percolation_explosive_v5.h"

using namespace std;


/**
 * Date : 2019.05.17
 * @param argc
 * @param argv
 */
void demarcationLine(int argc, char **argv){
    int m = atoi(argv[1]);
    int N = atoi(argv[2]);
    int M = atoi(argv[3]);

//   NetworkBApercolationExplosive_v3 net(m, m, N, M);
//    NetworkBApercolation_v3 net(m, m, N);

    NetworkBApercolationExplosive_v5 net(m, m, N, M);
//    NetworkBApercolation_v5 net(m, m, N);

    net.setRandomState(0, true);
    net.initializeNetwork();

    string signature = net.get_signature();
    string filename = signature + "demarcationLine-" + currentTime() + ".txt";



//    double t;
    double P{}, P_old{}, dP;
    bool c{false};


    vector<double> dP_list(net.linkCount()), color_list(net.linkCount());

    size_t i{1};

    net.occupyLink();
    P_old = net.largestClusterSize();
//    t = net.occupationProbability();
//    fout << t << '\t' << 0 << '\t' << c << endl;

    while (net.occupyLink()){
//        cout << " ************* **************** *************" << endl;

//        t = net.relativeLinkDensity();
        P = net.largestClusterSize();

        dP = abs(P- P_old);

        P_old = P;
        c = net.isSelfClusterJump();

        dP_list[i] = dP;
        color_list[i] = c;
        ++i;

//        fout << t << '\t' << P << '\t' << c << endl; // takes ~80% of the total time
//        net.viewClusterExtended();
//        net.viewCluster();
    }

    ofstream fout(filename);
    stringstream ss;
    ss   << "{\"signature\":" << "\"" << signature << "\""
         << R"*(,"class":")*" << net.getClassName() << "\""
         << ",\"m\":" << m << ",\"N\":" << N
         << ",\"M\":" << M  // in case of explosive percolation
         << ",\"number_of_links\":" << net.linkCount()
         << ",\"number_of_nodes\":" << net.nodeCount()
         << R"*(,"date":")*" << currentTime() << "\""
         << ",\"cols\":" << "[\"t\", \"dP\", \"color\"]" << "}";


    fout << "#" << ss.str() << endl;
    fout << "#n = number of occupied links" << endl;
    fout << "#t = n / N" << endl;
    fout << "#t = relative occupation probability" << endl;
    fout << "#cluster size = number of nodes in the cluster" << endl;
    fout << "#P = order parameter = largest cluster size / N" << endl;
    fout << "#N = network size = number of nodes in it" << endl;
    fout << "#S_max = largest cluster size" << endl;
    fout << "#dS = largest clusteter jump" << endl;
    fout << "#c = 0 or 1 (different cluster jump or self cluster jump)"<< endl;
    fout << "#<t>\t<dP>\t<c>" << endl;
    size_t sz = net.linkCount();
    size_t Norm = net.nodeCount();
    for(size_t i{}; i < sz; ++i){
        if (dP_list[i] != 0) {
            fout << (i+1)/double(Norm) << '\t' << dP_list[i] << '\t' << color_list[i] << endl;
        }
    }

    fout.close();
}


void run_BA_percolation(int argc, char **argv) {
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


    //    NetworkBApercolation_v3 net(m, m, N);
//    NetworkBApercolationExplosive_v3 net(m, m, N, M);
//   NetworkBApercolationExplosive_Inverted_v3 net(m, m, N, M);

//    NetworkBApercolation_v5 net(m, m, N);
    NetworkBApercolationExplosive_v5 net(m, m, N, M);
//    NetworkBApercolationExplosive_Inverted_v5 net(m,m,N,M);

    net.setRandomState(0, true);
    net.initializeNetwork();

    size_t linkCount = net.linkCount();
    size_t nodeCount = net.nodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
//    net.viewNetwork();
//    net.viewListOfLinkIndices();
    double entropy_jump{}, order_jump{};
    vector<double> entropy(linkCount), order_param(linkCount); // entropy and order parameter
    for (size_t k{0}; k < ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        net.reset(k%25 == 0); // every 25 step. reset the network

        size_t i{};
//        net.viewClusters();
//        net.viewListOfLinkIndices();
//        net.viewNetwork();
//        cout << "entering to while" << endl;
        while (net.occupyLink()) {
//            cout << "i " << i  << endl;
            entropy[i] += net.entropy();
            order_param[i] += net.largestClusterSize();
//            net.viewClusters();
//            cout << net.entropy_v1()  << "\t";
//            cout << net.entropy_v2() << endl;
//            cout << net.largestClusterSize() << endl;
            net.jump();
//            _network_frame.viewClusterExtended();
            ++i;
//            if (i == 7) break;
        }
        entropy_jump += net.largestEntropyJump();
        order_jump += net.largestOrderJump();

//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }

    auto tm = currentTime();
    string signature = net.get_signature();
    string filename_jump = signature + "_entropy_jump_" + tm;
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
    fout_jump << "#<m><N><M><En><largest entropy jump><larget order parameter jump>" << endl;
    for(size_t k{}; k < ensemble_size ; ++k){
        fout_jump << m << "\t" << N << "\t" << M << "\t" << ensemble_size
                  << "\t" << entropy_jump/ensemble_size << '\t' << order_jump/ensemble_size << endl;
    }
    fout_jump.close();


    string filename = signature + "_entropy-order_" + tm;
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


void clusterSizeDistribution(int argc, char **argv) {
//    size_t m0 = size_t(atoi(argv[1]));
    if (argc < 4) {
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

    NetworkBApercolation_v3 net(m*2, m, N);
    net.setRandomState(0, true);

    vector<double> csd(10);
    double t;
    for (size_t en{}; en < En; ++en) {
        auto t0 = std::chrono::system_clock::now();
        net.reset(en%50 == 0);

        while(net.occupyLink()){
            t = net.relativeLinkDensity();
            if (t  > 0.5){
                auto tmp = net.clusterSizeDistribution();
                if(csd.size() < tmp.size()){
                    csd.resize(tmp.size());
                }
                for(size_t i{}; i < tmp.size(); ++i){
                    csd[i] += tmp[i];
                }
                break;
            }
        }


//        cout << "line " << __LINE__ << endl;
        auto t1 = std::chrono::system_clock::now();
        std::chrono::duration<double> drtion = t1 - t0;
        cout << "Iteration " << en << " : time " << drtion.count() << " sec" << endl;
    }

    string tm = currentTime();
    string filename = net.get_signature() + "N_" + to_string(N)
                      + "-cluster-size-distribution-" + tm + ".txt";
    stringstream ss;
    ss   << "{"
         << R"("signature":")" << net.get_signature() << "\""
         << R"(,"m":)" << m
         << R"(,"N":)" << net.getNetworkSize()
         << R"(,"number_of_links":)" << net.linkCount()
         << R"(,"number_of_nodes":)" << net.nodeCount()
         //         << R"(,"M":)" << M
         << R"(,"ensemble_size":)" << En
         << R"(,"random_state":)" << net.getRandomState()
         << R"(,"time":")" << tm << "\""
         << "}";


    ofstream fout(filename);
    fout << "#" << ss.str() << endl;
    fout << "#{\"m0\":" << net.get_m0()
         << ",\"m\":" << net.get_m()
         << ",\"N\":" << N
         << ",\"En\":" << En
         << "}" << endl;

    for (size_t i{0}; i < csd.size(); ++i) {
        if(csd[i] == 0) continue;
        fout << i << "\t" << csd[i]/(En * net.getNetworkSize()) << endl;
    }

    // normalization
    fout.close();
}