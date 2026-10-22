#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 8585
#define SIZE 512

int main()
{
    //Socket creation...
    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1){
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    //Struct for server's address...
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    int opt = 1;
    setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //Binding...
    int b = bind(server_sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(b == -1){
        perror("Bind failed!\n");
        exit(EXIT_FAILURE);
    }

    //Listening...
    int l = listen(server_sock_fd, SOMAXCONN);
    if(l == -1){
        perror("Listen failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Listening...\n");

    printf("Connecting...\n");
    struct sockaddr_in clients_addr;
    memset(&clients_addr, 0, sizeof(clients_addr));
    socklen_t length = sizeof(clients_addr);

    //Connecting with Server...(Handshake)
    int new_sock_fd = accept(server_sock_fd, (struct sockaddr*)&clients_addr, &length);
    if(new_sock_fd == -1){
        perror("Accept is failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected!\n");

    srand(time(NULL));
    int random_number = rand() % 1000;
    char reading_client_name[SIZE] = {0};
    char *sending_name_string = "Game -> To guess a Number: Input your name: ";

    //Sending string for input client name...
    ssize_t s1 = send(new_sock_fd, sending_name_string, strlen(sending_name_string), 0);
    if(s1 == -1){
        perror("Sending failed!\n");
        exit(EXIT_FAILURE);
    }

    //Recving that name...
    ssize_t r1 = recv(new_sock_fd, reading_client_name, SIZE, 0);
    if(r1 > 0){
        reading_client_name[strcspn(reading_client_name, "\n")] = '\0';
        printf("%s\n", reading_client_name);

        char *welcom_string = "Input your number:";
        ssize_t welcome = send(new_sock_fd, welcom_string, strlen(welcom_string), 0);
        if(welcome == -1){
            perror("Send is failed!\n");
            exit(EXIT_FAILURE);
        }

        //Sending string for client's number... And catch them...
        while(1){
            unsigned long long number = 0;
            char recving_number[SIZE] = {0};
            ssize_t r2 = recv(new_sock_fd, recving_number, sizeof(recving_number), 0);
            recving_number[r2] = '\0';
            if(r2 > 0){
                number = atoi(recving_number);
                if(number == random_number){
                    char *success = "Hooray! You found number!";
                    ssize_t s3 = send(new_sock_fd, success, strlen(success), 0);
                    if(s3 == -1){
                        perror("Sending is failed!\n");
                        exit(EXIT_FAILURE);
                    }
                    exit(EXIT_SUCCESS);
                }
                else if(number < random_number){
                    ssize_t s4 = send(new_sock_fd, "Too Low", strlen("Too Low"), 0);
                    if(s4 == -1){
                        perror("Send is failed!\n");
                        exit(EXIT_FAILURE);
                    }
                    continue;
                }
                else if(number > random_number){
                    ssize_t s4 = send(new_sock_fd, "Too High", strlen("Too High"), 0);
                    if(s4 == -1){
                        perror("Send is failed!\n");
                        exit(EXIT_FAILURE);
                    }
                    continue;
                }
            }
        }
    }
    else if(r1 == 0){
        perror("Nothing to read!\n");
        exit(EXIT_FAILURE);
    }
    else{
        perror("Recv is failed!\n");
        exit(EXIT_FAILURE);
    }
}