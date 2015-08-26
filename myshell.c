#include <stdio.h>
#include <string.h>
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

char* getCommandName(char* strings);
char* trimString(char* str, size_t size, int* length);
char** getCommandArgs();
void readCommand(char** command, char*** args);
void handleCommand(char* command, char** args, bool* shouldContinue);

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
    readCommand(&command, &args);
    handleCommand(command, args, &shouldContinue);
  } while (shouldContinue);

  return 0;
};

void handleCommand(char* command, char** args, bool* shouldContinue) {
  if (0 == strcmp(command, "quit")) {
    *shouldContinue = FALSE;
  } else if (0 < strlen(command)) {
    printf("launching command...\n");
  }
}

void readCommand(char** command, char*** args) {
  char* line;
  size_t size;
  int trimmedSize;
  char* trimmedCmd;

  getline(&line, &size, stdin);
  trimmedCmd = trimString(line, size, &trimmedSize);
  command[0] = getCommandName(trimmedCmd);
  args[0] = getCommandArgs();

  free(line);
  free(trimmedCmd);
};

char** getCommandArgs() {
  int i = 0;
  char** args = malloc(NUM_OF_ARGS);
  char* arg;
  while ((arg = strtok(NULL, " ")) != NULL) {
    args[i++] = arg;
  }
  return args;
}

char* getCommandName(char* strings) {
  return strtok(strings, " ");
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
