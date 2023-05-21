#ifndef _IP_
#define _IP_

#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

bool isIPAddress(const string address);
bool isIPv4 (const string address);
bool isIPv6 (const string address);
string ipFromDomain(const string domain); 



#endif