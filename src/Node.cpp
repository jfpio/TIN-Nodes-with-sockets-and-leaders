#include "../include/Node.h"

Node::Node(int id, int role){
    this->sock = 0;
    this->id = id;
    this->role = role;
    this->new_vleader = false;
    this->should_node_die = false;
    clock_gettime(CLOCK_REALTIME, &last_leader_msg_time);
    clock_gettime(CLOCK_REALTIME, &last_vleader_msg_time);
    clock_gettime(CLOCK_REALTIME, &first_id_msg_time);
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

    rcvr_t.join();
    sndr_t.join();
    close(sock);
}

bool Node::is_leader_absent() {
    std::stringstream log_msg;
    struct timespec now;
    std::lock_guard guard(mutex_);
    clock_gettime(CLOCK_REALTIME, &now);
    if((now.tv_sec + now.tv_nsec*1e-9 - last_leader_msg_time.tv_sec - last_leader_msg_time.tv_nsec*1e-9) * MILLISECONDS > MAX_WAITING_TIME){
        log_msg << "node " << id << " informs: there is not a leader";
        Logger::getInstance().log(log_msg);
        return true;
    }
    return false;
}

bool Node::is_vleader_absent(){
    std::stringstream log_msg;
    struct timespec now;
    std::lock_guard guard(mutex_);
    clock_gettime(CLOCK_REALTIME, &now);
    if((now.tv_sec + now.tv_nsec*1e-9 - last_vleader_msg_time.tv_sec - last_vleader_msg_time.tv_nsec*1e-9) * MILLISECONDS> MAX_WAITING_TIME){
        log_msg << "node " << id << " informs: there is not a vice-leader";
        Logger::getInstance().log(log_msg);
        return true;
    }
    return false;
}

void *Node::receiver(void* arg){
    std::stringstream msg;
    char buf[MAX_MSG_SIZE];
    struct timespec now;
    Receiver receiver(sock, PORT, IN6ADDR_ANY_INIT);
    receiver.init();

    while(!should_node_die) {
        receiver.receive(buf, sizeof buf);
        switch(atoi(buf + MSG_TYPE_POSITION)){
            case LEADERS_MESSAGE:
                if(atoi(buf + ROLE_POSITION) == LEADER){                //leader's message received
                    if (role == LEADER && atoi(buf + ID_POSITION) != id) {          //if i am a leader and got leader msg from another node
                        msg << "node " << id << " informs there is more than one leader!";
                        Logger::getInstance().log(msg);
                        std::lock_guard guard(mutex_);
                        role = NONE;
                    } else {
                        std::lock_guard guard(mutex_);
                        clock_gettime(CLOCK_REALTIME, &last_leader_msg_time);
                    }

                }else{
                    if(atoi(buf + ROLE_POSITION) == VICE_LEADER){       //vice-leader's message received
                        if (role == VICE_LEADER && atoi(buf + ID_POSITION) != id) { // if i am a leader and got leader msg from another node
                            msg << "node " << id << " informs there is more than one vice-leader!";
                            Logger::getInstance().log(msg);
                            std::lock_guard guard(mutex_);
                            role = NONE;
                        } else {
                            std::lock_guard guard(mutex_);
                            clock_gettime(CLOCK_REALTIME, &last_vleader_msg_time);
                        }
                    }else{
                        perror("unknown role in message");
                        exit(1);
                    }
                }
                break;
            case ID_MESSAGE:                                                //id message received
                clock_gettime(CLOCK_REALTIME, &now);
                if((now.tv_sec + now.tv_nsec*1e-9 - first_id_msg_time.tv_sec - now.tv_nsec*1e-9) * MILLISECONDS > MAX_WAITING_TIME){
                    std::lock_guard guard(mutex_);
                    clock_gettime(CLOCK_REALTIME, &first_id_msg_time);
                    new_vleader = true;
                }
                if(atoi(buf + ID_POSITION) < id) {
                    std::lock_guard guard(mutex_);
                    new_vleader = false;
                }
                break;

            case SESSION_CONTROLLER_KILL_MSG:
                if (atoi(buf + ID_POSITION) == id) {
                    std::lock_guard guard(mutex_);
                    should_node_die = true;
                }
                break;
            default:
                perror("unknown message type");
                exit(1);
        }
        msg.str("");
        msg << "node " << id << " received: " << buf;
        Logger::getInstance().log(msg);
    }
}

void *Node::sender(void *arg){
    Sender sender(sock, PORT);
    bool leader_absent = false;
    bool vleader_absent = false;
    std::stringstream log_msg;
    char msg[MAX_MSG_SIZE];
    int msg_type;
    while(!should_node_die) {
        usleep(SENDING_PERIOD * MILLISECONDS);
        if(role != NONE){                               //if I have role - sending LEADERS_MESSAGE with role
            msg_type = LEADERS_MESSAGE;
            sprintf(msg, "%d %d %d", msg_type, id,  role);
            sender.send(msg, sizeof msg);
            log_msg << "node " << id << " sent " << msg;
            Logger::getInstance().log(log_msg);
        }

        leader_absent = is_leader_absent();
        vleader_absent = is_vleader_absent();

        if(leader_absent && role == VICE_LEADER) {      //if leader absent and I am a vice-leader - I become a leader
            log_msg <<"node " << id << " is now leader";
            Logger::getInstance().log(log_msg);
            std::lock_guard guard(mutex_);
            role = LEADER;
        }
        if(vleader_absent && role != LEADER){          //if vice-leader absent and I am not a leader - sending ID_MESSAGE
            msg_type = ID_MESSAGE;
            sprintf(msg, "%d %d", msg_type, id);
            sender.send(msg, sizeof msg);
            log_msg <<"node " << id << " sent " << msg;
            Logger::getInstance().log(log_msg);
            struct timespec now;
            clock_gettime(CLOCK_REALTIME, &now);
            std::lock_guard guard(mutex_);
            if(new_vleader && (now.tv_sec + now.tv_nsec*1e-9 - first_id_msg_time.tv_sec - first_id_msg_time.tv_nsec*1e-9) * MILLISECONDS > CHOOSING_TIME){
                role = VICE_LEADER;
                new_vleader = false;
                log_msg <<"node " << id << " is now new vice-leader " << msg;
                Logger::getInstance().log(log_msg);
            }
        }
    }
}