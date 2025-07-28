#include "quarto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUARTO_DB "./database/quarto.txt"
#define TEMP_DB "./database/temp.txt"

void relatorio_quartos_mais_reservados() {
  FILE *file = fopen(QUARTO_DB, "r");
  if (!file) {
    printf("Erro ao abrir o arquivo de quartos.\n");
    return;
  }

  Quarto lista[500];
  int count = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), file) != NULL && count < 500) {
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f;%d;%d", &lista[count].id,
               &lista[count].tipo_id, lista[count].nome, lista[count].descricao,
               &lista[count].diaria_por_pessoa, &lista[count].reservas,
               &lista[count].capacidade) == 7) {
      count++;
    } else {
      fprintf(stderr, "Aviso: Linha malformada ignorada em %s: %s", QUARTO_DB,
              line);
    }
  }
  fclose(file);

  // Se nenhum quarto foi lido, informa e sai
  if (count == 0) {
    printf("\nNenhum quarto cadastrado para gerar o relatório.\n");
    return;
  }

  // Ordenar por número de reservas (do maior para o menor)
  for (int i = 0; i < count - 1; i++) {
    for (int j = i + 1; j < count; j++) {
      if (lista[i].reservas < lista[j].reservas) {
        Quarto temp = lista[i];
        lista[i] = lista[j];
        lista[j] = temp;
      }
    }
  }

  printf("\n=== RELATÓRIO DE QUARTOS MAIS RESERVADOS ===\n");
  for (int i = 0; i < count; i++) {
    printf("ID: %d | Tipo ID: %d | Nome: %s | Descrição: %s | Diária: "
           "R$%.2f | Reservas: %d | Capacidade: %d\n",
           lista[i].id, lista[i].tipo_id, lista[i].nome, lista[i].descricao,
           lista[i].diaria_por_pessoa, lista[i].reservas, lista[i].capacidade);
  }
  printf("===========================================\n");
}

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

const int quartos_capacities[NUM_QUARTOS] = {2, 4, 1, 2};

// --- Funções CRUD ---
void get_rooms() {
  FILE *arq = fopen(QUARTO_DB, "r");
  if (arq == NULL) {
    perror("Erro ao abrir o arquivo de Quartos");
    printf("Nenhum quarto cadastrado ainda.\n");
    return;
  }

  char line[MAX_LINE_LENGTH];
  int contador = 0;
  printf("\n--- LISTA DE QUARTOS ---\n");
  while (fgets(line, sizeof(line), arq) != NULL) {
    Quarto quarto;
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f;%d;%d", &quarto.id, &quarto.tipo_id,
               quarto.nome, quarto.descricao, &quarto.diaria_por_pessoa,
               &quarto.reservas, &quarto.capacidade) == 7) {
      printf("ID: %d | Tipo ID: %d | Nome: %s | Descrição: %s | Diária: R$%.2f "
             "| Reservas: %d | Capacidade: %d\n",
             quarto.id, quarto.tipo_id, quarto.nome, quarto.descricao,
             quarto.diaria_por_pessoa, quarto.reservas, quarto.capacidade);
      contador++;
    } else {
      fprintf(stderr, "Aviso: Linha malformada ignorada em %s: %s", QUARTO_DB,
              line);
    }
  }

  if (contador == 0) {
    printf("Nenhum quarto cadastrado.\n");
  }

  fclose(arq);
}

Quarto get_room_details(int id_procurado, int interno) {
  FILE *arq = fopen(QUARTO_DB, "r");
  if (arq == NULL) {
    perror("Erro ao abrir o arquivo de Quartos");
    Quarto q_erro;
    q_erro.id = -1;
    return q_erro;
  }

  char line[MAX_LINE_LENGTH];
  Quarto quarto;
  if (!interno) {
    printf("\n--- BUSCA DE QUARTO (ID Procurado: %d) ---\n", id_procurado);
  }
  while (fgets(line, sizeof(line), arq) != NULL) {
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f;%d;%d", &quarto.id, &quarto.tipo_id,
               quarto.nome, quarto.descricao, &quarto.diaria_por_pessoa,
               &quarto.reservas, &quarto.capacidade) == 7) {
      if (quarto.id == id_procurado) {
        if (!interno) {
          printf("ID: %d | Tipo ID: %d | Nome: %s | Descrição: %s | Diária: "
                 "R$%.2f | Reservas: %d | Capacidade: %d\n",
                 quarto.id, quarto.tipo_id, quarto.nome, quarto.descricao,
                 quarto.diaria_por_pessoa, quarto.reservas, quarto.capacidade);
        }
        fclose(arq);
        return quarto;
      }
    }
  }

  if (!interno) {
    printf("Quarto com ID %d não encontrado.\n", id_procurado);
  }
  fclose(arq);
  quarto.id = -1;
  return quarto;
}

