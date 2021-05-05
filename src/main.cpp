#include <iostream>
#include <fstream>
#include "../include/Session_ctrl.h"

int main() {
    std::ofstream out;
    out.open(LOG_FILE, std::ofstream::out | std::ofstream::trunc);
    out.close();

    Session_ctrl session_ctrl;
    session_ctrl.start();
    return 0;
}