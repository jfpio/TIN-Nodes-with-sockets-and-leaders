//
// Created by Daniel Chmielewiec on 05.05.2021.
//

#ifndef TIN_LONG_LIVE_THE_KING_SESSION_CTRL_H
#define TIN_LONG_LIVE_THE_KING_SESSION_CTRL_H

#include <vector>
#include "Node.h"

#define MAX_NODES 10

struct Node_info{
    pid_t pid;
    int id;
    int role;
};

class Session_ctrl{
private:
    std::vector<Node_info> nodes;
    int next_id;
public:
    Session_ctrl();
    ~Session_ctrl();
    void start();
    void show_menu();
    void add_node();
    void delete_node(int);
    void list_nodes();
};

#endif