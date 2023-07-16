#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {
    try{
        cout << "init cert " << endl;
        secure crypto ("../example/cert.pem", "../example/privkey.pem");
        cout << "init server " << endl;
        server myserver(5000, 100, crypto.fds);
        
        cout << "init client " << endl;

        // client myclient(&myserver, 100, crypto.fds);
        // comming myclient(&myserver, 100);
        myserver.accept();
        // cout << "wait client " << myclient.ipv4 << endl;
        cout << "wait client " << myserver.cli->ipv4 << endl;

        string fromclient =  myserver.cli->pull();
        // string fromclient =  myclient.pull();
        cout << "tell client " << fromclient << endl;
        // usleep(600*1000);
        sleep(5);
        myserver.cli->push(fromclient);
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
    }
    catch(const string err) {
        cout << err << endl;
    }
    return 0;
}