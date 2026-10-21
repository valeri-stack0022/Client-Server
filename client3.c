#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8181
#define SIZE 512

int main()
{
    //Socket creation...
    int client_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock_fd == -1){
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &client_addr.sin_addr);

    //Connecting with Server...
    int c = connect(client_sock_fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if(c == -1){
        perror("Connecting is failed!\n");
        exit(EXIT_FAILURE);
    }

    char sending_string[SIZE] = {0};
    char result_buffer[SIZE] = {0};
    while(1){
        printf("Enter your string: ");
        if(fgets(sending_string, SIZE - 1, stdin) == NULL){
            break;
        }
        if(strcmp(sending_string, "exit\n") == 0){
            printf("Exited.\n");
            exit(1);
        }

        ssize_t s1 = send(client_sock_fd, sending_string, strlen(sending_string), 0);
        if(s1 == -1){
            perror("Sending is failed!\n");
            exit(EXIT_FAILURE);
        }

        ssize_t r1 = recv(client_sock_fd, result_buffer, SIZE, 0);
        if(r1 > 0){
            result_buffer[strcspn(result_buffer, "\n")] = '\0';
            printf("%s\n", result_buffer);
        }
        else if(r1 == -1){
            perror("Recv failed!\n");
            exit(EXIT_FAILURE);
        }
    }
}