int get_room(int id_procurado) {
  Quarto q = get_room_details(id_procurado, 1);
  return (q.id != -1);
}

void create_room() {
  int id;
  int tipo_id_escolhido;
  char nome[MAX_NOME];
  char descricao[MAX_DESC];
  float diaria_por_pessoa;
  int capacidade;

  printf("\n--- CRIAR NOVO QUARTO ---\n");
  printf("Digite o ID do quarto: ");
  if (scanf("%d", &id) != 1) {
    printf("Entrada inválida para o ID. Por favor, digite um número.\n");
    while (getchar() != '\n')
      ;
    return;
  }
  while (getchar() != '\n')
    ;

  if (get_room(id)) {
    printf(
        "O quarto com ID %d já existe. Por favor, escolha um ID diferente.\n",
        id);
    return;
  }

  printf("\n--- TIPOS DE QUARTO DISPONÍVEIS ---\n");
  for (int i = 0; i < NUM_QUARTOS; i++) {
    printf("%d. %s (Diária: R$%.2f, Capacidade: %d pessoas)\n", i + 1,
           quartos_names[i], quartos_values[i], quartos_capacities[i]);
  }

  do {
    printf("Escolha o número correspondente ao tipo de quarto: ");
    if (scanf("%d", &tipo_id_escolhido) != 1) {
      printf("Entrada inválida. Por favor, digite um número.\n");
      while (getchar() != '\n')
        ;
      tipo_id_escolhido = -1;
      continue;
    }
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
  capacidade = quartos_capacities[tipo_id_escolhido];
  snprintf(nome, MAX_NOME, "Quarto %d", id);

  printf("\nQuarto a ser criado:\n");
  printf("ID: %d | Tipo ID: %d | Nome: %s | Descrição: %s | Diária: R$%.2f | "
         "Reservas: %d | Capacidade: %d\n",
         id, tipo_id_escolhido, nome, descricao, diaria_por_pessoa, 0,
         capacidade);

  FILE *arq = fopen(QUARTO_DB, "a");
  if (arq == NULL) {
    perror("Erro ao abrir arquivo de quartos para escrita");
    return;
  }

  fprintf(arq, "%d;%d;%s;%s;%.2f;%d;%d\n", id, tipo_id_escolhido, nome,
          descricao, diaria_por_pessoa, 0, capacidade);
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
    printf("Erro: O arquivo '%s' não pode ser aberto para leitura (pode não "
           "existir).\n",
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
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f;%d;%d", &quarto_lido.id,
               &quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
               &quarto_lido.diaria_por_pessoa, &quarto_lido.reservas,
               &quarto_lido.capacidade) == 7) {
      if (quarto_lido.id == id_procurado) {
        encontrado = 1;
      } else {
        fprintf(temp, "%d;%d;%s;%s;%.2f;%d;%d\n", quarto_lido.id,
                quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
                quarto_lido.diaria_por_pessoa, quarto_lido.reservas,
                quarto_lido.capacidade);
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
  int novo_tipo_id_escolhido;
  char line[MAX_LINE_LENGTH];

  printf("\n--- ATUALIZAR QUARTO (ID: %d) ---\n", id_procurado);

  if (!get_room(id_procurado)) {
    printf("Quarto com ID %d não encontrado para atualização.\n", id_procurado);
    return;
  }

  printf("\n--- TIPOS DE QUARTO DISPONÍVEIS PARA ATUALIZAÇÃO ---\n");
  for (int i = 0; i < NUM_QUARTOS; i++) {
    printf("%d. %s (Diária: R$%.2f, Capacidade: %d pessoas)\n", i + 1,
           quartos_names[i], quartos_values[i], quartos_capacities[i]);
  }

  do {
    printf(
        "Escolha o número correspondente ao NOVO tipo de quarto para o ID %d: ",
        id_procurado);
    if (scanf("%d", &novo_tipo_id_escolhido) != 1) {
      printf("Entrada inválida. Por favor, digite um número.\n");
      while (getchar() != '\n')
        ;
      novo_tipo_id_escolhido = -1;
      continue;
    }
    while (getchar() != '\n')
      ;

    novo_tipo_id_escolhido--;

    if (novo_tipo_id_escolhido < 0 || novo_tipo_id_escolhido >= NUM_QUARTOS) {
      printf("Opção inválida! Por favor, escolha um número entre 1 e %d.\n",
             NUM_QUARTOS);
    }
  } while (novo_tipo_id_escolhido < 0 || novo_tipo_id_escolhido >= NUM_QUARTOS);

  original = fopen(QUARTO_DB, "r");
  if (original == NULL) {
    perror("Erro ao abrir arquivo original de quartos para leitura");
    return;
  }

  temp = fopen(TEMP_DB, "w");
  if (temp == NULL) {
    perror("Erro ao criar arquivo temporário de quartos para escrita");
    fclose(original);
    return;
  }

  while (fgets(line, sizeof(line), original) != NULL) {
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f;%d;%d", &quarto_lido.id,
               &quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
               &quarto_lido.diaria_por_pessoa, &quarto_lido.reservas,
               &quarto_lido.capacidade) == 7) {
      if (quarto_lido.id == id_procurado) {
        fprintf(temp, "%d;%d;%s;%s;%.2f;%d;%d\n", quarto_lido.id,
                novo_tipo_id_escolhido, quarto_lido.nome,
                quartos_names[novo_tipo_id_escolhido],
                quartos_values[novo_tipo_id_escolhido], quarto_lido.reservas,
                quartos_capacities[novo_tipo_id_escolhido]);
        encontrado = 1;
        quarto_lido.tipo_id = novo_tipo_id_escolhido;
        strcpy(quarto_lido.descricao, quartos_names[novo_tipo_id_escolhido]);
        quarto_lido.diaria_por_pessoa = quartos_values[novo_tipo_id_escolhido];
        quarto_lido.capacidade = quartos_capacities[novo_tipo_id_escolhido];
      } else {
        fprintf(temp, "%d;%d;%s;%s;%.2f;%d;%d\n", quarto_lido.id,
                quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
                quarto_lido.diaria_por_pessoa, quarto_lido.reservas,
                quarto_lido.capacidade);
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
    printf("\nQuarto com ID %d atualizado com sucesso!\n", id_procurado);
    printf("ID: %d | Tipo ID: %d | Nome: %s | Descrição: %s | Diária: R$%.2f | "
           "Reservas: %d | Capacidade: %d\n",
           quarto_lido.id, quarto_lido.tipo_id, quarto_lido.nome,
           quarto_lido.descricao, quarto_lido.diaria_por_pessoa,
           quarto_lido.reservas, quarto_lido.capacidade);
  } else {
    remove(TEMP_DB);
    printf("Erro: Quarto com ID %d não encontrado para atualização.\n",
           id_procurado);
  }
}

void adicionar_reserva(int id_procurado) {
  FILE *original, *temp;
  Quarto quarto_lido;
  int encontrado = 0;
  int novo_tipo_id_escolhido;
  char line[MAX_LINE_LENGTH];

  printf("\n--- ATUALIZAR QUARTO (ID: %d) ---\n", id_procurado);

  if (!get_room(id_procurado)) {
    printf("Quarto com ID %d não encontrado para atualização.\n", id_procurado);
    return;
  }

  original = fopen(QUARTO_DB, "r");
  if (original == NULL) {
    perror("Erro ao abrir arquivo original de quartos para leitura");
    return;
  }

  temp = fopen(TEMP_DB, "w");
  if (temp == NULL) {
    perror("Erro ao criar arquivo temporário de quartos para escrita");
    fclose(original);
    return;
  }

  while (fgets(line, sizeof(line), original) != NULL) {
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f;%d;%d", &quarto_lido.id,
               &quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
               &quarto_lido.diaria_por_pessoa, &quarto_lido.reservas,
               &quarto_lido.capacidade) == 7) {
      if (quarto_lido.id == id_procurado) {
        quarto_lido.reservas++;
        fprintf(temp, "%d;%d;%s;%s;%.2f;%d;%d\n", quarto_lido.id,
                quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
                quarto_lido.diaria_por_pessoa, quarto_lido.reservas,
                quarto_lido.capacidade);
        encontrado = 1;
        quarto_lido.tipo_id = novo_tipo_id_escolhido;
        strcpy(quarto_lido.descricao, quartos_names[novo_tipo_id_escolhido]);
        quarto_lido.diaria_por_pessoa = quartos_values[novo_tipo_id_escolhido];
        quarto_lido.capacidade = quartos_capacities[novo_tipo_id_escolhido];
      } else {
        fprintf(temp, "%d;%d;%s;%s;%.2f;%d;%d\n", quarto_lido.id,
                quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
                quarto_lido.diaria_por_pessoa, quarto_lido.reservas,
                quarto_lido.capacidade);
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
    printf("\nQuarto com ID %d atualizado com sucesso!\n", id_procurado);
    printf("ID: %d | Tipo ID: %d | Nome: %s | Descrição: %s | Diária: R$%.2f | "
           "Reservas: %d | Capacidade: %d\n",
           quarto_lido.id, quarto_lido.tipo_id, quarto_lido.nome,
           quarto_lido.descricao, quarto_lido.diaria_por_pessoa,
           quarto_lido.reservas, quarto_lido.capacidade);
  } else {
    remove(TEMP_DB);
    printf("Erro: Quarto com ID %d não encontrado para atualização.\n",
           id_procurado);
  }
}

void remover_reserva(int id_procurado) {
  FILE *original, *temp;
  Quarto quarto_lido;
  int encontrado = 0;
  int novo_tipo_id_escolhido;
  char line[MAX_LINE_LENGTH];

  printf("\n--- ATUALIZAR QUARTO (ID: %d) ---\n", id_procurado);

  if (!get_room(id_procurado)) {
    printf("Quarto com ID %d não encontrado para atualização.\n", id_procurado);
    return;
  }

  original = fopen(QUARTO_DB, "r");
  if (original == NULL) {
    perror("Erro ao abrir arquivo original de quartos para leitura");
    return;
  }

  temp = fopen(TEMP_DB, "w");
  if (temp == NULL) {
    perror("Erro ao criar arquivo temporário de quartos para escrita");
    fclose(original);
    return;
  }

  while (fgets(line, sizeof(line), original) != NULL) {
    if (sscanf(line, "%d;%d;%[^;];%[^;];%f;%d;%d", &quarto_lido.id,
               &quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
               &quarto_lido.diaria_por_pessoa, &quarto_lido.reservas,
               &quarto_lido.capacidade) == 7) {
      if (quarto_lido.id == id_procurado) {
        quarto_lido.reservas--;
        fprintf(temp, "%d;%d;%s;%s;%.2f;%d;%d\n", quarto_lido.id,
                quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
                quarto_lido.diaria_por_pessoa, quarto_lido.reservas,
                quarto_lido.capacidade);
        encontrado = 1;
        quarto_lido.tipo_id = novo_tipo_id_escolhido;
        strcpy(quarto_lido.descricao, quartos_names[novo_tipo_id_escolhido]);
        quarto_lido.diaria_por_pessoa = quartos_values[novo_tipo_id_escolhido];
        quarto_lido.capacidade = quartos_capacities[novo_tipo_id_escolhido];
      } else {
        fprintf(temp, "%d;%d;%s;%s;%.2f;%d;%d\n", quarto_lido.id,
                quarto_lido.tipo_id, quarto_lido.nome, quarto_lido.descricao,
                quarto_lido.diaria_por_pessoa, quarto_lido.reservas,
                quarto_lido.capacidade);
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
    printf("\nQuarto com ID %d atualizado com sucesso!\n", id_procurado);
    printf("ID: %d | Tipo ID: %d | Nome: %s | Descrição: %s | Diária: R$%.2f | "
           "Reservas: %d | Capacidade: %d\n",
           quarto_lido.id, quarto_lido.tipo_id, quarto_lido.nome,
           quarto_lido.descricao, quarto_lido.diaria_por_pessoa,
           quarto_lido.reservas, quarto_lido.capacidade);
  } else {
    remove(TEMP_DB);
    printf("Erro: Quarto com ID %d não encontrado para atualização.\n",
           id_procurado);
  }
}