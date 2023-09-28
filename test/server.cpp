#include <iostream>
#include <chrono>


#include "../lib/tcp_socket.hpp"

using namespace std;
using namespace chrono;


int main() {
    try{
        // cout << "init cert " << endl;
        // secure crypto ("../example/cert.pem", "../example/privkey.pem");
        // cout << "init server " << endl;
        // server myserver(5000, 100, crypto.fds);
        // cout << "init cert " << endl;
        // secure crypto ("../example/cert.pem", "../example/privkey.pem");
        // cout << "init server " << endl;
        // server myserver(8000, 100, crypto.fds);
        
        // cout << "init client " << endl;

        // // client myclient(&myserver, 100, crypto.fds);
        // // comming myclient(&myserver, 100);
        // myserver.accept();
        // // cout << "wait client " << myclient.ipv4 << endl;
        // cout << "wait client " << myserver.cli->ipv4 << endl;

        // string fromclient =  myserver.cli->pull();
        // // string fromclient =  myclient.pull();
        // cout << "tell client " << fromclient << endl;
        // // usleep(600*1000);
        // sleep(5);
        // myserver.cli->push(fromclient);
        // string fromclient =  myserver.cli->pull();
        // string fromclient =  myclient.pull();
        // cout << "tell client " << fromclient << endl;
        // usleep(600*1000);
        //sleep(5);
        // myserver.cli->push(fromclient);
        // myclient.push(fromclient);
        // myclient.~comming();

        // while (true) {
        //     comming myclient(&myserver, 1000);
        //     string fromclient =  myclient.obey();
        //     cout << fromclient << " " << myclient.conn << endl;
        //     cout << "Poslano: " << myclient.tell(fromclient) << "Veličin: " << fromclient.length() << endl;
        //     // myclient.~comming();
        //     cout << "IPv4 " << myclient.ipv4 << " ipv6 " << myclient.ipv6;
        // }

        // sleep(80);

        cout << "init server " << endl;
        server myserver(5000, 100);
        cout << "init client " << endl;
        myserver.async(8, [](client &cli) {
            auto t3 = high_resolution_clock::now();
            string fromclient;
            try {
                fromclient = cli.pull();
            }
            catch(const ConnectionException except) {
                if (except.isInterrupted()) {
                    throw except;
                }
                else {
                    cout << "[EXCEPT] " << except.what() << endl;
                    fromclient = except.getData();
                }
            } 
            auto t4 = high_resolution_clock::now();
            cout << "Recive : " << duration_cast<microseconds>(t4 - t3).count() << endl;
            
            cout << "> " << fromclient << endl;
            auto t5 = high_resolution_clock::now();
            cli.push(fromclient);
             auto t6 = high_resolution_clock::now();
            cout << "Response : " << duration_cast<microseconds>(t6 - t5).count() << endl;

        });

        // string teststr = " Idemooo";

        // myserver.sync([](client &cli) {
        //     cout << "Klijent " << cli.ipv4 << endl;
        //     string fromclient = cli.pull();
        //     cout << "S klijenta " << fromclient << endl;
        //     // fromclient += teststr;
        //     cli.push(fromclient);
        // });

    }
    catch(const string err) {
        cout << err << endl;
    }
    return 0;
}