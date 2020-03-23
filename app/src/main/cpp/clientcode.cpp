//
// Created by karan on 12-03-2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <android/log.h>

#include "clientcode.h"
#include "util.h"

#define PORT 8080

int mainfunc(const char *ip){
    setbuf(stdout, NULL);
    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket < 0){
        log_output("[-]Error creating socket.\n");
        return 1;
    }

    log_output("[+]Client Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
//    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        log_output("[-]Error in connection.\n");
        return 1;
    }

    log_output("[+]Connected to Server.\n");

    while(1){
         log_output("Client: \t");
         // scanf("%s", &buffer[0]);
         // send(clientSocket, buffer, strlen(buffer), 0);

         if(strcmp(buffer, ":exit") == 0){
            close(clientSocket);
            log_output("[-]Disconnected from server.\n");
             break;
         }

         if(recv(clientSocket, buffer, 1024, 0) < 0){
            log_output("[-]Error in receiving data.\n");
         }else{
            log_output("Server: \t%s\n", buffer);
         }
    }

    return 0;
}
