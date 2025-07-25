#include "quarto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const float quartos_values[NUM_QUARTOS] = {200.0, 350.0, 100.0, 175.0};

// --- Funções CRUD ---
void get_rooms() {
  FILE *arq = fopen(QUARTO_DB, "r");
  if (arq == NULL) {
    perror("Erro ao abrir o arquivo de Quartos");
    return;
  }

  char line[MAX_LINE_LENGTH];
  int contador = 0;
  printf("\n--- LISTA DE QUARTOS ---\n");
  while (fgets(line, sizeof(line), arq) != NULL) {
    Quarto quarto;
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f", &quarto.id, &quarto.tipo_id,
               quarto.nome, quarto.descricao, &quarto.diaria_por_pessoa) == 5) {
      printf("ID: %d, Tipo ID: %d, Nome: %s, Descrição: %s, Diária: R$%.2f\n",
             quarto.id, quarto.tipo_id, quarto.nome, quarto.descricao,
             quarto.diaria_por_pessoa);
      contador++;
    }
  }

  if (contador == 0) {
    printf("Nenhum quarto cadastrado.\n");
  }
  printf("------------------------\n");

  fclose(arq);
}

int get_room(int id_procurado) {
  FILE *arq = fopen(QUARTO_DB, "r");
  if (arq == NULL) {
    perror("Erro ao abrir o arquivo de Quartos");
    return -1;
  }

  char line[MAX_LINE_LENGTH];
  int encontrado = 0;
  printf("\n--- BUSCA DE QUARTO (ID Procurado: %d) ---\n", id_procurado);
  int line_num = 0;

  while (fgets(line, sizeof(line), arq) != NULL) {
    line_num++;
    Quarto quarto;
    int sscanf_result =
        sscanf(line, "%d;%d;%[^;];%[^;];%f", &quarto.id, &quarto.tipo_id,
               quarto.nome, quarto.descricao, &quarto.diaria_por_pessoa);

    if (sscanf_result == 5) {
      if (quarto.id == id_procurado) {
        printf("ID: %d, Tipo ID: %d, Nome: %s, Descrição: %s, Diária: R$%.2f\n",
               quarto.id, quarto.tipo_id, quarto.nome, quarto.descricao,
               quarto.diaria_por_pessoa);
        encontrado = 1;
        break;
      }
    }
  }

  if (!encontrado) {
    printf("Quarto com ID %d não encontrado.\n", id_procurado);
  }
  printf("------------------------\n");

  fclose(arq);
  return encontrado;
}

void create_room() {
  int id;
  int tipo_id_escolhido;
  char nome[MAX_NOME];
  char descricao[MAX_DESC];
  float diaria_por_pessoa;

  printf("\n--- CRIAR NOVO QUARTO ---\n");
  printf("Digite o ID do quarto: ");
  scanf("%d", &id);
  while (getchar() != '\n')
    ;

  if (get_room(id)) {
    printf("O quarto de ID: %d, já existe.\n", id);
    return;
  }

  printf("\n--- TIPOS DE QUARTO DISPONÍVEIS ---\n");
  for (int i = 0; i < NUM_QUARTOS; i++) {
    printf("%d. %s (Diária: R$%.2f)\n", i + 1, quartos_names[i],
           quartos_values[i]);
  }

  do {
    printf("Escolha o número correspondente ao tipo de quarto: ");
    scanf("%d", &tipo_id_escolhido);
    while (getchar() != '\n')
      ;

    tipo_id_escolhido--;

    if (tipo_id_escolhido < 0 || tipo_id_escolhido >= NUM_QUARTOS) {
      printf("Opção inválida! Por favor, escolha um número entre 1 e %d.\n",
             NUM_QUARTOS);
    }
  } while (tipo_id_escolhido < 0 || tipo_id_escolhido >= NUM_QUARTOS);

  strcpy(descricao, quartos_names[tipo_id_escolhido]);
  diaria_por_pessoa = quartos_values[tipo_id_escolhido];
  snprintf(nome, MAX_NOME, "Quarto %d", id);

  printf("\nQuarto a ser criado:\n");
  printf("ID: %d, Tipo ID: %d, Nome: %s, Descrição: %s, Diária: R$%.2f\n", id,
         tipo_id_escolhido, nome, descricao, diaria_por_pessoa);

  FILE *arq = fopen(QUARTO_DB, "a");
  if (arq == NULL) {
    perror("Erro ao abrir o arquivo para escrita");
    return;
  }

  fprintf(arq, "%d;%d;%s;%s;%.2f\n", id, tipo_id_escolhido, nome, descricao,
          diaria_por_pessoa);
  fclose(arq);

  printf("Quarto adicionado ao arquivo com sucesso!\n");
}

