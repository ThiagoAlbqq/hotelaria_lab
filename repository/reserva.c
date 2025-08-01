#include "reserva.h"
#include "../utils/clear_terminal.h"
#include "../utils/data.h"
#include "../utils/interface.h"
#include "cliente.h"
#include "quarto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESERVA_DB "./database/reserva.txt"
#define TEMP "./database/temp_reserva.txt"

// Gera um novo ID automático
int gerar_novo_id_reserva() {
  FILE *file = fopen(RESERVA_DB, "r");
  if (!file)
    return 1;
  int max_id = 0, id;
  char linha[300];
  while (fgets(linha, sizeof(linha), file)) {
    sscanf(linha, "%d;", &id);
    if (id > max_id)
      max_id = id;
  }
  fclose(file);
  return max_id + 1;
}

int verificar_disponibilidade_quarto(int quarto_id, Data checkin,
                                     Data checkout) {
  FILE *file = fopen(RESERVA_DB, "r");
  if (!file) {
    return 0;
  }

  char linha[300];
  Reserva temp_reserva;
  int id, q_id, c_id, qtd_pessoas;
  float preco;
  char s_check_in[11], s_check_out[11];

  while (fgets(linha, sizeof(linha), file)) {
    sscanf(linha, "%d;%d;%d;%[^;];%[^;];%d;%f", &id, &q_id, &c_id, s_check_in,
           s_check_out, &qtd_pessoas, &preco);

    if (q_id == quarto_id) {
      Data res_checkin, res_checkout;
      sscanf(s_check_in, "%d/%d/%d", &res_checkin.dia, &res_checkin.mes,
             &res_checkin.ano);
      sscanf(s_check_out, "%d/%d/%d", &res_checkout.dia, &res_checkout.mes,
             &res_checkout.ano);

      if (comparar_datas(checkin, res_checkout) <= 0 &&
          comparar_datas(res_checkin, checkout) <= 0) {
        fclose(file);
        return 1; // Quarto não disponível
      }
    }
  }
  fclose(file);
  return 0; // Quarto disponível
}

// Função para procurar reserva por id
void get_reserva(int id_procurado) {
  FILE *reserva = fopen(RESERVA_DB, "r");
  char line[MAX_LINE_LENGTH];

  if (reserva == NULL) {
    perror("Erro ao abrir o arquivo reservas.");
    return;
  }

  int encontrado = 0;

  while (fgets(line, sizeof(line), reserva)) {
    char copy_line[MAX_LINE_LENGTH];
    strcpy(copy_line, line);

    char *token = strtok(copy_line, ";");
    if (token != NULL && atoi(token) == id_procurado) {
      // Quebra os campos
      int id = atoi(token);
      int quarto_id = atoi(strtok(NULL, ";"));
      int cliente_id = atoi(strtok(NULL, ";"));
      char *data_entrada = strtok(NULL, ";");
      char *data_saida = strtok(NULL, ";");
      int num_pessoas = atoi(strtok(NULL, ";"));
      float valor_total = atof(strtok(NULL, ";"));

      printf("Reserva encontrada: \n");
      printf("ID: %d || Quarto: %d || Cliente: %d || Entrada: %s || Saída: %s "
             "|| Pessoas: %d || Valor: R$ %.2f\n",
             id, quarto_id, cliente_id, data_entrada, data_saida, num_pessoas,
             valor_total);

      encontrado = 1;
      break;
    }
  }

  if (!encontrado) {
    printf("Falha ao encontrar reserva com ID %d.\n", id_procurado);
  }

  fclose(reserva);
}

// Listar reservas
void get_reservas() {
  FILE *reserva = fopen(RESERVA_DB, "r");
  char line[MAX_LINE_LENGTH];

  if (reserva == NULL) {
    perror("Erro ao abrir o arquivo de reservas.");
    return;
  }

  printf("Lista de Reservas:\n");

  int reserva_ok = 0;

  while (fgets(line, sizeof(line), reserva)) {
    reserva_ok = 1;
    char copy_line[MAX_LINE_LENGTH];
    strcpy(copy_line, line);

    char *token = strtok(copy_line, ";");
    int id = atoi(token);
    int quarto_id = atoi(strtok(NULL, ";"));
    int cliente_id = atoi(strtok(NULL, ";"));
    char *data_entrada = strtok(NULL, ";");
    char *data_saida = strtok(NULL, ";");
    int num_pessoas = atoi(strtok(NULL, ";"));
    float valor_total = atof(strtok(NULL, ";"));

    printf("ID: %d || Quarto: %d || Cliente: %d || Entrada: %s || Saída: %s || "
           "Pessoas: %d || Valor: R$ %.2f\n",
           id, quarto_id, cliente_id, data_entrada, data_saida, num_pessoas,
           valor_total);
  }

  if (!reserva_ok) {
    printf("Nenhuma reserva encontrada.\n");
  }

  fclose(reserva);
}

