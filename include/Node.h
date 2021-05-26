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
#define SENDING_PERIOD 1000
#define MAX_WAITING_TIME 3000      //3 seconds for debugging purposes, in future 0.1 s
#define CHOOSING_TIME 800
#define MILLISECONDS 1000.0
#define LEADERS_MESSAGE 0
#define ID_MESSAGE 1
#define SESSION_CONTROLLER_KILL_MSG 2
#define MSG_TYPE_POSITION 0
#define ID_POSITION 2
#define ROLE_POSITION 4
#define NONE 0
#define LEADER 1
#define VICE_LEADER 2
/// MSG:"TYPE_ID_ROLE"
/// ID_MESSAGE: ID_X
/// LEADERS_MESSAGE: ID_ROLE
/// SESSION_CONTROLLER_KILL_MSG: ID_X

/**
 * @brief class defines initialization and other operations of the communication node
 */
class Node{
private:
    int sock;
    int id;
    int role;
    struct timespec first_id_msg_time;
    struct timespec last_leader_msg_time;
    struct timespec last_vleader_msg_time;
    bool new_vleader;
    bool should_node_die;
    mutable std::mutex mutex_;

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
    bool is_vleader_absent();

    /**
     * @brief method receiving messages in a loop
     */
    void *receiver(void*);

    /**
     * @brief method sends messages in a loop
     */
    void *sender(void*);
};

#endif
