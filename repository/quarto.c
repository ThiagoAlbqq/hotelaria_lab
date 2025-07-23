#include "./quarto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 500
#define QUARTO_DB "./database/quarto.txt"
#define TEMP_DB "./database/temp.txt"

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
  int contador = 0;
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
      contador++;
    }
  }

  if (contador == 0) {
    printf("Nenhum quarto cadastrado\n");
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

void delete_room(int id_procurado) {
  FILE *original, *temp;
  Quarto quarto;
  int encontrado = 0;

  original = fopen(QUARTO_DB, "r");
  if (original == NULL) {
    printf("Erro: O arquivo %s não pode ser aberto para leitura.\n", QUARTO_DB);
    return;
  }

  temp = fopen(TEMP_DB, "w");
  if (temp == NULL) {
    printf("Erro: Não foi possível criar o arquivo temporário.\n");
    fclose(original);
    return;
  }

  while (fscanf(original, "%d;%d;%[^;];%[^;];%f", &quarto.id, &quarto.tipo_id,
                quarto.nome, &quarto.descricao,
                &quarto.diaria_por_pessoa) == 5) {
    if (quarto.id == id_procurado) {
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%d;%s;%s;%.2f\n", quarto.id, quarto.tipo_id,
              quarto.nome, quarto.descricao, quarto.diaria_por_pessoa);
    }
  }

  fclose(original);
  fclose(temp);

  if (encontrado) {
    remove(QUARTO_DB);
    rename(TEMP_DB, QUARTO_DB);
    printf("Quarto com ID %d deletado com sucesso.\n", id_procurado);
  } else {
    remove(TEMP_DB);
    printf("Erro: Quarto com ID %d não encontrado.\n", id_procurado);
  }
}

void update_room(int id_procurado) {
  FILE *original, *temp;
  Quarto quarto_lido;
  int encontrado = 0;

  int tipo_id;

  printf("\n--- TIPO DE QUARTO ---\n");
  printf("Selecione o tipo para o quarto: \n");
  for (int i = 0; i < NUM_QUARTOS; i++) {
    printf("%d. %s\n", i + 1, quartos_names[i]);
  }
  printf("Escolha uma opcao: ");
  scanf("%d", &tipo_id);
  tipo_id--;

  original = fopen(QUARTO_DB, "r");
  if (original == NULL) {
    printf("Erro: O arquivo %s não pode ser aberto para leitura.\n", QUARTO_DB);
    return;
  }

  temp = fopen(TEMP_DB, "w");
  if (temp == NULL) {
    printf("Erro: Não foi possível criar o arquivo temporário.\n");
    fclose(original);
    return;
  }

  while (fscanf(original, "%d;%d;%[^;];%[^;];%f", &quarto_lido.id,
                &quarto_lido.tipo_id, quarto_lido.nome, &quarto_lido.descricao,
                &quarto_lido.diaria_por_pessoa) == 5) {
    if (quarto_lido.id == id_procurado) {
      fprintf(temp, "%d;%d;%s;%s;%.2f\n", quarto_lido.id, tipo_id,
              quarto_lido.nome, quarto_lido.descricao,
              quarto_lido.diaria_por_pessoa);
      encontrado = 1;
    } else {
      fprintf(temp, "%d;%d;%s;%s;%.2f\n", quarto_lido.id, quarto_lido.tipo_id,
              quarto_lido.nome, quarto_lido.descricao,
              quarto_lido.diaria_por_pessoa);
    }
  }

  fclose(original);
  fclose(temp);

  if (encontrado) {
    remove(QUARTO_DB);
    rename(TEMP_DB, QUARTO_DB);
    printf("Quarto com ID %d atualizado com sucesso!\n", id_procurado);
  } else {
    remove(TEMP_DB);
    printf("Erro: Quarto com ID %d não encontrado para atualização.\n",
           id_procurado);
  }
}