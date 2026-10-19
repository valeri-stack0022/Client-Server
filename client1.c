#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
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
        perror("Connect is failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter an expression: ");
    char expression_buffer[SIZE];
    fgets(expression_buffer, SIZE - 1, stdin);
    expression_buffer[strlen(expression_buffer)] = '\0';


    int s = send(client_sock_fd, expression_buffer, strlen(expression_buffer), 0);
    if(s == -1){
        perror("Sending failed!\n");
        exit(EXIT_FAILURE);
    }

    char buffer_for_error[SIZE] = {0};
    char result[SIZE] = {0};
    ssize_t r2 = recv(client_sock_fd, result, SIZE, 0);
    if(r2 > 0){
        printf("Result %s\n", result);
    }
    else{
        perror("Recv failed!\n");
        exit(EXIT_FAILURE);
    }
}

  /*#include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <arpa/inet.h>

   #define PORT 8080
   #define BUFFER_SIZE 1024

  int main(){
      int sock = 0;
      struct sockaddr_in serv_addr;
      char expression[BUFFER_SIZE] = {0};
      char response[BUFFER_SIZE] = {0};

      if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
          printf("Socket creation error\n");
          return -1;
      }

      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(PORT);

      if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <=0){
          printf("Invalid address/Address not supported\n");
          close(sock);
          return -1;
      }

      if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
          printf("Connect failed\n");
          close(sock);
          return -1;
      }

      printf("Mutqagreq dzer artahaytutyuny\n");
      if(fgets(expression, sizeof(expression), stdin) != NULL){
          send(sock, expression, strlen(expression), 0);

          ssize_t bytes_read = read(sock, response, BUFFER_SIZE - 1);
          if(bytes_read > 0){
              response[bytes_read] = '\n';
              printf("Result = %s\n", response);
          }
      }

      close(sock);
      return 0;
  }*/

