//
// Created by Daniel Chmielewiec on 05.05.2021.
//

#ifndef TIN_LONG_LIVE_THE_KING_NODE_H
#define TIN_LONG_LIVE_THE_KING_NODE_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include "Logger.h"

#define PORT 6000
#define LOOPBACK_ADDR "ff02::1"
#define NONE 0
#define LEADER 1
#define VICE_LEADER 2


class Node{
private:
    int sock;
    int id;
    int role;

public:
    Node(int);
    void init();
    void *receiver(void*);
    void *sender(void*);
};

#endif