void delete_room(int id_procurado) {
  FILE *original, *temp;
  int encontrado = 0;
  char line[MAX_LINE_LENGTH];
  Quarto quarto_lido;

  original = fopen(QUARTO_DB, "r");
  if (original == NULL) {
    printf("Erro: O arquivo '%s' não pode ser aberto para leitura.\n",
           QUARTO_DB);
    return;
  }

  temp = fopen(TEMP_DB, "w");
  if (temp == NULL) {
    printf("Erro: Não foi possível criar o arquivo temporário '%s'.\n",
           TEMP_DB);
    fclose(original);
    return;
  }

  while (fgets(line, sizeof(line), original) != NULL) {
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f", &quarto_lido.id,
               &quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
               &quarto_lido.diaria_por_pessoa) == 5) {
      if (quarto_lido.id == id_procurado) {
        encontrado = 1;
      } else {
        fprintf(temp, "%d;%d;%s;%s;%.2f\n", quarto_lido.id, quarto_lido.tipo_id,
                quarto_lido.nome, quarto_lido.descricao,
                quarto_lido.diaria_por_pessoa);
      }
    } else {
      fprintf(temp, "%s", line);
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
    printf("Erro: Quarto com ID %d não encontrado para exclusão.\n",
           id_procurado);
  }
}

void update_room(int id_procurado) {
  FILE *original, *temp;
  Quarto quarto_lido;
  int encontrado = 0;
  int novo_tipo_id;
  char line[MAX_LINE_LENGTH];

  printf("\n--- ATUALIZAR QUARTO ---\n");
  printf("\n--- TIPOS DE QUARTO DISPONÍVEIS ---\n");
  for (int i = 0; i < NUM_QUARTOS; i++) {
    printf("%d. %s (Diária: R$%.2f)\n", i + 1, quartos_names[i],
           quartos_values[i]);
  }

  do {
    printf(
        "Escolha o número correspondente ao NOVO tipo de quarto para o ID %d: ",
        id_procurado);
    scanf("%d", &novo_tipo_id);
    while (getchar() != '\n')
      ;

    novo_tipo_id--;

    if (novo_tipo_id < 0 || novo_tipo_id >= NUM_QUARTOS) {
      printf("Opção inválida! Por favor, escolha um número entre 1 e %d.\n",
             NUM_QUARTOS);
    }
  } while (novo_tipo_id < 0 || novo_tipo_id >= NUM_QUARTOS);

  original = fopen(QUARTO_DB, "r");
  if (original == NULL) {
    printf("Erro: O arquivo '%s' não pode ser aberto para leitura.\n",
           QUARTO_DB);
    return;
  }

  temp = fopen(TEMP_DB, "w");
  if (temp == NULL) {
    printf("Erro: Não foi possível criar o arquivo temporário '%s'.\n",
           TEMP_DB);
    fclose(original);
    return;
  }

  while (fgets(line, sizeof(line), original) != NULL) {
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f", &quarto_lido.id,
               &quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
               &quarto_lido.diaria_por_pessoa) == 5) {
      if (quarto_lido.id == id_procurado) {
        fprintf(temp, "%d;%d;%s;%s;%.2f\n", quarto_lido.id, novo_tipo_id,
                quarto_lido.nome, quartos_names[novo_tipo_id],
                quartos_values[novo_tipo_id]);
        encontrado = 1;
      } else {
        fprintf(temp, "%s", line);
      }
    } else {
      fprintf(temp, "%s", line);
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