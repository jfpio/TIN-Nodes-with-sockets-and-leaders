/**
 * @file Receiver.h
 * @author Daniel Chmielewiec
 * @brief contains definition of class Receiver
 * @date 17-05-2021
 * @project long live king
 */

#ifndef TIN_LONG_LIVE_THE_KING_RECEIVER_H
#define TIN_LONG_LIVE_THE_KING_RECEIVER_H

#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

#define LOOPBACK_ADDR "ff02::1"
#define MAX_MSG_SIZE 16

/**
 * @brief class provides receiving and checking messages
 */
class Receiver{
    int sock;
    struct sockaddr_in6 addr;
public:

    /**
     * @brief constructor
     * @param socket, port and host address
     */
    Receiver(int, int, struct in6_addr);

    /**
     * @brief initializes receiver
     */
    void init();

    /**
     * @brief method receives and checks single message
     * @param buffer in which method saves message and its size
     */
    void receive(char*, int);
};

#endif //TIN_LONG_LIVE_THE_KING_RECEIVER_H
