/**
 * @file Logger.h
 * @author Daniel Chmielewiec
 * @brief contains definition of Node class
 * @date 05-05-2021
 */

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

/**
 * @brief class defines initialization and other operations of the communication node
 */
class Node{
private:
    int sock;
    int id;
    int role;

public:
    Node(int);

    /**
     * @brief method starting node
     */
    void init();

    /**
     * @brief method provides receiving messages
     */
    void *receiver(void*);

    /**
     * @brief method privides sending messages
     */
    void *sender(void*);
};

#endif
