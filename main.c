#include <fcntl.h>
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

void execBuiltinCommands(char *command) {
  char **commands = malloc(sizeof(char *) * 100);
  split(command, " ", commands);
  if (strcmp(commands[0], "cd") == 0) {
    chdir(commands[1]);
  } else if (strcmp(commands[0], "exit") == 0) {
    exit(0);
  }
}

int has_builtin_command(char *input) {
  if ((strstr(input, "cd") != NULL) || (strstr(input, "exit") != NULL)) {
    return 1;
  }
  return 0;
}

int has_pipe(char *input) {
  if (strcmp(input, "|") == 0) {
    return 1;
  }
  return 0;
}

int has_bigger_than(char *input) {
  if (strcmp(input, ">") == 0) {
    return 1;
  }
  return 0;
}
int has_less_than(char *input) {
  if (strcmp(input, "<") == 0) {
    return 1;
  }
  return 0;
}

void run_with_bigger_than(char *input) {
  char **arr = malloc(sizeof(char *) * 100);
  split(input, ">", arr);
  close(STDOUT_FILENO);
  int out = open(arr[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

  char **commands = malloc(sizeof(char *) * 100);
  split(arr[0], " ", commands);

  execvp(commands[0], commands);
  close(out);
}

void run_with_less_than(char *input) {
  char **arr = malloc(sizeof(char *) * 100);
  split(input, "<", arr);
  close(0);
  int in = open(arr[1], O_RDONLY);

  char **commands = malloc(sizeof(char *) * 100);
  split(arr[0], " ", commands);

  execvp(commands[0], commands);
  close(in);
}

void exec(char *input, int whitout_wait) {
  char **arr = malloc(sizeof(char *) * 100);
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (pid == 0) {
    printf("Buguei caralho\n");
    // if (has_bigger_than(input)) {
    //   run_with_bigger_than(input);
    // } else if (has_less_than(input)) {
    //   run_with_less_than(input);
    // } else if (has_pipe(input)) {
    // }
  } else {
    if (whitout_wait == 1) {
      wait(NULL);
    }
  }
}

void execCommands(char *commands, int whitout_wait) {
  if (has_builtin_command(commands) == 1) {
    execBuiltinCommands(commands);
  } else {
    exec(commands, whitout_wait);
  }
}

int main(int argc, char **argv) {
  char string[100];
  while (1) {
    string[0] = '\0';
    input(string);
    execCommands(string, 0);
  }
}
