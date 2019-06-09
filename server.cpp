#include "utils.h"

using namespace std;

int main() {
    int sockfd = createSocket();

    auto my_addr = genStructFields("./socket");

    if (unlink("./socket") == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (struct sockaddr *) &my_addr.second, my_addr.first) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    int backlog = SOMAXCONN;
    if (listen(sockfd, backlog) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    int clientSockd;
    socklen_t addrlen;
    while (true) {
        if ((clientSockd = accept(sockfd, (sockaddr *) &addr, &addrlen)) == -1) {
            perror("accept");
            break;
        }

        auto message = genMsgFields();

        struct cmsghdr *cmsg = CMSG_FIRSTHDR(&message);
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;

        if (sendmsg(clientSockd, &message, 0) == -1) {
            perror("sendmsg");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
