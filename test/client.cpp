#include <iostream>

#include "../lib/tcp_socket.hpp"

using namespace std;

int main() {

    client myserver("localhost", 5000);
    string sends = "Hello wld!";
    cout << myserver.tell(sends) << " " << sends.length() << endl;
    cout << myserver.obey();

    return 0;
}