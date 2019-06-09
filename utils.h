#ifndef OS_NET_UTILS_H
#define OS_NET_UTILS_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

const size_t bufferLen = 1024;

int closeSocket(int socket) {
    if (close(socket) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}

msghdr genMsgFields() {
    char duff[bufferLen];
    struct iovec io = {
            io.iov_base = &duff,
            io.iov_len = bufferLen};

    struct msghdr message;
    char buff[CMSG_SPACE(sizeof(int))];
    bzero(buff, sizeof(buff));
    message.msg_iov = &io;
    message.msg_iovlen = 1;
    message.msg_control = buff;
    message.msg_controllen = sizeof(buff);
    return message;
}

int createSocket() {
    int sockfd;
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}


std::pair<socklen_t, sockaddr_un> genStructFields(const char *socketName) {
    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socketName, sizeof(address.sun_path) - 1);
    return {sizeof(address), address};
}

#endif //OS_NET_UTILS_H
