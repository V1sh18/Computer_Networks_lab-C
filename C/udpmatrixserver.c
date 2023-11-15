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

void combine_matrix(int received_matrix[MAX_ROWS][MAX_COLS], int combined_matrix[MAX_ROWS][MAX_COLS], int rows_received) {
    for (int i = 0; i < rows_received; ++i) {
        for (int j = 0; j < MAX_COLS; ++j) {
            combined_matrix[i][j] = received_matrix[i][j];
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int received_matrix[MAX_ROWS][MAX_COLS];
    int combined_matrix[MAX_ROWS][MAX_COLS] = {{0}};
    int rows_received;

    // Creating socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address initialization
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding the socket to the server address
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Receiving rows of the matrix from the client
        recvfrom(sockfd, &rows_received, sizeof(rows_received), 0, (struct sockaddr *)&client_addr, &client_len);
        recvfrom(sockfd, received_matrix, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        // Combine the received rows into the combined matrix
        combine_matrix(received_matrix, combined_matrix, rows_received);

        printf("Received rows: %d\n", rows_received);

        // Print the combined matrix
        printf("Combined Matrix:\n");
        for (int i = 0; i < rows_received; ++i) {
            for (int j = 0; j < MAX_COLS; ++j) {
                printf("%d\t", combined_matrix[i][j]);
            }
            printf("\n");
        }
    }

    return 0;
}
