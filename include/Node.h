/**
 * @file Logger.h
 * @author Daniel Chmielewiec
 * @brief contains definition of Node class
 * @date 05-05-2021
 */

#ifndef TIN_LONG_LIVE_THE_KING_NODE_H
#define TIN_LONG_LIVE_THE_KING_NODE_H

#include <sys/socket.h>
#include <thread>
#include "Logger.h"
#include "Receiver.h"
#include "Sender.h"

#define PORT 6000
#define LEADERS_MESSAGE 'a'
#define IDS_MESSAGE 'b'
#define ROLE_POSITION 4
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
    Node(int, int);

    /**
     * @brief method initializes node
     */
    void init();

    /**
     * @brief method receiving messages in a loop
     */
    void *rcvr(void*);

    /**
     * @brief method sends messages in a loop
     */
    void *sndr(void*);
};

#endif
