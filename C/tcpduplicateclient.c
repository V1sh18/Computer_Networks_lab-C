#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char sentence[MAX_BUFFER_SIZE];

    // Creating socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Server address initialization
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        // Get the sentence from the user
        printf("Client: Enter a sentence (type 'Stop' to terminate): ");
        fgets(sentence, sizeof(sentence), stdin);

        // Send the sentence to the server
        write(sockfd, sentence, strlen(sentence) + 1);

        // Check for termination condition
        if (strcmp(sentence, "Stop\n") == 0) {
            printf("Client: Terminating...\n");
            break;
        }

        // Receive the result from the server
        read(sockfd, sentence, sizeof(sentence));

        // Display the received data
        printf("Client: Received modified sentence from server: %s", sentence);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
