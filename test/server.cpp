#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

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

        // cout << "init server " << endl;
        // server myserver(5000, 100);
        // cout << "init client " << endl;
        // myserver.async(8, [](client &cli, mutex &io) {
        //     cout << "Klijent " << cli.ipv4 << endl;
        //     string fromclient = cli.pull();
        //     io.lock();
        //     cout << "S klijenta " << fromclient << endl;
        //     io.unlock();
        //     // fromclient += teststr;
        //     cli.push(fromclient);
        // }, 200);

        server myserver(7000, 100);
        // myserver.asyncPool(10, [](client &cli) {
        //     cout << "Klijent " << cli.ipv4 << endl;
        //     string fromclient = cli.pull();
        //     // io.lock();
        //     cout << "S klijenta " << fromclient << endl;
        //     // io.unlock();
        //     // fromclient += teststr;
        //     usleep(10000);
        //     cli.push(fromclient);
        // }, 200);



        // string teststr = " Idemooo";

        myserver.async(4, [](client &cli, mutex &io) { 
            while (true) {
                string fromclient = cli.pull();
                cout << "> " << fromclient << endl;
                // fromclient += teststr;
                cli.push(fromclient);
            }
        });




    }
    catch(const string err) {
        cout << err << endl;
    }
    return 0;
}