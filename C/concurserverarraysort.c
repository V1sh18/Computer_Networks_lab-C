#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>

#define PORTNO 10200
#define MAX_ARRAY_SIZE 10
#define BUFFER_SIZE (MAX_ARRAY_SIZE * sizeof(int))

void handle_client(int client_socket) {
    int received_array[MAX_ARRAY_SIZE];
    char buffer[BUFFER_SIZE];

    // Receive the integer array from the client
    read(client_socket, received_array, BUFFER_SIZE);

    // Sorting the array (using a simple bubble sort for illustration purposes)
    for (int i = 0; i < MAX_ARRAY_SIZE - 1; i++) {
        for (int j = 0; j < MAX_ARRAY_SIZE - i - 1; j++) {
            if (received_array[j] > received_array[j + 1]) {
                // Swap elements if they are in the wrong order
                int temp = received_array[j];
                received_array[j] = received_array[j + 1];
                received_array[j + 1] = temp;
            }
        }
    }

    // Create a response string with the sorted array and process ID
    snprintf(buffer, sizeof(buffer), "Sorted Array: ");
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%d ", received_array[i]);
    }
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "\nProcess ID: %d\n", getpid());

    // Send the response to the client
    write(client_socket, buffer, strlen(buffer));

    // Close the client socket
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1;
    int addrlen = sizeof(server_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORTNO);
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server listening on port %d...\n", PORTNO);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);

        if (fork() == 0) {
            close(server_fd);
            handle_client(client_socket);
            exit(0);
        } else {
            close(client_socket);
            wait(NULL);
        }
    }

    return 0;
}
