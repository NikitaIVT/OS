
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigHandler(int signum) {
  printf("Signal %d was handled successfully\n", signum);
  _exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  int pid, ppid, rez;
  char adr[] = "../lab2/fsmanager";
   if(signal(SIGINT, sigHandler) == SIG_ERR){
      printf("Error while receiving signal\n");
    } else {
      printf("Signal can be handled successfully\n");
    }
 if(signal(SIGTSTP, sigHandler) == SIG_ERR){
      printf("Error while receiving signal\n");
    } else {
      printf("Signal can be handled successfully\n");
    }
 if(signal(SIGTERM, sigHandler) == SIG_ERR){
      printf("Error while receiving signal\n");
    } else {
      printf("Signal can be handled successfully\n");
    }
 if(signal(SIGQUIT, sigHandler) == SIG_ERR){
      printf("Error while receiving signal\n");
    } else {
      printf("Signal can be handled successfully\n");
    }

  
  
  
  
  if(argc == 1){
   printf("Check for help like this\n./litesh -h\n");
   return 0;
   }
  if (strcmp("-fm", argv[1]) == 0) {
    pid_t pid;
    pid = fork();
    int status;

    if (pid == 0) {
      char args[3][40] = {0};
      printf("Input args: ");
      for (int i = 0; i < 3; i++) {
        fgets(args[i], 40, stdin );
      }
      for (int i = 0; i < 3; i++) {
        for(int j = 0; j < 40; j++){
          if(args[i][j] == '\n'){
            args[i][j] = '\0';
          }
        }
      }
      execl(adr, adr, args[0], args[1], args[2], NULL);
      _exit(EXIT_FAILURE);
    } else if (pid == (-1))
      return 1;
    else if ((waitpid(pid, &status, 0)) != pid)
      return 1;
  } else if (strcmp("-cp", argv[1]) == 0) {
    if (argc != 3) {
      printf("Wrong amount of arguements");
      return 1;
    }
    int status;
    pid_t pidt;
    pidt = fork();
    if (pidt == -1) {
      printf("Process creation failure\n");
      return 1;
    } else if (pidt == 0) {
      pid_t pid, ppid;
      pid = getpid();
      ppid = getppid();
      if (execl(argv[2], argv[2], NULL) == -1) {
	perror("exec");
	_exit(EXIT_FAILURE);
      }
      wait(&status);
      printf(
          "Process-child was created successfully\n %d - child's number\n %d - "
          "parent's number\n",
          pid, ppid);
      return 0;
    }
  } else if (strcmp("-cbp", argv[1]) == 0) {
    if (argc != 3) {
            printf("Wrong amount of arguements\n");
            return 1;
    }
    int ind = 0;
    pid_t pidt;
    pidt = fork();
    if (pidt == -1) {
      printf("Background process was not process created\n");
      return 1;
    } else if (pidt == 0) {
      setsid();
      pid = getpid();
      ppid = getppid();
      chdir("/");
      execl(argv[2], argv[2], NULL);
      wait(&ind);
      printf("Background process was created \npid = %d\nppid = %d", pid, ppid);
      return 0;
    }

  } else if (strcmp("-rs", argv[1]) == 0) {
    if (argc != 2) {
      printf("Wrong amount of arguements");
      return 1;
    }
    sleep(30);
    return 0;
  } else if(strcmp("-h", argv[1]) == 0){
    printf("Авторы: Мяконьких Дмитрий, Шиндель Эдуард, Полищук Никита\n");
    printf("-fm - Вызов программы для работы с файловой системой\n");
    printf("Пример запуска - ./litesh -fm\n");
    printf("-cp - Порождение процесса\n");
    printf("Пример запуска - ./litesh -cp proc_name\n");
    printf("-cbp - Перевод процесса в фоновый режим\n");
    printf("Пример запуска - ./litesh -cbp proc_name\n");
    printf("-rs - Получение сигнала от процесса\n");
    printf("Пример запуска - ./litesh -rs\n");
  }
}
