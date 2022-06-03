/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <csignal>
#include <pthread.h>

#define PORT "9034" // the port client will be connecting to 

#define MAXDATASIZE 1024 // max number of bytes we can get at once 
int sockfd, numbytes;
std::string nickname;
pthread_t t_id;

// get sockaddr, IPv4 or IPv6:
void *incoming_addr(struct sockaddr *sock_addr)
{
    if (sock_addr->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sock_addr)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sock_addr)->sin6_addr);
}

bool str_comp(std::string a, std::string b){
    if (a.size()!= b.size()){
        return false;
    }
    for (int i = 0; i < a.size(); i++){
        if (a.at(i) != b.at(i)){
            return false;
        }
    }
    return true;
}


void signal_handler(int signal)
{
    pthread_cancel(t_id);
    std::string msg = "ctrl_c";
    int is_sent = send(sockfd, msg.c_str(),msg.length(), 0);
    if (is_sent == -1)
    {
        perror("send commend error\n");
        exit(1);
    }
    close(sockfd);
    exit(0);
}


void* recv_thread_handler(void* arg)
{
    int received;
    char txt[1024];

    while(1){
        memset(txt,0,sizeof(txt));
        received = recv(sockfd,txt,sizeof(txt),0);
        if (received == -1)
        {
            perror("recv error - commend response\n");
        }
        if (strcmp(txt,"server_ctrl_c") == 0)
        {
            pthread_cancel(t_id);
            close(sockfd);
            exit(0);
        }
        else
        {
            std::cout << txt << '\n';
        }
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    std::signal(SIGINT,signal_handler);
    char buf[MAXDATASIZE];
    memset(buf,0,MAXDATASIZE);

    std::cout<< "Whats your name?\n";
    fflush(stdout);
    std::cin.getline(buf,MAXDATASIZE);
    nickname.clear();
    nickname = buf;
    nickname.append("# ");
    memset(buf,0,MAXDATASIZE);

    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, incoming_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    
    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);    

    int err = pthread_create(&t_id, NULL, recv_thread_handler, NULL);
    if (err)
    {
        std::cout << "Pthread create error!";
    }

    int is_sent;
    char txt[1024];
    std::string msg;

    while(1){
        msg.clear();
        memset(txt,0,sizeof(txt));
        msg.append(nickname);
        std::cin.getline(txt,sizeof(txt));
        msg.append(txt);

        is_sent = send(sockfd, msg.c_str(),msg.length(), 0);
        if (is_sent == -1)
        {
            perror("send commend error\n");
            exit(1);
        }
    }

    close(sockfd);

    return 0;
}
