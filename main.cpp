#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 5000
#define LOOPBACK_ADDR "ff02::1"

int sock;

void *receiver(void *arg){
    struct sockaddr_in6 addr;
    bzero(&addr, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(PORT);
    addr.sin6_addr= IN6ADDR_ANY_INIT;

    if (bind(sock, (struct sockaddr *) &addr, sizeof addr) != 0) {
        perror("bind error");
        exit(1);
    }

    struct ipv6_mreq mreq;
    inet_pton(AF_INET6, LOOPBACK_ADDR, &(mreq.ipv6mr_multiaddr));
    mreq.ipv6mr_interface = 0;

    if (setsockopt(sock, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq, sizeof (mreq)) < 0) {
        perror("setsockopt IPV6_ADD_MEMBERSHIP error");
        exit(1);
    }

    char buf[1024];
    do {
        if (read(sock, buf, 1024) == -1) {
            perror("read error");
            exit(1);
        }

        std::cout << "RECEIVED: " << buf << std::endl;
    }while(strlen(buf) != 1 || buf[0] != 'q');
}

void *sender(void *arg){
    struct sockaddr_in6 addr;
    bzero(&addr, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(PORT);
    inet_pton(AF_INET6, LOOPBACK_ADDR, &(addr.sin6_addr));

    std::string msg;
    do{
        std::cin>>msg;
        const char* msg_c = msg.c_str();

        if(sendto(sock, msg_c, sizeof msg_c, 0, (struct sockaddr *) &addr, sizeof addr) == -1) {
            perror("sendto error");
            exit(1);
        }
    }while(msg[0] != 'q' || msg.length() != 1);

}

int main() {
    pthread_t recv_id, snd_id;

    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        perror("socket error");
        return 1;
    }

    int opt;

    /*opt = 0;
    if(setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &opt, sizeof(opt)) < 0) {
        perror("setsockopt IP_MULTICAST_LOOP error");
        exit(1);
    }*/

    opt = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0){
        perror("setsockopt SO_REUSEADDR error");
        return 1;
    }
    std::cout<<"Type sth to send, 'q' to exit"<<std::endl;

    if(pthread_create(&recv_id, nullptr, &receiver, nullptr) < 0){
        perror("receiver pthread_create error");
        return 1;
    }

    if(pthread_create(&snd_id, nullptr, &sender, nullptr) < 0){
        perror("sender pthread_create error");
        return 1;
    }

    pthread_join(snd_id, nullptr);
    pthread_join(recv_id, nullptr);

    close(sock);

    return 0;
}