#include "utils.h"


int main() {
    int sockfd = createSocket();
    auto address = genAddrStruct();

    if (connect(sockfd, (struct sockaddr*)&address, sizeof(struct sockaddr_un)) == -1)
        printError("connect");

    char id[bufferLen];
    cout << "Enter ID\n";
    cin >> id;

    auto msg = genMsgStruct();
    if (recvmsg(sockfd, &msg, 0) == -1)
        printError("recvmsg");

    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    int* fd = (int*)CMSG_DATA(cmsg);
    dup2(*fd, 1);

    cout << id << " connected.\n";
    while (true) {
        char message[bufferLen];
        cin >> message;
        if (strcmp(message, "exit") == 0) {
            cout << id << " disconnected.\n";
            break;
        }
        cout << id << ": " << message << '\n';
    }

    if (close(sockfd) == -1)
        printError("close");

    return 0;
}
