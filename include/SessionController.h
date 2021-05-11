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
    int next_id; // factory?

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
     * method adds new node
     * @param cli
     */
    void add_node(int role, const CommandLineInterface& cli);

    /**
     * method deletes chosen node
     * @param cli
     */
    void delete_node(int, const CommandLineInterface& cli);
};

#endif