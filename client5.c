#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8585
#define SIZE 512

int main()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        perror("Socket creation is failed!\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &dest_addr.sin_addr);

    int c = connect(client_socket, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if(c == -1){
        perror("Connecting is failed!\n");
        exit(EXIT_FAILURE);
    }

        char reading_name_string[SIZE] = {0};
        ssize_t r1 = recv(client_socket, reading_name_string, SIZE, 0);
        if(r1 > 0){
            reading_name_string[strcspn(reading_name_string, "\n")] = '\0';
            printf("%s\n", reading_name_string);
        }
        else if(r1 == 0){
            perror("Nothing to read!\n");
            exit(EXIT_FAILURE);
        }
        else{
            perror("Recv is failed!\n");
            exit(EXIT_FAILURE);
        }

        char sending_my_name[SIZE] = {0};
        if(fgets(sending_my_name, SIZE - 1, stdin) == NULL){
            perror("Nothing to write!\n");
            exit(EXIT_FAILURE);
        }
        sending_my_name[strcspn(sending_my_name, "\n")] = '\0';
        ssize_t s1 = send(client_socket, sending_my_name, strlen(sending_my_name), 0);
        if(s1 == -1){
            perror("Sending is failed!\n");
            exit(EXIT_FAILURE);
        }

        char welcome_string[SIZE] = {0};
        ssize_t welcome = recv(client_socket, welcome_string, SIZE, 0);
        if(welcome > 0){
            welcome_string[welcome] = '\0';
            printf("%s\n", welcome_string);
        }
        else if(welcome == 0){
            perror("Nothing to read!\n");
            exit(EXIT_FAILURE);
        }
        else{
            perror("Recv is failed!\n");
            exit(EXIT_FAILURE);
        }

        while(1){
            char sending_number[SIZE] = {0};
            if(fgets(sending_number, SIZE - 1, stdin) == NULL){
                perror("Writing is failed!\n");
                exit(EXIT_FAILURE);
            }
            sending_number[strcspn(sending_number, "\n")] = '\0';
            ssize_t s2 = send(client_socket, sending_number, strlen(sending_number), 0);
            if(s2 == -1){
                perror("Sending is failed!\n");
                exit(EXIT_FAILURE);
            }
                char result_from_server[SIZE] = {0};
                ssize_t r2 = recv(client_socket, result_from_server, SIZE, 0);
                if(r2 > 0){
                    result_from_server[r2] = '\0';
                    if(strcmp(result_from_server, "Too Low") == 0){
                        printf("Too Low\n");
                        continue;
                    }
                    else if(strcmp(result_from_server, "Too High") == 0){
                        printf("Too High\n");
                        continue;
                    }
                    else if(strcmp(result_from_server, "Hooray! You found number!") == 0){
                        printf("%s\n", result_from_server);
                        exit(EXIT_SUCCESS);
                    }

                }
                else if(r2 == 0){
                    perror("Nothing to read!\n");
                    exit(EXIT_FAILURE);
                }
                else{
                    perror("Recv is failed!\n");
                    exit(EXIT_FAILURE);
                }
    }
}