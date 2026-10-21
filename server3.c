#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 8181
#define SIZE 512

char * string_handling(char string[]){
    char *string1 = malloc((strlen(string) + 1) * sizeof(char));
    strcpy(string1, string);
    char *left = string1;
    char *right = string1 + strlen(string1) - 1;
    while(left < right){
        char temp = *left;
        *left = *right;
        *right = temp;
        ++left;
        --right;
    }
    left = string1;
    right = string1 + strlen(string1) - 1;
    char *other_left = string1;
    char *other_right = NULL;
    while(left < right){
        ++other_left;
        if(*other_left == ' ' || *other_left == '\0'){
            other_right = other_left;
            while(left < other_left){
                --other_left;
                char temp = *left;
                *left  = *other_left;
                *other_left = temp;
                ++left;
            }
            left = other_right;
            other_left = other_right;
        }
    }
    return string1;
}

int main()
{
    //Socket creation...
    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1){
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    //Struct creation...
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
        perror("Listening is failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Server is listening...\n");
    printf("Connecting...\n");
    sleep(2);
    struct sockaddr_in clients_addr;
    memset(&clients_addr, 0, sizeof(clients_addr));
    socklen_t length = sizeof(clients_addr);

    //Connecting with Client(Accepting...)
    int new_sock_fd = accept(server_sock_fd, (struct sockaddr*)&clients_addr, &length);
    if(new_sock_fd == -1){
        perror("Accpeting is failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected!\n");

    while(1){
        char accepting_string[SIZE] = {0};

        ssize_t r1 = recv(new_sock_fd, accepting_string, SIZE, 0);
        if(r1 > 0){
            char *result_string = malloc(SIZE * sizeof(char));
            accepting_string[strcspn(accepting_string, "\n")] = '\0';
            result_string = string_handling(accepting_string);
            ssize_t s1 = send(new_sock_fd, result_string, strlen(result_string), 0);
            if(s1 == -1){
                perror("Sending failed!\n");
                exit(EXIT_FAILURE);
            }
        }
        else if(r1 == -1){
            perror("Recv is failed!\n");
        }
    }
}