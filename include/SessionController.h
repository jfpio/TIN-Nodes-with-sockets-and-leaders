/**
 * @file SessionController.h
 * @author Daniel Chmielewiec
 * @brief contains definition of SessionController class
 * @date 05-05-2021
 */

#ifndef TIN_LONG_LIVE_THE_KING_SESSIONCONTROLLER_H
#define TIN_LONG_LIVE_THE_KING_SESSIONCONTROLLER_H

#include <vector>
#include <unistd.h>
#include <algorithm>
#include <csignal>
#include "Node.h"
#include "CommandLineInterface.h"

#define MAX_NODES 10

class CommandLineInterface;

/**
 * @brief keeps information about node
 */
struct Node_info{
    pid_t pid;
    int id;
    int role;
};

/**
 * @brief class creates session and provides adding, removing and listing nodes
 */
class SessionController{
private:
    std::vector<Node_info> nodes; // why not use std::map?
    int next_id;
    std::thread rcvr_t;
    mutable std::mutex nodes_mutex;
    bool stop;
    int sock;

    SessionController();
public:
    SessionController& operator=(const SessionController&) = delete;
    SessionController(const SessionController&) = delete;
    ~SessionController();

    /**
     * @brief only way to get access to SessionController object. usage: SessionController::getInstance()
     * @return SessionController& instance
     */
    static SessionController& getInstance();

    /**
     * @return node's information vector
     */
    const std::vector<Node_info>& getNodes() const;

    /**
    * @return node's information
    */
    Node_info getNodeById(int id);

    /**
     * @brief method adds new node
     * @param role
     */
    void add_node(int role);

    /**
     * @brief method deletes chosen node
     * @param id
     */
    void delete_node(int id);

    /**
     * @brief method receives messages in a loop
     */
    void* receiver(void*);

    /**
     * @brief method stops receiver
     */
    void stop_receiver() const;

    /**
     * @brief method updates role
     * @param id and role to set
     */
    void setRole(int, int);

    void cleanup();
};

#endif