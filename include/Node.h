//
// Created by Daniel Chmielewiec on 05.05.2021.
//

#ifndef TIN_LONG_LIVE_THE_KING_NODE_H
#define TIN_LONG_LIVE_THE_KING_NODE_H

#include <mutex>
#include <fstream>

#define PORT 6000
#define LOOPBACK_ADDR "ff02::1"
#define NONE 0
#define LEADER 1
#define VICE_LEADER 2
#define LOG_FILE "log.txt"

class Node{
private:
    int sock;
    int id;
    int role;
    std::mutex out_mtx;
    std::ofstream out;
public:
    Node(int);
    void init();
    void *receiver(void*);
    void *sender(void*);
};

#endif
