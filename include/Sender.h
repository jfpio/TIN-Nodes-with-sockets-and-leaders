/**
 * @file Sender.h
 * @author Daniel Chmielewiec
 * @brief contains definition of class Sender
 * @date 18-05-2021
 * @project long live king
 */

#ifndef TIN_LONG_LIVE_THE_KING_SENDER_H
#define TIN_LONG_LIVE_THE_KING_SENDER_H

#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

#define LOOPBACK_ADDR "ff02::1"

/**
 * @brief class provides sending messages
 */
class Sender{
private:
    int sock;
    struct sockaddr_in6 addr;
public:

    /**
     * @brief constructor
     * @param socket and port
     */
    Sender(int, int);

    /**
     * @brief sends given message
     * @param message and its size
     */
    void send(char*, int);
};

#endif //TIN_LONG_LIVE_THE_KING_SENDER_H
