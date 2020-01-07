//
// Created by shahnoor on 11/10/19.
//

#include "operation.h"
#include "../nets/BA/network_BA_v7.h"
#include "../util/time_tracking.h"

void PerformPercolation::set_network(string name, int m, int N) {
    if(name == "BA"){
        net = new NetworkBA_v7(m);

    }
    m_val = m;
    N_size = N;
    if(net != nullptr) {
        net->initialize(N);
    }
}

void PerformPercolation::set_network(string name, double p, int N) {
    if(name == "ER"){
        net = new NetworkER_v7(p);

    }
    p_val = p;
    N_size = N;
    if(net != nullptr) {
        net->initialize(N);
    }
}

void PerformPercolation::set_percolation(string name, int M) {
    if(name == "explosive"){
        percolation = new NetworkPercolationExplosive_v7(net, M);
    }
    M_val = M;

}

void PerformPercolation::run(int ensemble_size, int reset_interval) {
    percolation->initialize();
    size_t linkCount = net->getLinkCount();
    size_t nodeCount = net->getNodeCount();
    cout << nodeCount << ", " << linkCount << ", " << endl;
    size_t limit = nodeCount * 2;
//    percolation.viewNetwork();
//    percolation.viewListOfLinkIndices();

    entropy.resize(linkCount), order_param.resize(linkCount); // entropy and order parameter
    for (int k{1}; k <= ensemble_size; ++k) {
        auto t_start= chrono::_V2::system_clock::now();
//        net.viewListOfLinkIndices();
        percolation->reset(k % reset_interval == 0); // every 100 step. reset the network
//        cout << net->getLinkCount() << endl;
        size_t i{};
//        percolation.viewClusters();
//        percolation.viewListOfLinkIndices();
//        percolation.viewNetwork();
//        cout << "entering to while" << endl;
        while (percolation->occupyLink()) {
//            cout << "i " << i  << endl;
            entropy[i] += percolation->entropy();
            order_param[i] += percolation->largestClusterSize();
            percolation->jump();
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
        entropy_jump += percolation->largestEntropyJump();
        order_jump += percolation->largestOrderJump();

//        cout << entropy_jump[k] << " at " << entropy_jump_pc[k] << endl;
        auto t_end= chrono::_V2::system_clock::now();
        chrono::duration<double> drtion = t_end - t_start;
        cout << "iteration " << k << " : time elapsed " << drtion.count() << " sec" << endl;
    }
    tm = currentTime();
//    string signature = percolation.get_signature();
    signature = net->get_signature_array()[0] +"_"+ percolation->getClassName()
                       + "_N_" + to_string(nodeCount)
                       + "_m_" + to_string(m_val)
                       + "_M_" + to_string(M_val);
}

void PerformPercolation::write_data() {

//    stringstream ss;
//    ss << "{"
//       << R"*("signature":")*" << signature << "\""
//       << R"*(,"network_class":")*" << net->getClassName() << "\""
//       << R"*(,"percolation_class":")*" << percolation->getClassName() << "\""
//       << R"*(,"m":)*" << m
//       << R"*(,"N":)*" << N
//       << R"*(,"number_of_links":)*" << linkCount
//       << R"*(,"number_of_nodes":)*" << nodeCount
//       << R"*(,"M":)*" << M
//       << R"*(,"ensemble_size":)*" << En
//       << R"*(,"date":")*" << tm << "\""
//       << "}";
//
//
//
//    string filename = signature + "_entropy-order_" + tm + ".txt";
//    ofstream fout(filename);
//    fout << '#' << ss.str() << endl;
//    fout << "# t=relative link density" << endl;
//    fout << "#<t>\t<H>\t<P>" << endl;
//    for(size_t k{}; k < limit ; ++k){
//        auto t = (k+1)/double(N);
//        fout << t
//             << "\t" << entropy[k]/En
//             << "\t" << order_param[k]/(En*double(N)) << endl;
//    }
//    fout.close();


}

void PerformPercolation::write_jump_data() {
//    string filename_jump = signature + "_jump_" + tm + ".txt";
//
//    ofstream fout_jump(filename_jump);
//    fout_jump << '#' << ss.str() << endl;
//    fout_jump << "#<m><N><M><En><largest entropy jump><largest order parameter jump>" << endl;
//    fout_jump << m << "\t" << N << "\t" << M << "\t" << En
//              << "\t" << abs(entropy_jump)/En << '\t' << order_jump/En << endl;
//    fout_jump.close();
}
