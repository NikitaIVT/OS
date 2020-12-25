
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
    struct sockaddr_in server;
    char ans[256], command[256], arg[256];
    server.sin_family = AF_INET;
    server.sin_port = htons(2019);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    int sock = socket(AF_INET,SOCK_STREAM,0);

    if (connect(sock, (struct  sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed\n");
        return 1;
    } else {
        printf("Connection established\n");
    }
    while(strcmp(command, "exit")){
      printf("enter command\n>");
      scanf("%s", command);
      send(sock, command, 256,0);
      if(strcmp("-h", command) == 0){
        recv(sock, ans, 256,0);
        printf("Server send: \n%s\n", ans);
      } else if((strcmp(command, "-rs") == 0)){
        recv(sock, ans, 256,0);
        printf("Server send: \n%s\n", ans);
      } else if((strcmp(command, "-cbp") == 0) || (strcmp(command, "-cp") == 0)){
        recv(sock, ans, 256,0);
        printf("Server send: \n%s\n", ans);
        scanf("%s", arg);
        send(sock, arg, 256,0);
        recv(sock, ans, 256,0);
        printf("Server send: \n%s\n", ans);
      } else{
        printf("looks like a wrong command, try another one\nServer also said\n");
        recv(sock, ans, 256,0);
        printf("%s\n", ans);
      }
    }



    close(sock);
}
