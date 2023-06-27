#ifndef TCP_SCK
#define TCP_SCK

#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "ip.hpp"

using namespace std;

/**
 * Server klasa za TCP/IP soket
 * Instanca se incijalizira kada pokrećemo server
*/

class server {
    public:
    int sock;
    struct sockaddr_in addr;

    server (const ushort port, const uint limit = 1000);
    ~server ();

};

/**
 * Klasa za konstrukciju i destrukciju SSL certifikata
 * Pokreće se za server i client - (različiti konstruktori)
*/

class secure {
    public:
    SSL_CTX* fds;

    secure(); // client
    secure(const string cert, const string priv);   //server
    ~secure();

};


/**
 * Klasa za definiranje TCP soketa kod klijentskih aplikacija
*/

class client {
    public:
    int sock;
    struct sockaddr_in addr;
    SSL* ssl = NULL;

    client (const string address, const ushort port, const uint timeout = 100, SSL_CTX* securefds = NULL);
    ~client ();
    bool tell (const string msg);
    string obey (size_t byte_limit = 1024);
};


/**
 * Klasa za inicijalizaciju dolaznih veza
 * Definira se na serverskom tipu aplikacija i predstavlja identifikator klijenta
*/

class comming {
    public:
    const server *srv;
    struct sockaddr_in addr;
    int conn;
    string ipv4;
    string ipv6;
    SSL* ssl = NULL;

    comming(const server *_srv, const uint timeout = 100, SSL_CTX* securefds = NULL);
    ~comming();
    bool tell (const string msg);
    string obey (size_t byte_limit = 1024);


};




#endif