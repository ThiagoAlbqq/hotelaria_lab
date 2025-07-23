#include "./quarto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 500
#define QUARTO_DB "./database/quarto.txt"

typedef enum {
  CASAL,
  CASAL_DUPLO,
  SOLTEIRO,
  SOLTEIRO_DUPLO,
  NUM_QUARTOS
} quartos_enum;

const char *quartos_names[NUM_QUARTOS] = {
    "Quarto de casal", "Quarto de casal duplo", "Quarto de solteiro",
    "Quarto de solteiro duplo"};

const int quartos_values[NUM_QUARTOS] = {200, 350, 100, 175};

void get_rooms() {
  FILE *arq = fopen(QUARTO_DB, "r");
  if (arq == NULL) {
    perror("Erro ao abrir o arquivo de Quartos");
    return;
  }

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), arq)) {
    int id;
    int tipo_id;
    char nome[MAX_NOME];
    char descricao[MAX_DESC];
    float diaria_por_pessoa;

    if (sscanf(line, "%d;%d;%[^;];%[^;];%f", &id, &tipo_id, nome, descricao,
               &diaria_por_pessoa) == 5) {
      Quarto *quarto_encontrado = (Quarto *)malloc(sizeof(Quarto));
      quarto_encontrado->id = id;
      quarto_encontrado->tipo_id = tipo_id;
      strcpy(quarto_encontrado->nome, nome);
      strcpy(quarto_encontrado->descricao, descricao);
      quarto_encontrado->diaria_por_pessoa = diaria_por_pessoa;

      printf("ID: %d, Tipo: %d, Nome: %s, Descrição: %s, Diária: %.2f\n",
             quarto_encontrado->id, quarto_encontrado->tipo_id,
             quarto_encontrado->nome, quarto_encontrado->descricao,
             quarto_encontrado->diaria_por_pessoa);
      free(quarto_encontrado);
    }
  }
  fclose(arq);
}

void get_room(int id_procurado) {
  FILE *arq = fopen(QUARTO_DB, "r");
  if (arq == NULL) {
    perror("Erro ao abrir o arquivo de Quartos");
    return;
  }

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), arq)) {
    int id;
    int tipo_id;
    char nome[MAX_NOME];
    char descricao[MAX_DESC];
    float diaria_por_pessoa;

    if (sscanf(line, "%d;%d;%[^;];%[^;];%f", &id, &tipo_id, nome, descricao,
               &diaria_por_pessoa) == 5) {
      if (id == id_procurado) {
        Quarto *quarto_encontrado = (Quarto *)malloc(sizeof(Quarto));
        if (quarto_encontrado == NULL) {
          perror("Erro de alocacao de memoria");
          continue;
        }

        quarto_encontrado->id = id;
        quarto_encontrado->tipo_id = tipo_id;
        strcpy(quarto_encontrado->nome, nome);
        strcpy(quarto_encontrado->descricao, descricao);
        quarto_encontrado->diaria_por_pessoa = diaria_por_pessoa;

        printf("ID: %d, Tipo: %d, Nome: %s, Descrição: %s, Diária: %.2f\n",
               quarto_encontrado->id, quarto_encontrado->tipo_id,
               quarto_encontrado->nome, quarto_encontrado->descricao,
               quarto_encontrado->diaria_por_pessoa);

        free(quarto_encontrado);
      }
    }
  }
  fclose(arq);
}

void create_room() {
  int id;
  int tipo_id;
  char nome[MAX_NOME];
  char descricao[MAX_DESC];
  float diaria_por_pessoa;

  printf("Digite o ID do quarto: ");
  scanf("%d", &id);

  printf("\n--- TIPO DE QUARTO ---\n");
  printf("Selecione o tipo para o quarto: \n");
  for (int i = 0; i < NUM_QUARTOS; i++) {
    printf("%d. %s\n", i + 1, quartos_names[i]);
  }
  printf("Escolha uma opcao: ");
  scanf("%d", &tipo_id);
  tipo_id--;

  sprintf(nome, "Quarto %d", id);

  strcpy(descricao, quartos_names[tipo_id]);
  diaria_por_pessoa = quartos_values[tipo_id];

  printf("Quarto criado: ID: %d, Tipo: %d, Nome: %s, Descrição: %s, Diária: "
         "%.2f\n",
         id, tipo_id, nome, descricao, diaria_por_pessoa);

  FILE *arq = fopen(QUARTO_DB, "a");
  char line[MAX_LINE_LENGTH];
  snprintf(line, sizeof(line), "\n%d;%d;%s;%s;%.2f", id, tipo_id, nome,
           descricao, diaria_por_pessoa);

  fprintf(arq, line);
  fclose(arq);

  printf("Quarto adicionado ao arquivo com sucesso!\n");
}