#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <thread>
#include "../include/Node.h"

Node::Node(int id){
    this->sock = 0;
    this->id = id;
    this->role = NONE;
}

void Node::init(){
    out.open(LOG_FILE, std::ios_base::app);

    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        perror("socket error");
        exit(1);
    }

    out_mtx.lock();
    out<<time(nullptr)<<": node "<<id<<" joined"<<std::endl;
    out_mtx.unlock();

    int opt;

    /*opt = 0;
    if(setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &opt, sizeof(opt)) < 0) {
        perror("setsockopt IP_MULTICAST_LOOP error");
        exit(1);
    }*/

    opt = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0){
        perror("setsockopt SO_REUSEADDR error");
        exit(1);
    }

    std::thread rcv(&Node::receiver, this, (void*) nullptr);
    std::thread snd(&Node::sender, this, (void*) nullptr);

    rcv.join();
    snd.join();

    close(sock);
}

void *Node::receiver(void *arg){

    struct sockaddr_in6 addr;
    bzero(&addr, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(PORT);
    addr.sin6_addr= IN6ADDR_ANY_INIT;

    if (bind(sock, (struct sockaddr *) &addr, sizeof addr) != 0) {
        perror("bind error");
        exit(1);
    }

    struct ipv6_mreq mreq;
    inet_pton(AF_INET6, LOOPBACK_ADDR, &(mreq.ipv6mr_multiaddr));
    mreq.ipv6mr_interface = 0;

    if (setsockopt(this->sock, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq, sizeof (mreq)) < 0) {
        perror("setsockopt IPV6_ADD_MEMBERSHIP error");
        exit(1);
    }

    char buf[1024];
    while(true){
        if (read(sock, buf, 1024) == -1) {
            perror("read error");
            exit(1);
        }
        out_mtx.lock();
        out<< time(nullptr)<<": node "<<this->id<<" received: " << buf << std::endl;
        out_mtx.unlock();

    }
}

void *Node::sender(void *arg){

    struct sockaddr_in6 addr;
    bzero(&addr, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(PORT);
    inet_pton(AF_INET6, LOOPBACK_ADDR, &(addr.sin6_addr));

    while(true){
        char msg[10];
        sprintf(msg, "%d", id);
        sleep(id + 2);

        out_mtx.lock();
        out<<time(nullptr)<<": node "<<id<<" sent "<<id<<std::endl;
        out_mtx.unlock();

        if (sendto(sock, msg, sizeof msg, 0, (struct sockaddr *) &addr, sizeof addr) == -1) {
            perror("sendto error");
            exit(1);
        }
    }

}