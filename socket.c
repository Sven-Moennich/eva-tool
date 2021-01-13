#include <byteswap.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>


#include <linux/if.h>
#include <linux/in.h>

#include <sys/types.h>
#include <sys/socket.h>



int create_socket(char *ifname, unsigned int port) {
    struct sockaddr_in src = {0};
    struct timeval timeout = {0};
    const int bcast_flag = 1;
    int sd;

    src.sin_family = AF_INET;
    src.sin_port = htons(port);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Error creating socket");
        return -1;
    }

    if (bind(sd, (struct sockaddr *) &src, sizeof(struct sockaddr_in)) < 0) {
        perror("Error binding socket to port");
        close(sd);
        return -1;
    }

    if (setsockopt(sd, SOL_SOCKET, SO_BINDTODEVICE, ifname, strnlen(ifname, IFNAMSIZ)) < 0) {
        perror("Error binding to interface");
        close(sd);
        return -1;
    }

    if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &bcast_flag, sizeof(int)) < 0) {
        perror("Error enabling broadcast");
        close(sd);
        return -1;
    }

    if (setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0) {
        perror("Error setting socket receive timeout");
        close(sd);
        return -1;
    }

    return sd;
}
