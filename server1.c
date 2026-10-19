#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <limits.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 1024

int expression_resolution(char expression[]){

    int num1, num2;
    char operator;
    sscanf(expression, "%d %c %d", &num1, &operator, &num2);

    switch(operator){
        case '+':{
            printf("Addition!\n");
            return num1 + num2;
        }
        case '-':{
            printf("Subtraction!\n");
            return num1 - num2;
        }
        case '*':{
            printf("Multiplying!\n");
            return num1 * num2;
        }
        case '/':{
            if(num2 != 0){
                printf("Division!\n");
                return num1 / num2;
            }
            else{
                printf("I cant't divide by zero!\n");
                return INT_MIN;
            }
        }
        default:{
            printf("Wrong operator!\n");
            return INT_MIN;
        }
    }
}

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
    inet_pton(AF_INET, "127.0.0.1" ,&addr.sin_addr);

    int opt = 1;
    setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int b = bind(server_sock_fd, (struct sockaddr*)&addr, sizeof(addr));
    if(b == -1){
        perror("Bind failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");
    int l = listen(server_sock_fd, SOMAXCONN);
    if(l == -1){
        perror("Listen is failed!\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clients_addr;
    memset(&clients_addr, 0, sizeof(clients_addr));
    socklen_t length = sizeof(clients_addr);
    int new_sock_fd = accept(server_sock_fd, (struct sockaddr*)&clients_addr, &length);
    if(new_sock_fd == -1){
        perror("New socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    char expression_buffer_from_client[SIZE];
    ssize_t first_recv = recv(new_sock_fd, expression_buffer_from_client, SIZE, 0);
    if(first_recv > 0){
        expression_buffer_from_client[first_recv] = '\0';

       // printf("%s\n", expression_buffer_from_client);

        int result = expression_resolution(expression_buffer_from_client);
        char result_buffer[SIZE] = {0};
        snprintf(result_buffer, SIZE, "%d", result);
        if(result || !result){
            ssize_t s1 = send(new_sock_fd, result_buffer, strlen(result_buffer), 0);
            if(s1 == -1){
                perror("Expression sending has been failed!\n");
                exit(EXIT_FAILURE);
            }
        }
        else if(result == INT_MIN){
            char error[SIZE] = "ERROR";
            ssize_t s2 = send(new_sock_fd, error, strlen(error), 0);
            if(s2 == -1){
                perror("Expression sending has been failed!\n");
                exit(EXIT_FAILURE);
            }
        }

    }
    else if(first_recv == -1){
        perror("First recv is failed!\n");
        exit(EXIT_FAILURE);
    }
}
