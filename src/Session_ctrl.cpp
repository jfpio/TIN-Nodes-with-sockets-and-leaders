//
// Created by Daniel Chmielewiec on 05.05.2021.
//

#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <iomanip>
#include <mutex>
#include <csignal>
#include "../include/Session_ctrl.h"


Session_ctrl::Session_ctrl(){
    this->next_id = 0;
}

Session_ctrl::~Session_ctrl(){
    std::ofstream out;
    out.open(LOG_FILE, std::ios_base::app);
    for(struct Node_info node: nodes){
        out<<time(nullptr)<<": node "<<node.id<<" left"<<std::endl;
        kill(node.pid, SIGKILL);
    }
    out.close();
}

void Session_ctrl::start(){
    int choice;
    int id_to_del;
    do {
        show_menu();
        std::cin>>choice;
        std::cin.get();
        system("clear");

        switch(choice){
            case 1:
                add_node();
                break;
            case 2:
                std::cout<<"Type node's id which you want to delete"<<std::endl;
                std::cin>>id_to_del;
                std::cin.get();
                delete_node(id_to_del);
                break;
            case 3:
                list_nodes();
                break;
            case 4:
                break;
            default:
                std::cout<<"Choose between 1-4"<<std::endl;
        }
    }while(choice != 4);
}

void Session_ctrl::show_menu(){
    system("clear");
    std::cout << "Session controller" << std::endl;
    std::cout << "1. Add node" << std::endl;
    std::cout << "2. Delete node" << std::endl;
    std::cout << "3. List active nodes" << std::endl;
    std::cout << "4. Quit" << std::endl;
}

void Session_ctrl::add_node(){
    if(nodes.size() == MAX_NODES) {
        std::cout << "Can't add more nodes" << std::endl;
        std::cin.get();
        return;
    }

    pid_t c_pid = fork();
    if(c_pid < 0){
        perror("fork error");
        exit(1);
    }

    if(c_pid == 0) {
        Node *new_node = new Node(next_id);
        new_node->init();
    } else{
        struct Node_info new_node_info;
        new_node_info.pid = c_pid;
        new_node_info.id = next_id;
        new_node_info.role = 0;
        nodes.push_back(new_node_info);
        std::cout<<"Node with id = "<< next_id << " added"<<std::endl;
        ++next_id;
    }
    std::cin.get();
}

void Session_ctrl::delete_node(int id){
    bool deleted = false;
    for(int i = 0; i < nodes.size(); i ++){
        if(nodes.at(i).id == id){
            std::ofstream out;
            out.open(LOG_FILE, std::ios_base::app);
            out<<time(nullptr)<<": node "<<nodes.at(i).id<<" left"<<std::endl;
            out.close();
            kill(nodes.at(i).pid, SIGKILL);
            nodes.erase(nodes.begin() + i);
            deleted = true;
            break;
        }
    }

    if(deleted)
        std::cout<<"Node deleted"<<std::endl;
    else
        std::cout<<"There is not a node with id = "<<id<<std::endl;
    std::cin.get();
}

void Session_ctrl::list_nodes(){
    std::cout<<"------------Active nodes------------"<<std::endl;
    std::cout<<std::setw(10)<<"Id"<<std::setw(10)<<"Role"<<std::endl;
    for(int i = 0; i < nodes.size(); i ++)
        std::cout << std::setw(10) << nodes.at(i).id << std::setw(10)<<nodes.at(i).role<<std::endl;
    std::cin.get();
}