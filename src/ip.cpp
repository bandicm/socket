#include "../lib/ip.hpp"


bool isIPAddress(const string address) {
    char buf[16];
    return isIPv4(address) || isIPv6(address) ;
}

bool isIPv4 (const string address) {
    char buf[16];
    return inet_pton(AF_INET,  address.c_str(), buf);
}

bool isIPv6 (const string address) {
    char buf[16];
    return inet_pton(AF_INET6,  address.c_str(), buf);
}

string ipFromDomain(const string domain) {  
    struct hostent *host_name;     
    struct in_addr **ipaddress;     
    int i;   
    char ip_address[INET6_ADDRSTRLEN];

    if((host_name = gethostbyname(domain.c_str())) == NULL) { 
        printf ("[ERROR] IP Address Not Found from domain!");
    } 

    ipaddress = (struct in_addr **) host_name->h_addr_list;
    for(i = 0; ipaddress[i] != NULL; i++) {   
        strcpy(ip_address, inet_ntoa(*ipaddress[i]));
    }

    return (string) ip_address;
}