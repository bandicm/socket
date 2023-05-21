#ifndef TCP_SCK
#define TCP_SCK

#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "ip.hpp"

using namespace std;

class server {
    public:
    int sock;
    struct sockaddr_in addr;

    server (const ushort port, const uint limit = 1000);
    ~server ();

    
};


class client {
    public:
    int sock;
    struct sockaddr_in addr;

    client (const string address, const ushort port);
    ~client ();
    bool tell (const string msg);
    string obey (size_t byte_limit = 1024);
};


class comming {
    public:
    const server *srv;
    int conn;
    string ipv4;
    string ipv6;

    comming(const server *_srv, const uint timeout);
    ~comming();
    bool tell (const string msg);
    string obey (size_t byte_limit = 1024);

};




#endif