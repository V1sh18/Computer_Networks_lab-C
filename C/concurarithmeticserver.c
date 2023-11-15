#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORTNO 10200

int perform_operation(int num1, int num2, char op) {
    switch (op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0) {
                return num1 / num2;
            } else {
                return -1;  // Indicate division by zero
            }
        default:
            return -1;  // Indicate invalid operator
    }
}

void handle_client(int client_socket) {
    int numbers[2];
    char operator;
    int result;

    // Receive two integers and an operator from the client
    read(client_socket, numbers, sizeof(numbers));
    read(client_socket, &operator, sizeof(operator));

    // Perform the arithmetic operation
    result = perform_operation(numbers[0], numbers[1], operator);

    // Send the result back to the client
    write(client_socket, &result, sizeof(result));

    // Close the client socket
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1;
    int addrlen = sizeof(server_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int){1}, sizeof(int));
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
