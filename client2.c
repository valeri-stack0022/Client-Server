#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090
#define PASSWORD_SIZE 1024
#define SIZE 1024


int main()
{
    int client_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock_fd == -1){
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    int c = connect(client_sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(c == -1){
        perror("Connect failed!\n");
        exit(EXIT_FAILURE);
    }

    char search_password[PASSWORD_SIZE];
    while(1){

        printf("Enter your password: ");
        if(fgets(search_password, PASSWORD_SIZE - 1, stdin) == NULL){
            break;
        }

        if(strcmp(search_password, "exit\n") == 0){
            printf("Exited.\n");
            break;
        }
        printf("%s\n", search_password);
        ssize_t s1 = send(client_sock_fd, search_password, strlen(search_password), 0);
        if(s1 == -1){
            perror("Sending failed!\n");
            exit(EXIT_FAILURE);
        }

        char result[SIZE] = {0};
        ssize_t r1 = recv(client_sock_fd, result, SIZE, 0);
        if(r1 > 0){
            result[r1] = '\0';
            if(strcmp(result, "ACCESS GARANTED") == 0){
                printf("%s\n", result);
                break;
            }
            else{
                printf("ACCESS DENIED\n");
            }
        }
        else if(r1 == -1){
            perror("Recv failed!\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
