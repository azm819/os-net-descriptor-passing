#include "utils.h"

using namespace std;

int main() {
    int sockfd = createSocket();

    auto serv_addr = genStructFields("./socket");
    if (connect(sockfd, (struct sockaddr *) &serv_addr.second, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    auto message = genMsgFields();

    if (recvmsg(sockfd, &message, 0) == -1) {
        perror("recvmsg");
        exit(EXIT_FAILURE);
    }

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&message);
    dup2(*(int*)CMSG_DATA(cmsg), 1);

    return closeSocket(sockfd);
}