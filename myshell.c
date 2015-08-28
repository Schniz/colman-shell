#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_OF_ARGS 3
#define CLI_LENGTH 256

#define TRUE 1
#define FALSE 0

#define QUIT_CMD 0
#define EXTERNAL_CMD 1
#define NOP_CMD 2

typedef int CommandType;
typedef int bool;
typedef struct Process {
  int pid;
  int i;
  struct Process* next;
} Process;

char* trimString(char* str, size_t size, int* length);
char** getCommandArgs(char* trimmedCmd);
void readCommand(char*** args);
void handleCommand(char* command, char** args, bool* shouldContinue);

int processes = 0;
Process* firstProcess = NULL;

int main(int cargs, char* argv[]) {
  if (cargs < 2) {
    printf("Usage:\n");
    printf("------\n");
    printf("%s [name]\n", argv[0]);
    return 1;
  }

  bool shouldContinue = TRUE;
  char* shellName = argv[1];
  char** args = NULL;
  char* command = NULL;
  int i;

  do {
    printf("my_shell_%s>> ", shellName);
    readCommand(&args);
    if (args == NULL) {
      command = "";
    } else {
      command = args[0];
    }
    handleCommand(
      command,
      args,
      &shouldContinue
    );
    Process* proc = firstProcess;
    printf("processes: ");
    i = 0;
    while (proc != NULL) {
      printf("%i ", (int)proc->pid);
      proc = proc->next;
      i++;
    }
    printf("\n%i\n", i);
  } while (shouldContinue);

  return 0;
};

void handleCommand(char* command, char** args, bool* shouldContinue) {
  Process* oldProcess;
  Process* proc;
  int status;
  pid_t pid;
  int executed;

  if (command == NULL) {
    return;
  } else if (0 == strcmp(command, "quit")) {
    *shouldContinue = FALSE;
  } else if (0 < strlen(command)) {
    printf("launching command...\n");

    pid = fork();
    printf("pid = %i\n", pid);
    if (pid == 0) { // child process
      executed = execvp(args[0], args);
      if (executed < 0) {
        printf("error on execute: %i\n", executed);
        exit(1);
      }
    } else if (pid > 0) { // parent
      oldProcess = firstProcess;
      proc = malloc(sizeof(Process));
      proc->pid = pid;
      proc->next = oldProcess;
      proc->i = ++processes;
      firstProcess = proc;
      printf("%i\n", processes);
      while (wait(&status) != pid);
    } else { // error
      printf("errors");
      exit(1);
    }

    printf("status: %i\n", status);
  }
}

void readCommand(char*** args) {
  char* line;
  size_t size;
  int trimmedSize;
  char* trimmedCmd;

  getline(&line, &size, stdin);
  trimmedCmd = trimString(line, size, &trimmedSize);
  if (strlen(trimmedCmd) > 0) {
    args[0] = getCommandArgs(trimmedCmd);
  } else {
    args[0] = NULL;
  }

  free(line);
  free(trimmedCmd);
};

char** getCommandArgs(char* trimmedCmd) {
  int i = 0;
  char** args = malloc(NUM_OF_ARGS + 2);
  char* arg;
  arg = strtok(trimmedCmd, " ");
  while (arg != NULL) {
    args[i++] = arg;
    arg = strtok(NULL, " ");
  }
  args[i] = NULL;
  return args;
}

char* trimString(char* str, size_t size, int* length) {
  bool lastIsSpace = TRUE;
  char* almostTrimmed = malloc(size);
  char* trimmed = NULL;
  int charPlace = 0;
  int insertPlace = 0;

  for (charPlace = 0; charPlace < size; charPlace++) {
    if ((str[charPlace] == ' ') && (lastIsSpace)) {
      continue;
    } else if (str[charPlace] == ' ') {
      lastIsSpace = TRUE;
    } else {
      lastIsSpace = FALSE;
    }

    if (str[charPlace] == '\n') {
      almostTrimmed[insertPlace] = '\0';
      break;
    } else {
      almostTrimmed[insertPlace] = str[charPlace];
      insertPlace++;
    }
  }

  trimmed = malloc((size_t)insertPlace);
  for (charPlace = 0; charPlace < insertPlace; charPlace++) {
    trimmed[charPlace] = almostTrimmed[charPlace];
  }

  free(almostTrimmed);
  //free(almostTrimmed);
  length[0] = insertPlace - 1;
  return trimmed;
};
