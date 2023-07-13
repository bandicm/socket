#ifndef TCP_SCK
#define TCP_SCK

#include <iostream>
#include <string>
#include <vector>
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

    // dok god živi server žive i klijenti - klijetni moraju biti dio servera
    // omogućiti enkripciju i na nivou servera


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
    // zajedničke
    int conn; // mijenja sock
    struct sockaddr_in addr;
    SSL* ssl = NULL;
    // klijent sa serverom
    string ipv4;
    string ipv6;
    
    // konstruktor za klijente bez servera
    client (const string address, const ushort port, const uint timeout = 100, SSL_CTX* securefds = NULL);
    // konstruktor za klijente sa serverom
    client (const server *_srv, const uint timeout = 100, SSL_CTX* securefds = NULL);
    ~client ();
    bool push (const string msg);
    string pull (size_t byte_limit = 1024);
};

#endif