void create_reserva() {
  Reserva nova;
  nova.id = gerar_novo_id_reserva();
  Quarto quarto_selecionado;

  int quarto_existe = -1;
  do {
    clearTerminal();
    get_rooms();
    printf("Digite o ID do Quarto: ");
    scanf("%d", &nova.quarto_id);
    limpar_buffer();
    quarto_selecionado = get_room_details(nova.quarto_id, 1);
    if (quarto_selecionado.id == -1) {
      printf("O quarto informado não existe...\n");
      esperar_enter();
    } else {
      quarto_existe = 0; // Quarto encontrado
    }
  } while (quarto_existe == -1);

  // Validar cliente
  int cliente_existe = -1;
  do {
    clearTerminal();
    if (listar_clientes() != 0) {
      printf("\nDigite o ID do Cliente: ");
      scanf("%d", &nova.cliente_id);
      limpar_buffer();
      cliente_existe = buscar_cliente_por_id(nova.cliente_id);
      if (cliente_existe == -1) {
        printf("O cliente informado não existe...\n");
        esperar_enter();
      }
    } else {
      printf("\nCrie algum cliente antes de criar a reserva...\n");
      break;
    }
  } while (cliente_existe == -1);

  if (cliente_existe == -1)
    return;

  // Datas
  Data checkin, checkout;
  int datas_ok = 0;
  Data data_atual = get_data_atual();

  do {
    clearTerminal();
    checkin = ler_data_completa("Digite a data de Check-in");
    checkout = ler_data_completa("Digite a data de Check-out");

    if (comparar_datas(checkin, checkout) >= 0) {
      printf("A data de Check-out (%02d/%02d/%04d) deve ser posterior à data "
             "de Check-in (%02d/%02d/%04d). Tente novamente.\n",
             checkout.dia, checkout.mes, checkout.ano, checkin.dia, checkin.mes,
             checkin.ano);
      esperar_enter();
    } else if (comparar_datas(checkin, data_atual) < 0) {
      printf("A data de Check-in (%02d/%02d/%04d) não pode ser anterior à data "
             "atual (%02d/%02d/%04d). Tente novamente.\n",
             checkin.dia, checkin.mes, checkin.ano, data_atual.dia,
             data_atual.mes, data_atual.ano);
      esperar_enter();
    } else if (verificar_disponibilidade_quarto(nova.quarto_id, checkin,
                                                checkout) == 1) {
      printf("O quarto não está disponível para as datas selecionadas. Por "
             "favor, escolha outras datas ou outro quarto.\n");
      esperar_enter();
    } else {
      datas_ok = 1;
    }
  } while (!datas_ok);

  formatar_data(checkin, nova.check_in);
  formatar_data(checkout, nova.check_out);

  do {
    printf("Digite a quantidade de pessoas: ");
    scanf("%d", &nova.quantidade_pessoas);
    limpar_buffer();
    if (nova.quantidade_pessoas <= 0) {
      printf("A quantidade de pessoas deve ser maior que zero. Tente "
             "novamente.\n");
    } else if (nova.quantidade_pessoas > quarto_selecionado.capacidade) {
      printf("A quantidade de pessoas (%d) excede a capacidade máxima do "
             "quarto (%d). Tente novamente.\n",
             nova.quantidade_pessoas, quarto_selecionado.capacidade);
    } else {
      break;
    }
  } while (1);

  nova.preco_total = quarto_selecionado.diaria;
  adicionar_reserva(quarto_selecionado.id);

  FILE *file = fopen(RESERVA_DB, "a");
  if (!file) {
    printf("Erro ao abrir o banco de dados de reservas.\n");
    return;
  }

  fprintf(file, "%d;%d;%d;%s;%s;%d;%.2f\n", nova.id, nova.quarto_id,
          nova.cliente_id, nova.check_in, nova.check_out,
          nova.quantidade_pessoas, nova.preco_total);
  fclose(file);

  printf("\nReserva criada com sucesso! ID: %d\n", nova.id);
}

