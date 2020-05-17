//
// Created by karan on 12-03-2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <android/log.h>
#include <limits.h>

#include <dlfcn.h>
#include <fcntl.h>

#include "clientcode.h"
#include "util.h"

#define PORT 8080
#define BUFFER_LEN 1024

void recv_file(int serv_sockfd, const char *path) {
    FILE *fp = fopen(path, "w");
    // read the filesize first
    int fsize = -1;
    read(serv_sockfd, &fsize, sizeof(int));

    char read_buf[BUFFER_LEN];
    for (int i = 0; i < fsize / BUFFER_LEN; i++) {
        int read_bytes = read(serv_sockfd, read_buf,
                              BUFFER_LEN); // TODO: assert read_bytes as BUFFER_LEN
        fwrite(read_buf, read_bytes, 1, fp);
    }
    if (fsize % BUFFER_LEN != 0) {
        int leftbytes = fsize - ftell(fp);
        int read_bytes = read(serv_sockfd, read_buf,
                              leftbytes); // TODO: assert read_bytes as leftbytes
        fwrite(read_buf, read_bytes, 1, fp);
    }

    fclose(fp);
}

void send_file(const char *path, int client_fd) {
    char buffer[BUFFER_LEN];
    FILE *fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    write(client_fd, &filesize, sizeof(int));
    for (int i = 0; i < filesize / BUFFER_LEN; i++) {
        fread(buffer, BUFFER_LEN, 1, fp);
        write(client_fd, buffer, BUFFER_LEN);
        // printf("%d\n", BUFFER_LEN);
    }
    if (filesize % BUFFER_LEN != 0) {
        int leftbytes = filesize - ftell(fp);
        fread(buffer, leftbytes, 1, fp);
        write(client_fd, buffer, leftbytes);
        // printf("%d\n", leftbytes);
    }
}

// path needs to have a / to be absolute or relative\
// TODO: add cmd line args
int exec_file(const char *path, const char *output_path) {
    // TODO: decide to work on separate thread or exec as separate process
    char *errstring;
    // for now it'll exec on the same thread
    void *client_code = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    errstring = dlerror();
    if (errstring != NULL)
    {
        log_output("Err: %s\n", errstring);
        return -1;
    }
    int (*client_mainfunc)() = (int (*)(void)) dlsym(client_code, "main");
    // create a tmp file to store output, redirect stdout to file
    int op_fd = open(output_path, O_CREAT | O_TRUNC | O_WRONLY, 0777);
    int stdout_fd = dup(1);
    int stderr_fd = dup(2);
    dup2(op_fd, 1);
    dup2(op_fd, 2);
    log_output("calling main\n");
    int retval = client_mainfunc();
    log_output("called main\n");
    dlclose(client_code);
    close(op_fd);
    dup2(stdout_fd, 1);
    close(stdout_fd);
    dup2(stderr_fd, 2);
    close(stderr_fd);
    return retval;
}

int mainfunc(const char *ip) {

    if (chdir("/data/data/com.example.clientside") == -1) {
        log_output("Error setting working directory\n");
        return -1;
    }

    setbuf(stdout, NULL);
    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0) {
        log_output("[-]Error creating socket.\n");
        return 1;
    }

    log_output("[+]Client Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    ret = connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (ret < 0) {
        log_output("[-]Error in connection.\n");
        return 1;
    }

    log_output("[+]Connected to Server.\n");

    while (1) {
        log_output("Waiting for tasks from server....\n");
        read(clientSocket, buffer, 5); // command size
        if (strcmp(buffer, "ping") == 0){
            write(clientSocket, "pong", 5); // this will also write back 5 bytes, read 5 bytes to consume this
        }
        else if (strcmp(buffer, "file") == 0){
            log_output("Receiving file\n");
            recv_file(clientSocket, "client_node_recvfile.so");
            log_output("Received file\n");
            exec_file("./client_node_recvfile.so", "client_recvfile_op.txt");
            log_output("Executed code\n");
            log_output("Sending Output Back to the server\n");
            send_file("./client_recvfile_op.txt",clientSocket);
            write(clientSocket, "complete", 9);
        }
        else if (strcmp(buffer, "exit") == 0){
            close(clientSocket);
            break;
        }
    }

    return 0;
}
