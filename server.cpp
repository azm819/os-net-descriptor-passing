#include "utils.h"

int main() {
    int sockfd = createSocket();

    if (unlink(socketName) == -1 && errno != ENOENT)
        printError("unlink");

    auto address = genAddrStruct();

    if (bind(sockfd, (struct sockaddr*)&address, sizeof(struct sockaddr_un)) == -1)
        printError("bind");

    int backlog = SOMAXCONN;
    if (listen(sockfd, backlog) == -1)
        printError("listen");

    SERVER_INFO << "Server running.\n";
    int clientSocket;
    while ((clientSocket = accept(sockfd, nullptr, nullptr)) != -1) {
        auto msg = genMsgStruct();

        struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));
        int* fd = (int*)CMSG_DATA(cmsg);
        *fd = 1;

        if(sendmsg(clientSocket, &msg, 0) == -1)
            printError("sendmsg");

        SERVER_INFO << "New user.\n";
    }

    std::cerr << "Connetcion failed.\n";
    exit(EXIT_FAILURE);
}
