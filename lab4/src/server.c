#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <dlfcn.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

void sigHandler(int signum) {
  printf("Signal %d was handled successfully\n", signum);
  _exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  int pid, ppid, rez;
  char *error;
  void *handle = dlopen("/home/_chariot/os/lab4/dlib/libhelp.so", RTLD_NOW);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit (-1);
    }

    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit (-1);;
    }
    typedef void (*func_info)();
    func_info info = (func_info)dlsym(handle,"info");
    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit(-1);
    }
  char command[256], arg[256], ans[256];
    struct sockaddr_in server, client;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(2019);
    bind(sock, (struct sockaddr *) &server, sizeof(server));
    listen(sock, 5);
    int newsock, size;
      socklen_t clnlen;
      int numh,num;
      newsock = accept(sock, (struct sockaddr*)&client, &clnlen);
      printf("New client: %s\n",inet_ntoa(client.sin_addr));
    while(1){
        recv(newsock, command, 256, 0);

  if (strcmp("-cp", command) == 0) {

    int status;
    pid_t pidt;
    char invite[30] = "Proc name?\n>";
    send(newsock, invite, 30, 0);
    recv(newsock, arg, 256, 0);
    pidt = fork();
    if (pidt == -1) {
      strcpy(ans, "Process creation failure\n");
      send(newsock, ans, 256, 0);
    } else if (pidt == 0) {
      pid_t pid, ppid;
      pid = getpid();
      ppid = getppid();
      sprintf(
          ans, "Process-child was created successfully\n %d - child's number\n %d parents number\n",
          pid, ppid);
          send(newsock, ans, 256, 0);
      if (execl(arg, arg, NULL) == -1) {
	       perror("exec");
	        _exit(EXIT_FAILURE);
      }
    }
  } else if (strcmp("-cbp", command) == 0) {
    int ind = 0;
    pid_t pidt;
    char invite[40] = "Proc name?\n>";
    send(newsock, invite, 30, 0);
    recv(newsock, arg, 256, 0);
    pidt = fork();
    if (pidt == -1) {
      strcpy(ans, "Process creation failure\n");
      send(newsock, ans, 256, 0);
    } else if (pidt == 0) {
      setsid();
      pid = getpid();
      ppid = getppid();
      chdir("/");
      execl(arg, arg, NULL);
      wait(&ind);
      sprintf(ans, "Background process was created \npid = %d\nppid = %d", pid, ppid);
      send(newsock, ans, 256, 0);
    }

  } else if (strcmp("-rs", command) == 0) {
    signal(SIGINT , sigHandler);
    pid_t pidt;
    int pid;
    pidt = fork();
   if (pidt == 0) {
      pid_t pid, ppid;
      pid = getpid();
      ppid = getppid();
      kill(pid, SIGINT);
    }
    strcpy(ans, "Signal was catched\n");
    send(newsock, ans, 256, 0);
  } else if(strcmp("-h", command) == 0){
    info(ans);
    strcpy(ans, "You can check help on serverside\n");
    send(newsock, ans, 256, 0);
  } else{
    strcpy(ans, "Wrong command\n");
    send(newsock, ans, 256, 0);
  }

}
}
