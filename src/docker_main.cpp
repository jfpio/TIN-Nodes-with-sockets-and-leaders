/**
 * @file docker_main.cpp
 * @author Jan Piotrowski
 * @brief contains docker implementation
 * @date 31-05-2021
 * @project long live king
 */


#include <iostream>
#include <fstream>
#include "../include/SessionController.h"
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        int next_id = atoi(argv[1]);
        int role = atoi(argv[2]);

        Node new_node(next_id, role);
        new_node.init();
    }
    return -1;
}
