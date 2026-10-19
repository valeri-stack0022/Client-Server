#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define IP "127.0.0.1"
#define PORT 9090
#define SIZE 1024
#define PASSWORD_SIZE 100

char password[PASSWORD_SIZE] = "12345";

int main()
{
    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1){
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &addr.sin_addr);

    int b = bind(server_sock_fd, (struct sockaddr*)&addr, sizeof(addr));
    if(b == -1){
        perror("Bind failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");
    int l = listen(server_sock_fd, SOMAXCONN);
    if(l == -1){
        perror("Listen is failed1\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clients_addr;
    memset(&clients_addr, 0, sizeof(clients_addr));
    socklen_t length = sizeof(clients_addr);
    int new_sock_fd = accept(server_sock_fd, (struct sockaddr*)&clients_addr, &length);
    if(new_sock_fd == -1){
        perror("New Socket creation is failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected!\n");

    while(1){
        char search_password[SIZE] = {0};
        ssize_t r1 = recv(new_sock_fd, search_password, SIZE, 0);
        if(r1 > 0){

            search_password[strcspn(search_password, "\n")] = '\0';

            if(strcmp(search_password, password) == 0){
                ssize_t s1 = send(new_sock_fd, "ACCESS GARANTED", strlen("ACCESS GARANTED"), 0);
                if(s1 == -1){
                    perror("Send is failed!\n");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                ssize_t s2 = send(new_sock_fd, "ACCESS DENIED", strlen("ACCESS DENIED"), 0);
                if(s2 == -1){
                    perror("Send is failed!\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else if(r1 == -1){
            perror("Recv is failed!\n");
            exit(EXIT_FAILURE);
        }
    }
}
