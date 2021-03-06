/**
 * @file Receiver.h
 * @author Daniel Chmielewiec
 * @brief contains implementation of class Receiver
 * @date 17-05-2021
 * @project long live king
 */

#include "../include/Receiver.h"

Receiver::Receiver(int sock, int port, struct in6_addr ip6_addr){
    this->sock = sock;

    bzero(&this->addr, sizeof(this->addr));
    this->addr.sin6_family = AF_INET6;
    this->addr.sin6_port = htons(port);
    this->addr.sin6_addr = ip6_addr;
}

void Receiver::init(){
    if (bind(sock, (struct sockaddr *) &addr, sizeof addr) != 0) {
        perror("bind error");       // throw std::runtime_error ?
        exit(1);
    }

    struct ipv6_mreq mreq;
    inet_pton(AF_INET6, LOOPBACK_ADDR, &(mreq.ipv6mr_multiaddr));
    mreq.ipv6mr_interface = 0;

    if (setsockopt(this->sock, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq, sizeof (mreq)) < 0) {
        perror("setsockopt IPV6_ADD_MEMBERSHIP error");     // throw std::runtime_error ?
        exit(1);
    }
}

void Receiver::receive(char* buf, int size){
    int received = read(sock, buf, size);
    if(received == -1){
        perror("read error");
        exit(1);
    }
    if(received != MAX_MSG_SIZE){
        perror("wrong message size received");
        exit(1);
    }
}
