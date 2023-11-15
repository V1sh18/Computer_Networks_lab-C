#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

void process_sentence(char* sentence) {
    // Tokenize the sentence and check for duplicate words
    char* token = strtok(sentence, " ");
    while (token != NULL) {
        printf("%s ", token);
        token = strtok(NULL, " ");
    }
    printf("\n");
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1;
    int addrlen = sizeof(server_addr);

    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Server address initialization
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding the socket to the server address
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen for incoming connections
    listen(server_fd, 5);

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept the connection from the client
        client_socket = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);

        if (fork() == 0) {
            // Child process handles the client
            close(server_fd);

            char sentence[MAX_BUFFER_SIZE];

            while (1) {
                // Receive the sentence from the client
                read(client_socket, sentence, sizeof(sentence));
                
                // Check for termination condition
                if (strcmp(sentence, "Stop\n") == 0) {
                    printf("Server: Received 'Stop'. Terminating...\n");
                    close(client_socket);
                    exit(0);
                }

                // Process the sentence (remove duplicate words)
                process_sentence(sentence);

                // Send the result back to the client
                write(client_socket, sentence, strlen(sentence) + 1);
            }
        } else {
            // Parent process continues listening for new connections
            close(client_socket);
        }
    }

    return 0;
}
