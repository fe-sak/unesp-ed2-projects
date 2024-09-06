#include "projeto.h"
#include <stdbool.h>
#include <stdio.h>

// structs
struct Files {
  FILE *busca_p;
  FILE *busca_s;
  FILE *insere;
  FILE *data;
  FILE *primaryIndex;
  FILE *secondaryIndex;
};

// functions
int readCommand() {
  int command;
  bool invalidCommand = true;

  do {
    printf("\nOperações disponíveis ============================\n");
    printf("  1. Inserir\n");
    printf("  2. Procurar por {ID do aluno, Sigla da disciplina}\n");
    printf("  3. Procurar por Nome do aluno\n");
    printf("  4. Encerrar o programa\n");
    printf("--> ");

    scanf("%d", &command);

    invalidCommand =
        !(command == 1 || command == 2 || command == 3 || command == 4);

    if (invalidCommand) {
      printf("Operação invalida.\n\n");
    }

  } while ((invalidCommand));

  return command;
}

void runCommand(int command) {
  switch (command) {
  case 1:
    printf("\n1 não implementado\n");
    break;
  case 2:
    printf("\n2 não implementado\n");
    break;
  case 3:
    printf("\n3 não implementado\n");
    break;
  case 4:
    printf("\nPrograma encerrado ###############################\n");
    break;
  default:
    printf("\nOperação não reconhecida\n");
    break;
  };
}

int main() {
  int command;

  do {
    command = readCommand();
    runCommand(command);
  } while (command != 4);

  printf("\n");
}