#include <iostream>
#include <string>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {

    try {

        uint n = 10000;

        vector<thread> thr;
        for (uint i=0; i<n; i++) {
            thr.push_back(thread([](uint a){
                client myserver("127.0.0.1", 5000, 500);
                string sends = "Hello world " + to_string(a);
                myserver.push(sends);
                cout << myserver.pull() << endl;                
            }, i));
        }

        for (uint i=0; i<n; i++) {
            thr[i].join();
        } 

        // secure crypto;
        // cout << "init cert " << endl;

        // client myserver("127.0.0.1", 5000, 5000, crypto.fds);
        client myserver("localhost", 8000, 5000, crypto.fds);
        // client myserver("localhost", 5000);
        // cout << "init client " << endl;


        // string sends = "Hello world!";
        // cout << myserver.push(sends) << " " << sends.length() << endl;
        // cout << "wait client " << endl;

        // cout << myserver.pull();

    }
    catch (const string err) {
        cout << err << endl;
    }

    return 0;
}