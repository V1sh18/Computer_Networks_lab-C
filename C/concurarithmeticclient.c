#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORTNO 10200

void perform_arithmetic_operation(int sockfd) {
    int numbers[2];
    char operator;
    int result;

    // Get user input for two integers and an operator
    printf("Enter the first number: ");
    scanf("%d", &numbers[0]);
    printf("Enter the second number: ");
    scanf("%d", &numbers[1]);
    printf("Enter the operator (+, -, *, /): ");
    scanf(" %c", &operator);

    // Send the integers and operator to the server
    write(sockfd, numbers, sizeof(numbers));
    write(sockfd, &operator, sizeof(operator));

    // Receive the result from the server
    read(sockfd, &result, sizeof(result));

    // Display the result
    printf("Result received from server: %d\n", result);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Creating socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Assuming the server is running on the same machine
    server_addr.sin_port = htons(PORTNO);

    // Connect to the server
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Perform arithmetic operation
    perform_arithmetic_operation(sockfd);

    // Close the socket
    close(sockfd);

    return 0;
}
