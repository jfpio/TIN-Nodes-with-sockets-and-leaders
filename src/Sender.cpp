#include "../include/Sender.h"

Sender::Sender(int sock, int port){
    this->sock = sock;

    bzero(&this->addr, sizeof(this->addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    inet_pton(AF_INET6, LOOPBACK_ADDR, &(this->addr.sin6_addr));
}

void Sender::send(char* msg, int size){
    if (sendto(sock, msg, size, 0, (struct sockaddr *) &addr, sizeof addr) == -1) {
        perror("sendto error");
        exit(1);
    }
}