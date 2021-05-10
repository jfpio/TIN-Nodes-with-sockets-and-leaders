//
// Created by Daniel Chmielewiec on 05.05.2021.
//

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
struct Node_info{
    pid_t pid;
    int id;
    int role;
};

/// SINGLETON
class SessionController{
private:
    std::vector<Node_info> nodes; // why not use std::map?
    int next_id; // factory?

    SessionController();
public:
    SessionController& operator=(const SessionController&) = delete;
    SessionController(const SessionController&) = delete;
    ~SessionController();

    static SessionController& getInstance();
    const std::vector<Node_info>& getNodes() const;
    void add_node(const CommandLineInterface& cli);
    void delete_node(int, const CommandLineInterface& cli);
};

#endif