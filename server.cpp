/*
** server.c -- a stream socket server demo
*/

// How to run:
// server:
// g++ -pthread -o server server.cpp queue.cpp queue.hpp active_object.hpp active_object.cpp
// client:
// 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h> // threads

#include "active_object.hpp"

#define PORT "6060"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold
#define MAXDATASIZE 1024 // max number of bytes we can get at once 
#define MAX_COMMEND_SIZE 5
int sockfd;  // listen on sock_fd, new connection on new_fd
AO* ao1;
AO* ao2;
AO* ao3;

char current_commend[1024];

void *creat_thread(void *newfd); //thread making funcation decleration 

// get sockaddr, IPv4 or IPv6:
void *incoming_addr(struct sockaddr *socket_addr)
{
    if (socket_addr->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)socket_addr)->sin_addr);
    }

    return &(((struct sockaddr_in6*)socket_addr)->sin6_addr);
}


void* f1(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    if (p->first != NULL)
    {   
        // std::cout << "f1-data = " << (char*)p->first << '\n';
        char* str = (char*)p->first;
        int len = strlen(str);
        for (int i = 0; i < len; i++)
        {
            if (!isspace(str[i]) || !isblank(str[i]))
            {
                if (str[i] >= 'a' && str[i] <= 'z')
                {
                    str[i] = (((str[i] - 'a') + 1) % 26) + 'a';
                }
                else if ((str[i] >= 'A' && str[i] <= 'Z'))
                {
                    str[i] = (((str[i] - 'A') + 1) % 26) + 'A';
                }
            }
        }
        // std::cout << "f1-ans = " << (char*)p->first << '\n';
    }
    return data;
}

void* f2(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    if (p->first != NULL)
    {   
        // std::cout << "f2-data = " << (char*)p->first << '\n';
        char* str = (char*)p->first;
        int len = strlen(str);
        for (int j = 0; j < len; j++)
        {
            str[j] = toupper(str[j]);
        }
        // std::cout << "f2-ans = " << (char*)p->first << '\n';
        // fflush(stdout);
    }
    return data;
}


void* fill_Q2(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    void* ans = NULL;
    if (p->first != NULL)
    {   
        ans = ao2->Q->enqueue(p->first,p->second);
        delete (char*)p->first;
    }
    return ans;
}


void* fill_Q3(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    void* ans = NULL;
    if (p->first != NULL)
    {   
        ans = ao3->Q->enqueue(p->first,p->second);
        delete (char*)p->first;
    }
    return ans;
}


void* print_data(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    if (p->first != NULL)
    {   
        std::cout << p->first;
    }
    return data;
}


void* send_data(void* data){
    std::pair<void*,int>* p = (std::pair<void*,int>*)data;
    if (p->first != NULL)
    {    
        std::string msg = (char*)p->first;
        int num = send(p->second , msg.c_str(), msg.length(), 0);
        if (num == -1)
        {
            perror("Send eror\n");
        }
        delete (char*)p->first;
        delete p;
    }
    return data;
}


void *creat_thread(void *newfd) {
    int byteslen;
    int new_fd = *(int*)newfd;  
    std::string msg = "You are connected now!\n";
    if (send(new_fd, msg.c_str() , msg.length(), 0) == -1)  {
        perror("send");
        exit(1);
    }
    char txt_buf[MAXDATASIZE];

    while (1){
        memset(txt_buf,0,sizeof(txt_buf));
        byteslen = recv(new_fd,txt_buf,sizeof(txt_buf),0);
        if (byteslen == -1)
        {
            perror("Recv txt error\n");
            break;
        }
        std::cout << txt_buf <<"\n";
        fflush(stdout);
        if (strlen(txt_buf) == 0)
        {
            std::string msg = "You pressed enter by mistake";
            if (send(new_fd, msg.c_str(), msg.length(),0) == -1)
            {
                perror("Error- send enter error");
                exit(1);
            }
        }
        else if (strncmp(txt_buf, "ctrl_c",6) == 0)
        {
            std::cout << "Client " << (new_fd - 3) << " disconnected!\n";
            break;
        }
        else{
            enQ(txt_buf,ao1->Q, new_fd);
        }
        // std::cout << "enqed: " << (char*)ao1->Q->enqueue(txt_buf,new_fd) << '\n';
    }
    close(new_fd);
    return 0;
}


int main(void)
{
    ao1 = newAO(createQ(), &f1, &fill_Q2);
    ao2 = newAO(createQ(), &f2, &fill_Q3);
    ao3 = newAO(createQ(), &print_data, &send_data);

    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage client_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    int i = 0;
    int new_fd;
    while(1) {  // main accept() loop
        sin_size = sizeof client_addr;
        
        new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(client_addr.ss_family,
        incoming_addr((struct sockaddr *)&client_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);
        
        pthread_t *thread_num = (pthread_t*)malloc(sizeof(pthread_t)*BACKLOG); // up to 10 clients
        pthread_create(&thread_num[i++], NULL, creat_thread, &new_fd);

    }
    return 0;
}
