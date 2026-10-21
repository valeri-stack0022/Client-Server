#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7070
#define SIZE 512

int main()
{
    //clinet socket creation...
    int client_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock_fd == -1){
        perror("Socket creation is failed!\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

    //Connecting with server...
    int c = connect(client_sock_fd, (struct sockaddr*)&addr, sizeof(addr));
    if(c == -1){
        perror("Connect failed!\n");
        exit(EXIT_FAILURE);
    }

    char sending_string[SIZE] = {0};
    char result_string[SIZE] = {0};
    while(1){
        printf("Input a letter sequence: (or exit for Quit)");
        if(fgets(sending_string, SIZE - 1, stdin) == NULL){
            break;
        }

        if(strcmp(sending_string, "exit\n") == 0){
            printf("Exited.\n");
            break;
        }

        ssize_t s1 = send(client_sock_fd, sending_string, strlen(sending_string), 0);
        if(s1 == -1){
            perror("Sending failed!\n");
            exit(EXIT_FAILURE);
        }

        ssize_t r1 = recv(client_sock_fd, result_string, SIZE, 0);
        if(r1 > 0){
            result_string[strcspn(result_string, "\n")] = '\0';
            printf("%s\n", result_string);
            memset(result_string, 0, SIZE);
        }
        else if(r1 == -1){
            perror("Recv failed!\n");
            exit(EXIT_FAILURE);
        }
    }

}