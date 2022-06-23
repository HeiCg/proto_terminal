#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h>
#include <sys/types.h> // define pid_t
#include <sys/wait.h>  // define wait
#include <unistd.h>    // fork()

void input(char *string) {
  printf("\033[0;34m");
  char pwd[FILENAME_MAX];
  getcwd(pwd, sizeof(pwd));
  printf("λ >> ");
  printf("\033[1;35m");
  printf("%s >> ", pwd);
  printf("\033[0;35m");
  scanf("%[^\n]%*c", string);
}

void split(char *string, char *spacer, char **vet) {
  int i = 0;
  char *token;
  token = strtok(string, spacer);
  while (token != NULL) {
    vet[i] = token;
    i++;
    token = strtok(NULL, spacer);
  }
}

void execBuiltinCommands(char **command) {
  if (strcmp(command[0], "cd") == 0) {
    printf("Entrei aqui caralho");
    chdir(command[1]);
  } else if (strcmp(command[0], "exit") == 0) {
    exit(0);
  }
}

int has_builtin_command(char **input) {
  if (strcmp(input[0], "cd") == 0 || strcmp(input[0], "exit") == 0) {
    return 1;
  }
  return 0;
}

void execCommands(char **commands) {

  if (has_builtin_command(commands) == 1) {
    execBuiltinCommands(commands);
  } else {
    printf("to me fudendo\n");
    pid_t pid = fork();
    if (pid < 0) {
      fprintf(stderr, "fork failed\n");
      exit(1);
    } else if (pid == 0) {
      execvp(commands[0], commands); // runs word count
    } else {
      wait(NULL);
    }
  }
}

// # TODO
// void clearInput(char **input) { }

// # TODO
// void has_ampersand(char **input ) {
// char ampersand[] = "&";
// char *has_ampersand = strstr(string, ampersand);
//}

int main(int argc, char **argv) {
  char string[100];
  char **arr = malloc(sizeof(char *) * 100);
  system("clear");
  while (1) {
    string[0] = '\0';
    input(string);
    split(string, " ", arr);
    execCommands(arr);
  }
}
