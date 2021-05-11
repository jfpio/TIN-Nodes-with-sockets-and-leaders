/**
 * @file CommandLineInterface.h
 * @author Kamil Jabłonowski
 * @brief contains definition of CLI class
 * @date 10-05-2021
 */

#ifndef TIN_LONG_LIVE_THE_KING_COMMANDLINEINTERFACE_H
#define TIN_LONG_LIVE_THE_KING_COMMANDLINEINTERFACE_H
#include "SessionController.h"
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

struct Node_info;

/**
 * @brief CommandLineInterface class definition
 */
class CommandLineInterface
{
public:
    CommandLineInterface() = default;

    /**
     * @brief displays menu
     */
    static void displayOpts();

    /**
     * @brief starts CLI
     */
    void run() const;

    /**
     * @brief lists running nodes
     * @param nodes
     */
    void listNodes(const std::vector<Node_info>& nodes) const;

    /**
     * @brief display info for user
     * @param msg
     */
    void display(std::stringstream& msg) const;
    void display(const std::string& msg) const;
};
#endif //TIN_LONG_LIVE_THE_KING_COMMANDLINEINTERFACE_H