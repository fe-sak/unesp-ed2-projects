#include "projeto.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
struct StudentHist {
  char id_aluno[4];
  char sigla_disc[4];
  char nome_aluno[50];
  char nome_disc[50];
  float media;
  float freq;
};

struct SecondaryIndex {
  char nome_aluno[50];
  int byteOffset;
};

struct Files {
  FILE *busca_p;
  FILE *busca_s;
  FILE *insere;

  FILE *data;
  FILE *p_index;
  FILE *s_index_1;
  FILE *s_index_2;
  FILE *last_inserted;
  FILE *last_removed;

  char busca_p_path[50];
  char busca_s_path[50];
  char insere_path[50];

  char data_path[50];
  char p_index_path[50];
  char s_index_1_path[50];
  char s_index_2_path[50];
  char last_inserted_path[50];
};

// globals
struct Files files = {
    NULL,
    NULL,
    NULL,

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
    "./output/s_index_1.bin",
    "./output/s_index_2.bin",
    "./output/last_inserted.bin",
};

// functions
FILE *load_input_file(char *file_path) {
  FILE *file_pointer = fopen(file_path, "rb");

  if (file_pointer == NULL) {
    printf("Não foi possível ler o arquivo %s\n", file_path);
    exit(1);
  }

  return file_pointer;
}

FILE *load_output_file(char *file_path) {
  FILE *file_pointer = fopen(file_path, "rb");

  if (file_pointer != NULL) {
    // file exists, open in write read mode
    fclose(file_pointer);
    file_pointer = fopen(file_path, "rb+");
  } else {
    // create file
    file_pointer = fopen(file_path, "wb+");
    printf("Arquivo %s criado\n", file_path);
  }

  return file_pointer;
}

void close_all_opened_files() {
  if (files.busca_p) {
    fclose(files.busca_p);
    files.busca_p = NULL;
  }
  if (files.busca_s) {
    fclose(files.busca_s);
    files.busca_s = NULL;
  }
  if (files.insere) {
    fclose(files.insere);
    files.insere = NULL;
  }
  if (files.data) {
    fclose(files.data);
    files.data = NULL;
  }
  if (files.p_index) {
    fclose(files.p_index);
    files.p_index = NULL;
  }
  if (files.s_index_1) {
    fclose(files.s_index_1);
    files.s_index_1 = NULL;
  }
  if (files.s_index_2) {
    fclose(files.s_index_2);
    files.s_index_2 = NULL;
  }
  if (files.last_inserted) {
    fclose(files.last_inserted);
    files.last_inserted = NULL;
  }
  if (files.last_removed) {
    fclose(files.last_removed);
    files.last_removed = NULL;
  }
}

