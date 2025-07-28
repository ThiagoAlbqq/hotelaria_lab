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
<<<<<<< HEAD

#define RESERVA_DB "./database/reserva.txt"
#define TEMP "./database/temp.txt"

void esperar_enter() {
  printf("\nPressione Enter para continuar...");
  while (getchar() != '\n')
    ;
}

void limpar_buffer() {
  while (getchar() != '\n')
    ;
}
=======


#define RESERVA_DB "./database/reserva.txt"
#define TEMP "./database/temp.txt"
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e

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

int get_cliente(int id) { return (id == 1 || id == 2) ? 0 : -1; }

// Simulação de listar_clientes
void listar_clientes_mock() {
  printf("\n--- Clientes Disponíveis (Simulação) ---\n");
  printf("ID: 1 - Cliente Teste 1\n");
  printf("ID: 2 - Cliente Teste 2\n");
  printf("-----------------------------------------\n");
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

  do {
    printf("Digite o preço total da reserva: ");
    scanf("%f", &nova.preco_total);
    limpar_buffer();
    if (nova.preco_total < 0) {
      printf("O preço total não pode ser negativo. Tente novamente.\n");
    } else {
      break;
    }
  } while (1);

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
  esperar_enter();
}

<<<<<<< HEAD
//função para deletar a reserva em caso do cliente desistir de reservar o quarto
void delete_reserva(){
  int id;
  printf("Digite o id da reserva:\n");
  scanf("%d", &id);
  
  FILE * reserva = fopen(RESERVA_DB, "r");
  FILE * arq_temp = fopen(TEMP, "w");

  char line[MAX_LINE_LENGTH];

  if(reserva == NULL || arq_temp == NULL){
=======
// função para deletar a reserva em caso do cliente desistir de reservar o
// quarto
void delete_reserva() {
  int id;
  printf("Digite o id da reserva:\n");
  scanf("%d", &id);

  FILE *reserva = fopen(RESERVA_DB, "r");
  FILE *arq_temp = fopen(TEMP, "w");

  char line[MAX_LINE_LENGTH];

  if (reserva == NULL || arq_temp == NULL) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
    perror("Falha ao abrir o arquivo. Tente novamente!");
    return;
  }

<<<<<<< HEAD
  while(fgets(line, sizeof(line), reserva)){
=======
  while (fgets(line, sizeof(line), reserva)) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
    char copy_line[MAX_LINE_LENGTH];
    strcpy(copy_line, line);
    char *token = strtok(copy_line, ";");

<<<<<<< HEAD
    if(token != NULL && atoi(token) != id){
=======
    if (token != NULL && atoi(token) != id) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
      fputs(line, arq_temp);
    }
  }

  fclose(reserva);
  fclose(arq_temp);

  remove(RESERVA_DB);
  rename(TEMP, RESERVA_DB);

  printf("Reserva removida com sucesso");
  return;
}

<<<<<<< HEAD
//funcao para inserir informaçoes novas em uma reserva ja existente

void update_reserva(){
=======
// funcao para inserir informaçoes novas em uma reserva ja existente

void update_reserva() {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
  int opcao;
  int id_procurado;
  printf("Digite o id da reserva que deseja modificar:\n");
  scanf("%d", &id_procurado);
<<<<<<< HEAD
  
=======

>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
  printf("Selecione o que voce quer alterar na reserva:\n");
  printf("1. Id da reserva\n");
  printf("2. Id do quarto\n");
  printf("3. Id do cliente que reservou\n");
  printf("4. Dia do check-in\n");
  printf("5. Dia do Check-out\n");
  printf("6. Quantidade de pessoas\n");
  printf("7. Valor total\n");
  printf("0. Voltar ao Menu Principal\n");

  scanf("%d", &opcao);

<<<<<<< HEAD
  FILE * reserva = fopen(RESERVA_DB, "r");
  FILE * arq_temp = fopen(TEMP, "w");

  char line[MAX_LINE_LENGTH];

  if(reserva == NULL || arq_temp == NULL){
=======
  FILE *reserva = fopen(RESERVA_DB, "r");
  FILE *arq_temp = fopen(TEMP, "w");

  char line[MAX_LINE_LENGTH];

  if (reserva == NULL || arq_temp == NULL) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
    perror("Falha ao abrir o arquivo. Tente novamente!");
    return;
  }

<<<<<<< HEAD
  while(fgets(line, sizeof(line), reserva)){
=======
  while (fgets(line, sizeof(line), reserva)) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
    char copy_line[MAX_LINE_LENGTH];
    strcpy(copy_line, line);
    char *token = strtok(copy_line, ";");

<<<<<<< HEAD
    if(token != NULL && atoi(token) == id_procurado){
=======
    if (token != NULL && atoi(token) == id_procurado) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
      int id_lido = atoi(token);
      char *quarto_id = strtok(NULL, ";");
      char *cliente_id = strtok(NULL, ";");
      char *check_in = strtok(NULL, ";");
      char *check_out = strtok(NULL, ";");
      char *qtd_pessoas = strtok(NULL, ";");
      char *preco_total = strtok(NULL, ";\n");

<<<<<<< HEAD
      if(opcao == 1){
=======
      if (opcao == 1) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        int novo_id;
        printf("Digite o id da reserva corrigido:\n");
        scanf("%d", &novo_id);

<<<<<<< HEAD
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n",novo_id, quarto_id, cliente_id, check_in, check_out, qtd_pessoas, preco_total);
      } 

      else if(opcao == 2){
=======
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", novo_id, quarto_id,
                cliente_id, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 2) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        char novo_idq[50];
        printf("Digite o id do quarto corrigido:\n");
        scanf("%99s", novo_idq);

<<<<<<< HEAD
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n",id_lido, novo_idq, cliente_id, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if(opcao == 3){
=======
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, novo_idq,
                cliente_id, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 3) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        char novo_idc[50];
        printf("Digite o id do cliente corrigido:\n");
        scanf("%99s", novo_idc);

<<<<<<< HEAD
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n",id_lido, quarto_id, novo_idc, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if(opcao == 4){
=======
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                novo_idc, check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 4) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        char novo_check_in[50];
        printf("Digite o a data corrigida do chek-in:\n");
        scanf("%99s", novo_check_in);

<<<<<<< HEAD
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n",id_lido, quarto_id, cliente_id, novo_check_in, check_out, qtd_pessoas, preco_total);
      }

      else if(opcao == 5){
=======
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, novo_check_in, check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 5) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        char novo_check_out[10];
        printf("Digite a data corrigida do chek-out:\n");
        scanf("%99s", novo_check_out);

<<<<<<< HEAD
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n",id_lido, quarto_id, cliente_id, check_in, novo_check_out, qtd_pessoas, preco_total);
      }

      else if(opcao == 6){
=======
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, check_in, novo_check_out, qtd_pessoas, preco_total);
      }

      else if (opcao == 6) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        char nova_qntd_pessoas[20];
        printf("Digite o quantidade de pessoas:\n");
        scanf("%99s", nova_qntd_pessoas);

<<<<<<< HEAD
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n",id_lido, quarto_id, cliente_id, check_in, check_out, nova_qntd_pessoas, preco_total);
      }

      else if(opcao == 7){
=======
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, check_in, check_out, nova_qntd_pessoas,
                preco_total);
      }

      else if (opcao == 7) {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        char novo_preco_total[50];
        printf("Digite o preco total corrigido:\n");
        scanf("%99s", novo_preco_total);

<<<<<<< HEAD
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n",id_lido, quarto_id, cliente_id, check_in, check_out, qtd_pessoas, novo_preco_total);
      }

      else{
=======
        fprintf(arq_temp, "%d;%s;%s;%s;%s;%s;%s\n", id_lido, quarto_id,
                cliente_id, check_in, check_out, qtd_pessoas, novo_preco_total);
      }

      else {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
        fclose(reserva);
        fclose(arq_temp);
        remove(TEMP);
        return;
      }

<<<<<<< HEAD
    } 
    else{
=======
    } else {
>>>>>>> 673b846385b7e3a28140ccb4c5079e7f4940d44e
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