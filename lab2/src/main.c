#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
int numname(const char *name) {
  while (*name) {
    if (isdigit(*name++) == 0) {
      return 1;
    }
  }
  return 0;
}

void sizedir(const char *name, int *amount) {
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(name))) {
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    char path[1024];
    if (entry->d_type == DT_DIR) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }
      snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
      sizedir(path, amount);
    } else {
      struct stat statbuf;
      snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
      stat(path, &statbuf);
      *amount += statbuf.st_size;
    }
  }

  closedir(dir);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("See help for usage examples\n./fsmanager -h\n");
    return 0;
  }

  if (strcmp(argv[1], "-m") == 0) {
    if (argc == 4) {
      if (rename(argv[2], argv[3]) == -1) {
        printf("Error during moving file\n");
        return -1;
      }
    } else {
      printf("Wrong amount of arguements\n");
      return -1;
    }
  } else if (strcmp(argv[1], "-c") == 0) {
    if (argc == 4) {
      FILE *source = fopen(argv[2], "r"), *dest = fopen(argv[3], "w+");
      char byte;
      while ((byte = fgetc(source)) != EOF) {
        fputc(byte, dest);
      }
      fclose(source);
      fclose(dest);
    } else {
      printf("Wrong amount of arguements\n");
      return -1;
    }
  } else if (strcmp(argv[1], "-d") == 0) {
    if (argc == 3) {
      if (remove(argv[2]) == -1) {
        printf("Error during deleting file\n");
        return -1;
      }
    } else {
      printf("Wrong amount of arguements\n");
      return -1;
    }

  } else if (strcmp(argv[1], "-s") == 0) {
    if (argc == 3) {
      struct stat statst;
      if (stat(argv[2], &statst) != 0) {
        return -1;
      }

      int amount = 0;
      if (S_ISDIR(statst.st_mode)) {
        amount = statst.st_size;
        sizedir(argv[2], &amount);
      } else {
        amount = statst.st_size;
      }
      printf("%s -- %d bytes\n", argv[2], amount);

      return 0;
    } else {
      printf("Wrong amount of arguements\n");
      return -1;
    }
  } else if (strcmp(argv[1], "-ls") == 0) {
    if (argc == 3) {
      DIR *curdir = opendir(argv[2]);

      struct dirent *ep;
      printf("%s :\n", curdir);

      while (ep = readdir(curdir)) {
        if (strncmp(ep->d_name, ".", 1)) {
          struct stat statst;
          if (stat(ep->d_name, &statst) != 0) {
            return -1;
          }
          if (S_ISDIR(statst.st_mode)) {
            printf("dir\t%s/\n", ep->d_name);
          } else {
            printf("file\t%s\n", ep->d_name);
          }
        }
      }
      closedir(curdir);
    } else {
      printf("Wrong amount of arguements\n");
      return -1;
    }

  } else if (strcmp(argv[1], "-procfs") == 0) {
    if (argc == 2) {
      DIR *dir;
      struct dirent *entry;

      if (!(dir = opendir("/proc"))) {
        return -1;
      }

      while ((entry = readdir(dir)) != NULL) {
        if (numname(entry->d_name) == 0) {
          FILE *op = NULL;
          char filename[255];
          sprintf(filename, "%s/%s/comm", "/proc", entry->d_name);
          op = fopen(filename, "r");
          fscanf(op, "%[^\n]", filename);
          fclose(op);
          printf("%s\t%s\n", entry->d_name, filename);
        }
      }
    } else {
      printf("Wrong amount of arguements\n");
      return -1;
    }
  } else if (strcmp(argv[1], "-h") == 0) {
    printf(
        "Авторы: Мяконьких Дмитрий, Шиндель Эдуард, Полищук Никита\nДоступные "
        "аргументы:\n"
        "-m - перемещение файла\n"
        "Пример: fsmanager -m ./samplefile ./1/samplefile\n"
        "-c - копирование файла\n"
        "Пример: fsmanager -c ./samplefile ./2/samplefile\n"
        "-d - удаление файла\n"
        "Пример: fsmanager -d ./samplefile\n"
        "-s - подсчет размера файла/директории\n"
        "Пример: fsmanager -s ./1\n"
        "-ls - вывод содержимого папки\n"
        "Пример: fsmanager -ls ./samplefile ./1/samplefile\n"
        "-procfs - вывод данных из procfs\n"
        "Пример: fsmanager -procfs\n");
  } else {
    printf("See help for usage examples\n./fsmanager -h\n");
  }
}
