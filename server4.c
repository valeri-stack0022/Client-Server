#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 7070
#define SIZE 512

//Symbol + 1 = Next Symbol(In ASCII)
char *symbol_addition(char *string){
    char *string1 = string;
    while(*string1 != '\0'){
        if((*string1 >= 'a' && *string1 <= 'z') || (*string1 >= 'A' && *string1 <= 'Z')){
            switch(*string1){
                case 'z':{
                    *string1 = 'a';
                    ++string1;
                    break;
                }
                case 'Z':{
                    *string1 = 'A';
                    ++string1;
                    break;
                }
                default:{
                    ++(*(string1));
                    ++string1;
                }
            }
        }
        else{
            char *error = "Only Letters!";
            return error;
        }
    }
    return string;
}

int main()
{
    //Socket creation...
    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1){
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    //Struct for Server...
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    //For reuse our Port...
    int opt = 1;
    setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int b = bind(server_sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(b == -1){
        perror("Bind failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Listening...\n");
    //Listening...(Passive Socket)
    int l = listen(server_sock_fd, SOMAXCONN);
    if(l == -1){
        perror("Listen failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Connecting...\n");

    //Struct for Caching Clients structs...
    struct sockaddr_in clients_addr;
    memset(&clients_addr, 0, sizeof(clients_addr));
    socklen_t length = sizeof(clients_addr);

    //For handshake...
    int new_sock_fd = accept(server_sock_fd, (struct sockaddr*)&clients_addr, &length);
    if(new_sock_fd == -1){
        perror("Accepting failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected!\n");

    char accepting_string[SIZE] = {0};
    while(1){
        int r1 = recv(new_sock_fd, accepting_string, SIZE, 0);
        if(r1 > 0){
            accepting_string[strcspn(accepting_string, "\n")] = '\0';
            char *result_string = symbol_addition(accepting_string);
            ssize_t s1 = send(new_sock_fd, result_string, strlen(result_string), 0);
            if(s1 == -1){
                perror("Send failed!\n");
                exit(EXIT_FAILURE);
            }
        }
        else if(r1 == 0){
            printf("Nothing to read!\n");
            exit(EXIT_FAILURE);
        }
        else{
            perror("Recv failed!\n");
            exit(EXIT_FAILURE);
        }
    }
}