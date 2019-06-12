#ifndef OS_NET_UTILS_H
#define OS_NET_UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>

using std::cout;
using std::cin;

#define SERVER_INFO cout << "Server: "

const size_t bufferLen = 1024;
const char *socketName = "./socket";

msghdr genMsgStruct() {
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(int))], dup[bufferLen];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = {.iov_base = &dup, .iov_len = sizeof(dup)};

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    return msg;
}

sockaddr_un genAddrStruct() {
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketName, sizeof(addr.sun_path) - 1);
    return addr;
}

void printError(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int createSocket() {
    int sockfd;
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        printError("Failed to create socket");
    }
    return sockfd;
}

#endif //OS_NET_UTILS_H