struct Files get_files() {
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

int read_command() {
  int command;
  bool invalid_command = true;

  do {
    printf("\nOperações disponíveis ============================\n");
    printf("  1. Inserir\n");
    printf("  2. Procurar por {ID do aluno, Sigla da disciplina}\n");
    printf("  3. Procurar por Nome do aluno\n");
    printf("  4. Encerrar o programa\n");
    printf("--> ");

    scanf("%d", &command);

    invalid_command = !(command == 1 || command == 2 || command == 3 ||
                        command == 4 || command == 5);

    if (invalid_command) {
      printf("Operação invalida.\n\n");
    }

  } while ((invalid_command));

  return command;
}

void clean_stdin() {
  char trash;
  while ((trash = getchar()) != '\n' && trash != EOF) {
    printf("%c", trash);
  }
}

void insert_register() {
  files.insere = load_input_file(files.insere_path);

  // load students histories into memory
  int insert_registers_count = 0;
  struct StudentHist temp;
  while (fread(&temp, sizeof(struct StudentHist), 1, files.insere)) {
    insert_registers_count++;
  }

  rewind(files.insere);
  struct StudentHist studentHists[insert_registers_count];
  fread(&studentHists, sizeof(studentHists), 1, files.insere);

  // get last inserted
  files.last_inserted = load_output_file(files.last_inserted_path);
  int last_inserted = -1;
  fread(&last_inserted, sizeof(int), 1, files.last_inserted);
  rewind(files.last_inserted);
  if (last_inserted >= insert_registers_count - 1) {
    printf("\nTodos os registros do arquivo insere.bin foram inseridos, "
           "portanto nao eh possivel inserir mais registros.");
    return;
  }

  // write student history into data file
  last_inserted++;
  char buffer[200];
  snprintf(buffer, sizeof(buffer), "%s#%s#%s#%s#%.2f#%.2f",
           studentHists[last_inserted].id_aluno,
           studentHists[last_inserted].sigla_disc,
           studentHists[last_inserted].nome_aluno,
           studentHists[last_inserted].nome_disc,
           studentHists[last_inserted].media, studentHists[last_inserted].freq);

  int buffer_size = strlen(buffer);
  buffer[buffer_size] = '\0';
  buffer_size++;
  int studentHistorySize = buffer_size;

  files.data = load_output_file(files.data_path);
  fseek(files.data, 0, SEEK_END);
  fwrite(&buffer_size, sizeof(int), 1, files.data);
  fwrite(buffer, sizeof(char), buffer_size, files.data);

  long endOfInserted = ftell(files.data);
  long byteOffset = endOfInserted - studentHistorySize - sizeof(buffer_size);

  // write to last_inserted file
  fwrite(&last_inserted, sizeof(int), 1, files.last_inserted);

  // write to primary index
  files.p_index = load_output_file(files.p_index_path);
  snprintf(buffer, sizeof(buffer), "%s+%s+",
           studentHists[last_inserted].id_aluno,
           studentHists[last_inserted].sigla_disc);

  fseek(files.p_index, 0, SEEK_END);
  buffer_size = strlen(buffer);
  fwrite(buffer, sizeof(char), buffer_size, files.p_index);
  fwrite(&byteOffset, sizeof(byteOffset), 1, files.p_index);

  // write to secondary index

  files.s_index_1 = load_output_file(files.s_index_1_path);
  struct SecondaryIndex secondaryIndex;
  int found_byte_offset = -1;
  int name_size;

  while (fread(&name_size, sizeof(name_size), 1, files.s_index_1) == 1) {
    fread(secondaryIndex.nome_aluno, name_size, 1, files.s_index_1);
    fread(&secondaryIndex.byteOffset, sizeof(secondaryIndex.byteOffset), 1,
          files.s_index_1);

    if (strcmp(secondaryIndex.nome_aluno,
               studentHists[last_inserted].nome_aluno) == 0) {
      found_byte_offset = secondaryIndex.byteOffset;
      break;
    }

    memset(secondaryIndex.nome_aluno, 0, name_size);
  }

  printf("\nfoundByteOffset of s2: %d\n", found_byte_offset);

  if (found_byte_offset < 0) {
    // new secondary key case
    // write secondary key to s1
    files.s_index_2 = load_output_file(files.s_index_2_path);

    fseek(files.s_index_2, 0, SEEK_END);
    secondaryIndex.byteOffset = ftell(files.s_index_2);

    printf("SecondaryByteOffset: %d\n\n\n", secondaryIndex.byteOffset);

    strcpy(secondaryIndex.nome_aluno, studentHists[last_inserted].nome_aluno);

    printf("Nome: %s\n\n\n", secondaryIndex.nome_aluno);

    int registerSize = strlen(secondaryIndex.nome_aluno);

    fseek(files.s_index_1, 0, SEEK_END);
    fwrite(&registerSize, sizeof(registerSize), 1, files.s_index_1);

    fwrite(secondaryIndex.nome_aluno, strlen(secondaryIndex.nome_aluno), 1,
           files.s_index_1);
    fwrite(&secondaryIndex.byteOffset, sizeof(secondaryIndex.byteOffset), 1,
           files.s_index_1);

    // write primary key reference to s2
    // primarykey_byteoffset
    char primary_key[7] = "";
    strcat(primary_key, studentHists[last_inserted].id_aluno);
    strcat(primary_key, "+");
    strcat(primary_key, studentHists[last_inserted].sigla_disc);

    fwrite(primary_key, sizeof(primary_key), 1, files.s_index_2);

    int end_of_list = -1;
    fwrite(&end_of_list, sizeof(end_of_list), 1, files.s_index_2);
  } else {
    // secondary key already in s1 case
    // TODO bug: insert 7 times. First element of s2 is wrong
    files.s_index_2 = load_output_file(files.s_index_2_path);

    int prev_element_byte_offset;
    int next_element_byte_offset = found_byte_offset;
    char primary_key_buffer[8];
    printf("id_aluno: %s\n", studentHists[last_inserted].id_aluno);
    printf("found_byte_offset: %d\n", found_byte_offset);

    do {
      printf("\nnext_element_byte_offset: %d", next_element_byte_offset);
      // TODO here
      fseek(files.s_index_2, next_element_byte_offset, SEEK_SET);
      prev_element_byte_offset = ftell(files.s_index_2);

      fread(primary_key_buffer, sizeof(primary_key_buffer) - 1, 1,
            files.s_index_2);
      primary_key_buffer[8] = '\0';
      printf("\nprimaryKey: %s***", primary_key_buffer);

      fread(&next_element_byte_offset, sizeof(next_element_byte_offset), 1,
            files.s_index_2);

    } while (next_element_byte_offset != -1);

    char primary_key[7] = "";
    strcat(primary_key, studentHists[last_inserted].id_aluno);
    strcat(primary_key, "+");
    strcat(primary_key, studentHists[last_inserted].sigla_disc);

    fseek(files.s_index_2, 0, SEEK_END);
    int end_of_file = ftell(files.s_index_2);

    fseek(files.s_index_2, prev_element_byte_offset + strlen(primary_key),
          SEEK_SET);

    fwrite(&end_of_file, sizeof(end_of_file), 1, files.s_index_2);

    fseek(files.s_index_2, 0, SEEK_END);

    fwrite(primary_key, strlen(primary_key), 1, files.s_index_2);

    int end_of_list = -1;
    fwrite(&end_of_list, sizeof(end_of_list), 1, files.s_index_2);
  }
}

void remove_files() {
  remove(files.data_path);
  remove(files.p_index_path);
  remove(files.s_index_1_path);
  remove(files.s_index_2_path);
  remove(files.last_inserted_path);
}

void run_command(int command) {
  switch (command) {
  case 1:
    insert_register();
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
  case 5:
    remove_files();
    break;
  default:
    printf("\nOperação não reconhecida\n");
    break;
  };
}

int main() {
  int command;

  do {
    command = read_command();
    run_command(command);
    close_all_opened_files();
  } while (command != 4);

  printf("\n");
}
