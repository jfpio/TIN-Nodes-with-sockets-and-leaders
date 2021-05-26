#include <iostream>
#include <fstream>
#include "../include/SessionController.h"

const std::string LOG_FILE = "log.txt";

int main() {
    Logger::getInstance().setLogFile(LOG_FILE);
    CLI::run();

    return 0;
}