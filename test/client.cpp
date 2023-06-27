#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {

    try {

        secure crypto;
        cout << "init cert " << endl;

        client myserver("127.0.0.1", 5000, 5000, crypto.fds);
        // client myserver("localhost", 5000);
        cout << "init client " << endl;


        string sends = "Hello world!";
        cout << myserver.tell(sends) << " " << sends.length() << endl;
        cout << "wait client " << endl;

        cout << myserver.obey();

    }
    catch (const string err) {
        cout << err << endl;
    }

    return 0;
}