//
// Created by Daniel Chmielewiec on 05.05.2021.
//


#include "../include/SessionController.h"


SessionController::SessionController(){
    this->next_id = 0;
}

SessionController::~SessionController(){
    std::stringstream msg;
    for(const auto& node: nodes){
        msg << "node " << node.id << " left";
        kill(node.pid, SIGKILL);
        Logger::getInstance().log(msg);
    }
}

void SessionController::add_node(int role, const CommandLineInterface& cli){
    if(nodes.size() == MAX_NODES) {
        cli.display("Can't add more nodes"); // throw std::logic_error ?
        return;
    }

    pid_t c_pid = fork();
    if(c_pid < 0){
        perror("fork error"); // throw std::runtime_error ?
        exit(1);
    }

    if(c_pid == 0) {
        Node new_node(next_id, role);
        new_node.init();
    } else{
        struct Node_info new_node_info;
        new_node_info.pid = c_pid;
        new_node_info.id = next_id;
        new_node_info.role = role;
        nodes.push_back(new_node_info);
        std::stringstream msg;
        msg << "Node with id = " << next_id << " and role " << role << " added" << std::endl;
        cli.display(msg);
        ++next_id;
    }
}

void SessionController::delete_node(int id, const CommandLineInterface& cli){
    bool deleted = false;
    std::stringstream msg;
    for(int i = 0; i < nodes.size(); i ++){
        if(nodes.at(i).id == id){
            msg << "node " << nodes.at(i).id << " left";

            kill(nodes.at(i).pid, SIGKILL);
            nodes.erase(nodes.begin() + i);
            deleted = true;
            Logger::getInstance().log(msg);
            break;
        }
    }
    msg.str(std::string(""));

    if(deleted)
        msg << "Node deleted";
    else
        msg << "There is not a node with id = " << id;

    cli.display(msg);
}

const std::vector<Node_info>& SessionController::getNodes() const
{
    return nodes;
}

SessionController& SessionController::getInstance() {
    static SessionController instance;
    return instance;
}