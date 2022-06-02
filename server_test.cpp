/*
===================================
 Author: Orel Zamler & Amit Berger
 Date:   15 may 2022
===================================
*/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <csignal>

using namespace std;

#define PORT 6060
char rbuf[1024];
int error_check;

void send_msg(int socket, string txt){
    if (send(socket, txt.c_str() , txt.length() + 1, 0) == -1)
    {
        perror("Error- send");
        exit(1);
    }
}

void check_the_response(int socket, string expected){
    memset(rbuf,0,sizeof(rbuf));
    if (recv(socket, rbuf, sizeof(rbuf), 0) == -1)
    {
        perror("Sec recv prob!");
        exit(1);
    }
    cout << "expected: " << expected;
    cout << "\nGot: " << rbuf << '\n';
    fflush(stdout);
    assert((strcmp(expected.c_str(),rbuf) == 0));
}


int main()
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Socket failed\n");
        exit(1);
    }
    
    if (connect(sock,(struct sockaddr*)&server_address, sizeof(server_address))!= 0)
    {
        perror("Connect failed");
        exit(1);
    }
    memset(rbuf,0,sizeof(rbuf));
    int rv = recv(sock, rbuf, sizeof(rbuf), 0);
    if (rv == -1)
    {
        perror("First recv prob!");
    }
    
    cout << "Begin: " << rbuf << '\n';
    cout << "We are connected! starting the tests..\n";
    
// ================================
    // Basic stack operation check
// ================================

    // Lower case
    send_msg(sock, "a");
    check_the_response(sock, "B");
    send_msg(sock, "c");
    check_the_response(sock, "D");
    send_msg(sock, "abcd");
    check_the_response(sock, "BCDE");
    send_msg(sock, "hal");
    check_the_response(sock, "IBM");

    // Upper case
    send_msg(sock, "E");
    check_the_response(sock, "F");
    send_msg(sock, "F");
    check_the_response(sock, "G");
    send_msg(sock, "ABCD");
    check_the_response(sock, "BCDE");
    send_msg(sock, "HAL");
    check_the_response(sock, "IBM");

// ================================================= Special cases =================================================

    // =============
    // Ilegal input
    // =============
    
    send_msg(sock, " ");
    check_the_response(sock, " ");
    send_msg(sock, "a A");
    check_the_response(sock, "B B");
    send_msg(sock, "1");
    check_the_response(sock, "1");
    send_msg(sock, "1 2");
    check_the_response(sock, "1 2");
    send_msg(sock, "$");
    check_the_response(sock, "$");
    send_msg(sock, "[aaa]");
    check_the_response(sock, "[BBB]");
    send_msg(sock, "[]");
    check_the_response(sock, "[]");
    cout << "\n\n";

    // ================
    // Kisar end cases
    // ================

    send_msg(sock, "z");
    check_the_response(sock, "A");
    send_msg(sock, "Z");
    check_the_response(sock, "A");
    send_msg(sock, "tuvwxyzab");
    check_the_response(sock, "UVWXYZABC");
    cout << "\n\n";

    // =========================
    // Pressed enter by mistake
    // =========================

    send_msg(sock, "");
    check_the_response(sock, "You pressed enter by mistake");

    // ===================
    // Reconnecting check
    // ===================


    // After this the client will disconnect but the server is still open!!
    send_msg(sock, "ctrl_c");

    sleep(1);
    close(sock);
    // Let's make sure we can still reconnect:
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Socket failed\n");
        exit(1);
    }
    
    if (connect(sock,(struct sockaddr*)&server_address, sizeof(server_address))!= 0)
    {
        perror("Connect failed");
        exit(1);
    }
    memset(rbuf,0,sizeof(rbuf));
    rv = recv(sock, rbuf, sizeof(rbuf), 0);
    if (rv == -1)
    {
        perror("First recv prob!");
    }
    
    cout << "Begin: " << rbuf << '\n';
    cout << "We are connected! starting the tests..\n";

    send_msg(sock, "hl azbj!!");
    check_the_response(sock, "IM BACK!!");

    send_msg(sock, "ctrl_c");
    cout << "\nThe test was a success!!\n";
    sleep(1);
    close(sock);

    return 0;
}
