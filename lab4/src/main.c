#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

#define SHELL "/bin/sh"

int create_process(const char *command) {
    int status;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        execl(SHELL, SHELL, "-c", command, NULL);
        _exit(EXIT_FAILURE);
    } else if (pid < 0) {
        status = -1;
    } else {
        if (waitpid(pid, &status, 0) != pid) {
            status = -1;
        }
    }

    return status;
}

int create_background_process(const char *command) {
    int status = 0;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        setsid();
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
        execl(SHELL, SHELL, "-c", command, NULL);
        _exit(EXIT_FAILURE);
    } else if (pid < 0) {
        status = -1;
    }

    return status;
}

int send_signal(pid_t pid, int signum) {
    if (kill(pid, signum) == -1) {
        return 1;
    }

    return 0;
}

void signal_handler(int signumber) {
    printf("\nSignal - %d received successfully (Press Enter...)\n", signumber);
}

int receive_signal(int signumber) {
    if (signal(signumber, signal_handler) == SIG_ERR) {
        return 1;
    }

    return 0;
}

int main() {
    char *line = NULL;
    char *lin1 = NULL, *lin2 = NULL;
    size_t len = 0;
    char *choice = NULL;

    int exit = 0;
    while (!exit) {
        printf("<=>Welcome to LiteSH<=>\n"
               "1 create process\n"
               "2 create background process\n"
               "3 send signal\n"
               "4 receive signal\n"
               "5 help\n"
               "6 exit\n");
        printf("Enter key: ");
        getline(&choice, &len, stdin);
        choice[strlen(choice) - 1] = '\0';
        if (!strcmp(choice, "1")) {
            printf("Enter name of process: ");
            getline(&line, &len, stdin);
            line[strlen(line) - 1] = '\0';
            create_process(line);
            printf(">->->Procces over<-<-<\n");
        } else if (!strcmp(choice, "2")) {
            printf("Enter name of process: ");
            getline(&line, &len, stdin);
            line[strlen(line) - 1] = '\0';
            create_background_process(line);
        } else if (!strcmp(choice, "3")) {
            printf("Enter PID: ");
            getline(&lin1, &len, stdin);
            lin1[strlen(lin1) - 1] = '\0';
            printf("Enter Signal Number: ");
            getline(&lin2, &len, stdin);
            lin2[strlen(lin2) - 1] = '\0';
            send_signal(atoi(lin1), atoi(lin2));
        } else if (!strcmp(choice, "4")) {
            printf("Enter Signal Number: ");
            getline(&line, &len, stdin);
            line[strlen(line) - 1] = '\0';
            if (receive_signal(atoi(line)) == 1) {
                printf("Failed to receive signal\n");
            }
        } else if (!strcmp(choice, "5")) {
            printf("/This programm can send and receive signals to/from processes\n"
                   "/and create processes.\n"
                   ".To create process enter 1, next type the name of process\n"
                   " programm create process with this command.\n"
                   " (Enter name of programm on your computer)\n"
                   ".To create background process enter 2, type name\n"
                   " programm do the same things as create ordionary process,\n"
                   " but created process will running on another thread.\n"
                   " (Enter name of programm on your computer)\n"
                   ".To send signal enter 3, type Process ID, and number of\n"
                   " signal.\n"
                   " (PID: 777777, Signal: 15)\n"
                   ".To receive signal enter 4, type number of signal, when\n"
                   " process receive this signal, you will see message -\n"
                   " \"Signal - {number of signal} received successfully\"\n"
                   ".To get help enter 5.\n"
                   ".To exit enter 6.\n"
                   "Авторы: Полищук Никита, Дмитрий Мяконьких, Шиндель Эдуард\n");
            printf("Press Enter...");
            getchar();
        } else if (!strcmp(choice, "6")) {
            exit++;
        } else {
            printf("Wrong key\n");
        }
    }
    free(line);
    free(lin1);
    free(lin2);

    return 0;
}