// função para deletar a reserva em caso do cliente desistir de reservar o
// quarto
void delete_reserva() {
  int id;
  printf("Digite o id da reserva: ");
  scanf("%d", &id);

  FILE *original = fopen(RESERVA_DB, "r");
  FILE *temp = fopen(TEMP, "w");

  char line[MAX_LINE_LENGTH];

  if (original == NULL || temp == NULL) {
    perror("Falha ao abrir o arquivo. Tente novamente!");
    return;
  }

  Reserva reserva;

  while (fgets(line, sizeof(line), original) != NULL) {
    if (sscanf(line, "%d;%d;%d;%[^;];%[^;];%d;%f", &reserva.id,
               &reserva.quarto_id, &reserva.cliente_id, reserva.check_in,
               reserva.check_out, &reserva.quantidade_pessoas,
               &reserva.preco_total) == 7) {
      if (id == reserva.id) {
        remover_reserva(reserva.quarto_id);
      } else {
        fprintf(temp, "%d;%d;%d;%s;%s;%d;%.2f\n", reserva.id, reserva.quarto_id,
                reserva.cliente_id, reserva.check_in, reserva.check_out,
                reserva.quantidade_pessoas, reserva.preco_total);
      }
    } else {
      fprintf(temp, "%s", line);
    }
  }

  fclose(original);
  fclose(temp);

  remove(RESERVA_DB);
  rename(TEMP, RESERVA_DB);

  printf("Reserva removida com sucesso");
  return;
}

// funcao para inserir informaçoes novas em uma reserva ja existente
void update_reserva() {
  int opcao;
  int id_procurado;
  printf("Digite o id da reserva que deseja modificar: ");
  scanf("%d", &id_procurado);

  printf("Selecione o que voce quer alterar na reserva:\n");
  printf("1. Id da reserva\n");
  printf("2. Id do quarto\n");
  printf("3. Id do cliente que reservou\n");
  printf("4. Dia do check-in\n");
  printf("5. Dia do Check-out\n");
  printf("6. Quantidade de pessoas\n");
  printf("7. Valor total\n");
  printf("0. Voltar ao Menu Principal\n");

  printf("Opção: ");
  scanf("%d", &opcao);

  FILE *reserva = fopen(RESERVA_DB, "r");
  FILE *arq_temp = fopen(TEMP, "w");

  char line[MAX_LINE_LENGTH];

  if (reserva == NULL || arq_temp == NULL) {
    perror("Falha ao abrir o arquivo. Tente novamente!");
    return;
  }

  while (fgets(line, sizeof(line), reserva)) {
    char copy_line[MAX_LINE_LENGTH];
    strcpy(copy_line, line);
    char *token = strtok(copy_line, ";");

    if (token != NULL && atoi(token) == id_procurado) {
      int id_lido = atoi(token);
      char *quarto_id = strtok(NULL, ";");
      char *cliente_id = strtok(NULL, ";");
      char *check_in = strtok(NULL, ";");
      char *check_out = strtok(NULL, ";");
      char *qtd_pessoas = strtok(NULL, ";");
      char *preco_total = strtok(NULL, ";\n");

      if (opcao == 1) {
        int novo_id;
        printf("Digite o id da reserva corrigido:\n");
        scanf("%d", &novo_id);

        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", novo_id, quarto_id,
                cliente_id, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 2) {
        char novo_idq[50];
        printf("Digite o id do quarto corrigido:\n");
        scanf("%99s", novo_idq);

        remover_reserva(atoi(quarto_id));
        adicionar_reserva(atoi(novo_idq));

        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, novo_idq,
                cliente_id, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 3) {
        char novo_idc[50];
        printf("Digite o id do cliente corrigido:\n");
        scanf("%99s", novo_idc);

        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                novo_idc, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 4) {
        char novo_check_in[50];
        printf("Digite o a data corrigida do chek-in:\n");
        scanf("%99s", novo_check_in);

        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, novo_check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 5) {
        char novo_check_out[10];
        printf("Digite a data corrigida do chek-out:\n");
        scanf("%99s", novo_check_out);

        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, check_in, novo_check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 6) {
        char nova_qntd_pessoas[20];
        printf("Digite o quantidade de pessoas:\n");
        scanf("%99s", nova_qntd_pessoas);

        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, check_in, check_out, nova_qntd_pessoas,
                preco_total);
      }

      else if (opcao == 7) {
        char novo_preco_total[50];
        printf("Digite o preco total corrigido:\n");
        scanf("%99s", novo_preco_total);

        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, check_in, check_out, qtd_pessoas, novo_preco_total);
      }

      else {
        fclose(reserva);
        fclose(arq_temp);
        remove(TEMP);
        return;
      }

    } else {
      fputs(line, arq_temp);
    }
  }

  fclose(reserva);
  fclose(arq_temp);

  remove(RESERVA_DB);
  rename(TEMP, RESERVA_DB);

  printf("Alteracoes realizada com sucesso");
  return;
}