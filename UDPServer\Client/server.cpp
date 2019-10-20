#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int SOCKET;
#endif
/////////////////////////////////////////////////
// Cross-platform socket initialize
int sockInit(void)
{
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
    return 0;
#endif
}
/////////////////////////////////////////////////
// Cross-platform socket quit
int sockQuit(void)
{
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif
}
/////////////////////////////////////////////////
// Cross-platform socket close
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
/////////////////////////////////////////////////
// Output error message and exit
void error(const char *msg)
{
    perror(msg);
    // Make sure any open sockets are closed before calling exit
    exit(1);
}

struct udpMessage
{
    unsigned char nVersion;
    unsigned char nType;
    unsigned short nMsgLen;
    unsigned long lSeqNum;
    char chMsg[1000];
};

std::string compoToString(std::vector<struct udpMessage>& compoMsg) {
    int n = compoMsg.size();
    std::string compoMsgStr;
    int i = 0;
    while (i < n) {
        compoMsgStr.append(compoMsg[i].chMsg);
        i++;
    }
    return compoMsgStr;
}

void takeCmd(std::vector<struct udpMessage>& compoMsg, std::vector<struct sockaddr_in>& clientList, int sockfd, socklen_t fromlen) {
    while (true) {
        std::cout << "Please enter command: ";
        int command;
        std::cin >> command;
        std::cout<<std::endl;
        
        std::string compoMsgStr = compoToString(compoMsg);
        int length = compoMsgStr.length();
        
        if (command == 0) {
            char text[length + 1];
            strcpy(text, compoMsgStr.c_str());
            int i = 0;
            while (i < clientList.size()) {
                int n = sendto(sockfd, text, 1000, 0, (struct sockaddr*) & clientList[i], fromlen);
                if (n < 0) {
                    error("sendto");
                }
                i = i + 1;
            }
            //printf("all send\n");
            compoMsg.clear();
        }
        else if (command == 1) {
            // send out and clear
            compoMsg.clear();
        }
        
        else if (command == 2) {
            
            if (compoMsgStr.size() == 0) {
                printf("No message available\n");
            }
            else{
                char text[length + 1];
                strcpy(text, compoMsgStr.c_str());
                printf("the message is: %s \n", text);
                
            }
        }
        else {
            std::cout<<"invalid command!"<<std::endl;
        }
    }
}




/////////////////////////////////////////////////
// Main
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t fromlen;
    char buffer[1024];
    struct sockaddr_in serv_addr, from;
    int n;
    std::vector<struct udpMessage> compoMsg;
    std::vector<struct sockaddr_in> clientList;
    std::string compoMsgStr;
    struct udpMessage message;
    
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockInit();
    // Create the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // Make sure the socket was created
    if (sockfd < 0)
        error("ERROR opening socket");
    // Zero out the variable serv_addr
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    // Convert the port number string to an int
    portno = atoi(argv[1]);
    
    // Initialize the serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // Convert port number from host to network
    serv_addr.sin_port = htons(portno);
    // Bind the socket to the port number
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    
    //printf("Waiting on messages...\n");
    
    fromlen = sizeof(struct sockaddr_in);
    
    
    
    std::thread td(takeCmd, ref(compoMsg), ref(clientList), sockfd, fromlen);
    
    while (true)
    {
        n = recvfrom(sockfd, &message, sizeof(struct udpMessage), 0, (struct sockaddr *)&from, &fromlen);
        clientList.push_back(from);
        std::cout << "Received Msg Type:"<<message.nType-'0'<<", Seq: "<<message.lSeqNum-'0'<<", Msg: "<<message.chMsg<<std::endl;
        //printf("received\n");
        if (n < 0) {
            error("recvfrom");
        }
        if (message.nVersion == '1') {
            
        
        if (message.nType == '0') {
            //clear all current compoMsg and ignore following msg
            //printf("clear all!\n");
            compoMsg.clear();
        }
        else if (message.nType == '1') {
            // clear current compoMsg
            // use the chMsg as new start
            //printf("new start!\n");
            compoMsg.clear();
            compoMsg.push_back(message);
        }
        else if (message.nType == '2') {
            // add to compoMsg based on sequence number
            //printf("proceed!\n");
            int len = compoMsg.size();
            // insert the compoMsg according to the sequence number
            if (len == 0) {
                compoMsg.push_back(message);
            }
            else {
                int i = 0;
                while (i < len && (message.lSeqNum-'0') > (compoMsg[i].lSeqNum-'0')) {
                    i++;
                }
                compoMsg.insert(compoMsg.begin() + i, message);
            }
        }
        // check is size>1000
        //================= reorganize the compoMsg's structure ===============================
        // clear chMsg
        memset(message.chMsg, 0, 1024);
        // generate string message from composite massage
        compoMsgStr = compoToString(compoMsg);
        int length = compoMsgStr.length();
        char msgTxt[length + 1];
        strcpy(msgTxt, compoMsgStr.c_str());
        //printf("the length of the composite message is: %d \n", length);
        if (length >= 1000) {
            char tmp[1000];
            for (int i = 0; i < 1000; i++) {
                tmp[i] = msgTxt[i];
            }
            
            int len2 =clientList.size();
            for (int i=0; i < len2; i++) {
                int n = sendto(sockfd, tmp, 1000, 0, (struct sockaddr*) & clientList[i], fromlen);
                if (n < 0) error("sendto");
            }
            
            
            char res[1000];
            for (int i = 0; i < length + 1 - 1000; i++) {
                res[i] = msgTxt[i + 1000];
            }
            compoMsg.clear();
            udpMessage update;
            update.lSeqNum = 0;
            update.nVersion = 1;
            memcpy(update.chMsg, res, 1000);
            compoMsg.push_back(update);
            compoMsgStr = compoToString(compoMsg);
        }
        
        //printf("Now the composite message is: %s \n", compoMsgStr.c_str());
        
        
        
        if (message.nType == '3') {
            printf("broadcast!\n");
            int i = 0;
            while (i < clientList.size()) {
                n = sendto(sockfd, compoMsgStr.c_str(), length + 1, 0, (struct sockaddr*) & clientList[i], fromlen);
                if (n < 0) {
                    error("sendto");
                }
                i = i + 1;
            }
            compoMsg.clear();
        }
        }
        
    }
    
    sockClose(newsockfd);
    sockClose(sockfd);
    
    sockQuit();
    
#ifdef _WIN32
    std::cin.get();
#endif
    return 0;
}
