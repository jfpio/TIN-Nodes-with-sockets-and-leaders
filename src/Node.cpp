#include "../include/Node.h"

Node::Node(int id, int role){
    this->sock = 0;
    this->id = id;
    this->role = role;
    time(&last_leader_msg_time);
    time(&last_v_leader_msg_time);
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

    std::thread rcvr_t(&Node::receiver, this, (void*) nullptr);
    std::thread sndr_t(&Node::sender, this, (void*) nullptr);
    //std::thread absnc_chckr_t(&Node::absence_checker, this, (void*) nullptr);

    rcvr_t.join();
    sndr_t.join();
    //absnc_chckr_t.join();
    close(sock);
}

bool Node::is_leader_absent() {
    std::stringstream log_msg;
    time_t now;
    std::lock_guard guard(l_mutex);
    time(&now);
    if(difftime(now, last_leader_msg_time) * 1000 > MAX_WAITING_TIME){
        log_msg << "node " << id << " informs: there is not a leader";
        Logger::getInstance().log(log_msg);
        return true;
    }
    return false;
}

bool Node::is_v_leader_absent(){
    std::stringstream log_msg;
    time_t now;
    std::lock_guard guard(vl_mutex);
    time(&now);
    if(difftime(now, last_v_leader_msg_time) * 1000 > MAX_WAITING_TIME){
        log_msg << "node " << id << " informs: there is not a vice-leader";
        Logger::getInstance().log(log_msg);
        return true;
    }
    return false;
}

void *Node::receiver(void* arg){
    std::stringstream msg;
    Receiver receiver(sock, PORT, IN6ADDR_ANY_INIT);
    receiver.init();
    char buf[MAX_MSG_SIZE];
    auto leader_start = std::chrono::steady_clock::now();
    auto v_leader_start = std::chrono::steady_clock::now();
    while(true) {
        receiver.receive(buf, sizeof buf);
        if(buf[0] == LEADERS_MESSAGE){
            if(atoi(buf + ROLE_POSITION) == LEADER){                //leader's message received
                std::lock_guard guard(l_mutex);
                time(&last_leader_msg_time);
            }else{
                if(atoi(buf + ROLE_POSITION) == VICE_LEADER){       //vice-leader's message received
                    std::lock_guard guard(vl_mutex);
                    time(&last_v_leader_msg_time);
                }else {
                    perror("role in message doesn't exist");
                    exit(1);
                }
            }
        }
        if(buf[0] == ID_MESSAGE){
            //TODO
        }
        msg << "node " << id << " received: " << buf;
        Logger::getInstance().log(msg);
    }
}

void *Node::sender(void *arg){
    Sender sender(sock, PORT);
    bool leader_absent = false;
    bool v_leader_absent = false;
    std::stringstream log_msg;
    char msg[MAX_MSG_SIZE];
    char msg_type;
    while(true) {
        sleep(1);
        if(role != NONE){                               //if I have role - sending LEADERS_MESSAGE with role
            msg_type = LEADERS_MESSAGE;
            sprintf(msg, "%c %d", msg_type, role);
            sender.send(msg, sizeof msg);
            log_msg << "node " << id << " sent " << msg;
            Logger::getInstance().log(log_msg);
        }
        if(leader_absent && role == VICE_LEADER) {      //if leader absent and I am a vice-leader - I become a leader
            role = LEADER;
            log_msg <<"node" << id << " is now leader" << msg;
            Logger::getInstance().log(log_msg);
        }
        if(v_leader_absent && role != LEADER){          //if vice-leader absent and I am not a leader - sending ID_MESSAGE
            msg_type = ID_MESSAGE;
            sprintf(msg, "%c %d", msg_type, id);
            sender.send(msg, sizeof msg);
            log_msg <<"node" << id << " sent " << msg;
            Logger::getInstance().log(log_msg);
        }
        leader_absent = is_leader_absent();
        v_leader_absent = is_v_leader_absent();
    }
}

/*void *Node::absence_checker(void *arg){
    while(true){
        usleep(100000);
        is_leader_absent();
        is_v_leader_absent();
        ...
    }
}*/