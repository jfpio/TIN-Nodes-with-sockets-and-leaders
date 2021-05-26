//
// Created by Daniel Chmielewiec on 05.05.2021.
//


#include "../include/SessionController.h"


SessionController::SessionController(){
    this->next_id = 0;
    this->stop = false;
    this->rcvr_t = std::thread(&SessionController::receiver, this, (void*) nullptr);
}

SessionController::~SessionController(){
    std::stringstream msg;
    for(const auto& node: nodes){
        msg << "node " << node.id << " left";
        kill(node.pid, SIGKILL);
        Logger::getInstance().log(msg);
    }
    stop_receiver();
    rcvr_t.join();
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
        std::lock_guard guard(nodes_mutex);
        nodes.push_back(new_node_info);
        std::stringstream msg;
        msg << "Node with id = " << next_id << " and role " << role << " added" << std::endl;
        cli.display(msg);
        ++next_id;
    }
}

void SessionController::delete_node(int id, const CommandLineInterface& cli){
    std::stringstream msg;
    std::lock_guard guard(nodes_mutex);
    for(int i = 0; i < nodes.size(); i ++){
        if(nodes.at(i).id == id){
            msg << "node " << nodes.at(i).id << " left";
            kill(nodes.at(i).pid, SIGKILL);
            nodes.erase(nodes.begin() + i);
            Logger::getInstance().log(msg);
            msg << "Node deleted";
            cli.display(msg);
            return;
        }
    }
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

void* SessionController::receiver(void* arg){
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        perror("socket error");
        exit(1);
    }

    int opt = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0){
        perror("setsockopt SO_REUSEADDR error");        // throw std::runtime_error ?
        exit(1);
    }

    Receiver receiver(sock, PORT, IN6ADDR_ANY_INIT);
    receiver.init();
    char buf[MAX_MSG_SIZE];

    while(!stop){
        receiver.receive(buf, sizeof buf);
        if(atoi(buf + MSG_TYPE_POSITION) == LEADERS_MESSAGE){
            setRole(atoi(buf + ID_POSITION), atoi(buf + ROLE_POSITION));
        }
    }
}

void SessionController::stop_receiver(){
    Sender sender(sock, PORT);
    char msg[MAX_MSG_SIZE];

    stop = true;
    msg[0] = 'q';
    sender.send(msg, sizeof msg);
}

void SessionController::setRole(int id, int role){
    for(int i = 0; i < nodes.size(); i++){
        if(nodes.at(i).id == id){
            nodes.at(i).role = role;
        }
    }
}