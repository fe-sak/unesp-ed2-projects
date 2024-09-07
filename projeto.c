#include "projeto.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// structs
struct Register {
  char id_aluno[4];
  char sigla_disc[4];
  char nome_aluno[50];
  char nome_disc[50];
  float media;
  float freq;
};

struct Files {
  FILE *busca_p;
  FILE *busca_s;
  FILE *insere;

  FILE *data;
  FILE *p_index;
  FILE *s_index;
  FILE *last_inserted;
  FILE *last_removed;

  char busca_p_path[50];
  char busca_s_path[50];
  char insere_path[50];

  char data_path[50];
  char p_index_path[50];
  char s_index_path[50];
  char last_inserted_path[50];
  char last_removed_path[50];
};

// globals
struct Files files = {
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,

    "./input/busca_p.bin",
    "./input/busca_s.bin",
    "./input/insere.bin",

    "./output/data.bin",
    "./output/p_index.bin",
    "./output/s_index.bin",
};

// functions
void load_input_file(FILE **file_pointer, char *file_path) {
  *file_pointer = fopen(file_path, "rb");

  if (file_pointer == NULL) {
    printf("Não foi possível ler o arquivo %s\n", file_path);
    exit(1);
  }
}

void load_output_file(FILE **file_pointer, char *file_path) {
  *file_pointer = fopen(file_path, "rb");

  if (file_pointer != NULL) {
    // file exists, open in write read mode
    fclose(*file_pointer);
    *file_pointer = fopen(file_path, "rb+");
  } else {
    // create file
    *file_pointer = fopen(file_path, "wb+");
    printf("\nArquivo %s criado\n", file_path);
  }
}

void closeAllOpenedFiles() {
  if (files.busca_p) {
    fclose(files.busca_p);
  }
  if (files.busca_s) {
    fclose(files.busca_s);
  }
  if (files.data) {
    fclose(files.data);
  }
  if (files.insere) {
    fclose(files.insere);
  }
  if (files.p_index) {
    fclose(files.p_index);
  }
  if (files.s_index) {
    fclose(files.s_index);
  }
}

struct Files getFiles() {
  printf("\nLendo arquivos\n");

  // loadInputFile(files.busca_p, files.busca_p_path);
  // loadInputFile(files.busca_s, files.busca_s_path);
  // loadInputFile(files.insere, files.insere_path);

  // loadOutputFile(files.data, files.data_path);
  // loadOutputFile(files.p_index, files.p_index_path);
  // loadOutputFile(files.s_index, files.s_index_path);

  printf("\nArquivos lidos com sucesso\n");

  return files;
}

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

void cleanStdIn() {
  char trash;
  while ((trash = getchar()) != '\n' && trash != EOF) {
    printf("%c", trash);
  }
}

void insertRegister() {
  load_input_file(&files.insere, files.insere_path);

  // load insertRegisters into memory
  int insertRegistersCount = 0;
  struct Register r;

  while (fread(&r, sizeof(struct Register), 1, files.insere)) {
    insertRegistersCount++;
  }

  rewind(files.insere);

  struct Register registers[insertRegistersCount];

  fread(&registers, sizeof(registers), 1, files.insere);

  fclose(files.insere);
}

void runCommand(int command) {
  switch (command) {
  case 1:
    insertRegister();
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

  closeAllOpenedFiles();

  printf("\n");
}
