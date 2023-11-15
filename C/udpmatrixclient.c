#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define MAX_ROWS 10
#define MAX_COLS 10
#define BUFFER_SIZE (MAX_ROWS * MAX_COLS * sizeof(int))

void send_matrix(int sockfd, int matrix[MAX_ROWS][MAX_COLS], int rows) {
    // Send the number of rows in the matrix
    sendto(sockfd, &rows, sizeof(rows), 0, NULL, 0);

    // Send the matrix data
    sendto(sockfd, matrix, BUFFER_SIZE, 0, NULL, 0);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int matrix[MAX_ROWS][MAX_COLS] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int rows = 3;

    // Creating socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address initialization
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Send the matrix to the server
    send_matrix(sockfd, matrix, rows);

    // Close the socket
    close(sockfd);

    return 0;
}
