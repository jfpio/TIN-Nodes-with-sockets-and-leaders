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
#include <chrono>
#include "Logger.h"
#include "Receiver.h"
#include "Sender.h"

#define PORT 6000
#define MAX_WAITING_TIME 3000      //3 seconds for debugging purposes, in future 0.1 s
#define LEADERS_MESSAGE 'a'
#define ID_MESSAGE 'b'
#define ROLE_POSITION 2
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
    time_t last_leader_msg_time;
    time_t last_v_leader_msg_time;
    mutable std::mutex vl_mutex;
    mutable std::mutex l_mutex;

public:
    Node(int, int);

    /**
     * @brief method initializes node
     */
    void init();

    /**
     * @brief method checks if the leader is absent
     * @return true if absent
     */
    bool is_leader_absent();

    /**
     * @brief method checks if the vice-leader is absent
     * @return true if absent
     */
    bool is_v_leader_absent();

    /**
     * @brief method receiving messages in a loop
     */
    void *receiver(void*);

    /**
     * @brief method sends messages in a loop
     */
    void *sender(void*);

    /**
     * @brief method checks absence of leaders in a loop
     */
     //void *absence_checker(void*);        //is it necessary to check absence more often then sender sends messages?
};

#endif
