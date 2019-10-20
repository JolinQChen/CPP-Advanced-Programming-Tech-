/*
 http://www.linuxhowtos.org/C_C++/socket.htm
 A simple server in the internet domain using TCP
 The port number is passed as an argument */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int SOCKET;
#endif

int sockInit(void)
{
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
    return 0;
#endif
}

int sockQuit(void)
{
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif
}

/* Note: For POSIX, typedef SOCKET as an int. */

int sockClose(SOCKET sock)
{
    
    int status = 0;
    
#ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    if (status == 0)
    {
        status = closesocket(sock);
    }
#else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0)
    {
        status = close(sock);
    }
#endif
    
    return status;
    
}

void error(const char *msg)
{
    perror(msg);
    
    exit(0);
}

struct udpMessage
{
    unsigned char nVersion;
    unsigned char nType;
    unsigned short nMsgLen;
    unsigned long lSeqNum;
    char chMsg[1000];
};

void receive(int sockfd, struct sockaddr from, socklen_t fromlen) {
    char buffer[1000];
    while (true) {
        int n = recvfrom(sockfd, buffer, 1000, 0, (sockaddr*)& from, &fromlen);
        printf("Received Msg Type: 1. Seq: 33, Msg: %s\n", buffer);
    }
}

int main(int argc, char *argv[])
{
    char version;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct udpMessage message;
    socklen_t fromlen = 0;
    struct sockaddr  from;
    struct udpMessage buffer;
    memset((char *)&from, 0, sizeof(sockaddr));
    std::string cmd;
    std::vector<std::string> cmd_v;
    
    //char buffer[1024];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    sockInit();
    // Convert string to int
    portno = atoi(argv[2]);
    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    
    server = gethostbyname(argv[1]);
    
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    // Zero out serv_addr variable
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    
    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    
    serv_addr.sin_port = htons(portno);
    
    //    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    //       error("ERROR connecting");
    
    fromlen = sizeof(serv_addr);
    
    std::thread receiveThread(receive, sockfd, from, fromlen);
    
    while (true)
    {
        // reveive a user message
        std::cout <<"Please enter command: ";
        getline(std::cin, cmd);
        std::cout << std::endl;
        // split the cmd by space and store in vector v;
        std::istringstream in(cmd);
        cmd_v.clear();
        std::string t;
        while (in >> t) {
            cmd_v.push_back(t);
        }
        if(cmd_v[0].at(0)=='v') {
            // get version number
            version = cmd_v[1].at(0);
        }
        else if(cmd_v[0].at(0) == 't') {
            buffer.nVersion = version;
            // get nType, lSeqNum, chMsg
            buffer.nType = cmd_v[1].at(0);
            buffer.lSeqNum = cmd_v[2].at(0);
            int len =cmd_v[3].length();
            // user input should not be greater than 1000
            memset((char *)&buffer.chMsg, 0, sizeof(buffer.chMsg));
            for(int i=0; i<len; i++){
                buffer.chMsg[i] =cmd_v[3].at(i);
            }
            buffer.nMsgLen = len;
            n = sendto(sockfd, &buffer, sizeof(udpMessage), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            if (n < 0)
                error("sendto");
            printf("Sent message!\n" );
            
        }
        else if(cmd_v[0].at(0) == 'q') {
            break;
        }
        else {
            std::cout << "wrong command!"<<std::endl;
        }
        //n = sendto(sockfd, (char*)&buffer, sizeof(buffer), 0, (struct sockaddr *)&from, fromlen);
        
    }
    
#ifdef _WIN32
    std::cin.get();
#endif
    return 0;
}
