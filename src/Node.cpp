#include "../include/Node.h"

Node::Node(int id, int role){
    this->sock = 0;
    this->id = id;
    this->role = role;
}

void Node::init(){
    std::stringstream msg;
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        perror("socket error");
        exit(1);
    }

    msg << "node " << id << " joined";
    Logger::getInstance().log(msg);

    int opt = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0){
        perror("setsockopt SO_REUSEADDR error");        // throw std::runtime_error ?
        exit(1);
    }

    std::thread rcvr_t(&Node::rcvr, this, (void*) nullptr);
    std::thread sndr_t(&Node::sndr, this, (void*) nullptr);

    rcvr_t.join();
    sndr_t.join();
    close(sock);
}

void *Node::rcvr(void* arg){
    std::stringstream msg;
    Receiver receiver(sock, PORT, IN6ADDR_ANY_INIT);
    receiver.init();
    char buf[1024];
    while(true) {
        receiver.receive(buf);
        msg << "node " << id << " received: " << buf;
        Logger::getInstance().log(msg);
    }
}

void *Node::sndr(void *arg){
    Sender sender(sock, PORT);
    std::stringstream log_msg;
    while(true) {
        sleep(2);
        char msg[20];
        sprintf(msg, "%d.%d", id, role);
        char type_of_message = 'a'; // TODO Mock, in future we will need other types of messages
        sender.send(msg);
        log_msg << "node " << id << " sent " << msg;
        Logger::getInstance().log(log_msg);

    }
}