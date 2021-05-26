//
// Created by Daniel Chmielewiec on 05.05.2021.
//


#include "../include/SessionController.h"


SessionController::SessionController(){
    this->next_id = 0;
    this->stop = false;
    this->rcvr_t = std::thread(&SessionController::receiver, this, (void*) nullptr);
    rcvr_t.detach();
}

SessionController::~SessionController(){}

void SessionController::cleanup() {
    std::stringstream msg;
    for(const auto& node: nodes){
        msg << "node " << node.id << " left";
        kill(node.pid, SIGKILL);
        Logger::getInstance().log(msg);
    }
    stop_receiver();
}

void SessionController::add_node(int role){
    if(nodes.size() == MAX_NODES) {
        CLI::display("Can't add more nodes"); // throw std::logic_error ?
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
        exit(0);
    } else{
        struct Node_info new_node_info;
        new_node_info.pid = c_pid;
        new_node_info.id = next_id;
        new_node_info.role = role;
        std::lock_guard guard(nodes_mutex);
        nodes.push_back(new_node_info);
        std::stringstream msg;
        msg << "Node with id = " << next_id << " and role " << role << " added" << std::endl;
        CLI::display(msg);
        ++next_id;
    }
}

void SessionController::delete_node(int id){
    std::stringstream log_msg;
    std::lock_guard guard(nodes_mutex);
    char msg[MAX_MSG_SIZE];
    for(int i = 0; i < nodes.size(); i ++){
        if(nodes.at(i).id == id){
            Sender sender(sock, PORT);
            sprintf(msg, "%d %d", SESSION_CONTROLLER_KILL_MSG, id);
            sender.send(msg, sizeof msg);

            log_msg << "node " << nodes.at(i).id << " left";
            //kill(nodes.at(i).pid, SIGKILL);
            nodes.erase(nodes.begin() + i);
            Logger::getInstance().log(log_msg);
            log_msg << "Node deleted";
            CLI::display(log_msg);
            return;
        }
    }
    log_msg << "There is not a node with id = " << id;
    CLI::display(log_msg);
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
        } else if (atoi(buf + MSG_TYPE_POSITION) == SESSION_CONTROLLER_KILL_MSG && atoi(buf + ID_POSITION) == -1) {
            stop = true;
        }
    }
}

void SessionController::stop_receiver() const{
    Sender sender(sock, PORT);
    char msg[MAX_MSG_SIZE];

    sprintf(msg, "%d %d", SESSION_CONTROLLER_KILL_MSG, -1);
    sender.send(msg, sizeof msg);
}

void SessionController::setRole(int id, int role){
    for(int i = 0; i < nodes.size(); i++){
        if(nodes.at(i).id == id){
            nodes.at(i).role = role;
        }
    }
}