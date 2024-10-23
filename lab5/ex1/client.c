#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 6000

int main()
{
    int sockfd;
    struct sockaddr_in s_addr;
    char message[1024];
    printf("Enter the message to send: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';
    char buffer[1024];
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sendto(sockfd, message, strlen(message), MSG_CONFIRM, (struct sockaddr *)&s_addr, sizeof(s_addr));
    socklen_t len = sizeof(s_addr);
    int n = recvfrom(sockfd, buffer, 1024, MSG_WAITALL, (struct sockaddr *)&s_addr, &len);
    buffer[n] = '\0';
    printf("%s\n", buffer);
    close(sockfd);
    return 0;
}