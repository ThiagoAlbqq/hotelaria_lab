#include "menu.h"
#include "../repository/cliente.h"
#include "../repository/quarto.h"
#include "../repository/reserva.h"
#include "../utils/clear_terminal.h"
#include <stdio.h>

void show_crud_menu(const char *entity_name) {
  printf("--- GERENCIAR %s ---\n", entity_name);
  printf("1. Inserir %s\n", entity_name);
  printf("2. Buscar %s por Código\n", entity_name);
  printf("3. Listar Todos os %ss\n", entity_name);
  printf("4. Atualizar %s\n", entity_name);
  printf("5. Excluir %s\n", entity_name);
  printf("0. Voltar ao Menu Principal\n");
  printf("Escolha uma opção: ");
}

void menu_quartos() {
  int choice = 0;
  int cod;
  clearTerminal();
  do {
    show_crud_menu("Quarto");
    scanf("%d", &choice);
    while (getchar() != '\n')
      ;

    switch (choice) {
    case 1:
      create_room();
      break;
    case 2:
      printf("Digite o código do Quarto a ser listado: ");
      scanf("%d", &cod);
      while (getchar() != '\n')
        ;
      get_room_details(cod, 0);
      break;
    case 3:
      get_rooms();
      break;
    case 4:
      get_rooms();
      printf("Digite o código do Quarto a ser atualizado: ");
      scanf("%d", &cod);
      while (getchar() != '\n')
        ;
      update_room(cod);
      break;
    case 5:
      get_rooms();
      printf("Digite o código do Quarto a ser deletado: ");
      scanf("%d", &cod);
      while (getchar() != '\n')
        ;
      delete_room(cod);
      break;
    case 0:
      printf("Voltando ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
      break;
    }
    if (choice != 0) {
      printf("\nPressione Enter para continuar...");
      while (getchar() != '\n')
        ;
      clearTerminal();
    }
  } while (choice != 0);
}

void menu_reservas() {
  int choice = 0;
  int cod;
  clearTerminal();
  do {
    show_crud_menu("Reserva");
    scanf("%d", &choice);
    while (getchar() != '\n')
      ;

    switch (choice) {
    case 1:
      // Chamar a função de criar reserva
      create_reserva();
      break;
    case 2:
      printf("Digite o código da Reserva a ser listada: ");
      scanf("%d", &cod);
      limpar_buffer();
      // Chamar a função de buscar reserva
       get_reserva(cod);
      break;
    case 3:
      // Chamar a função de listar todas as reservas
      get_reservas();
      break;
    case 4:
      // Chamar a função de atualizar reserva
      update_reserva();
      break;
    case 5:
      // Chamar a função de deletar reserva
      delete_reserva();
      break;
    case 0:
      printf("Voltando ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
      break;
    }
    if (choice != 0) {
      printf("\nPressione Enter para continuar...");
      while (getchar() != '\n')
        ;
      clearTerminal();
    }
  } while (choice != 0);
}

void menu_clientes() {
  int choice = 0;
  int cod;
  int escolha_filtro;
  clearTerminal();
  do {
    show_crud_menu("Cliente");
    scanf("%d", &choice);
    while (getchar() != '\n')
      ;

    switch (choice) {
    case 1:
      adicionar_cliente();
      break;
    case 2:
      clearTerminal();
      do {
        printf("--- LISTAR Clientes ---\n");
        printf("1. Filtrar clientes por CPF\n");
        printf("2. Filtrar clientes por EMAIL\n");
        printf("0. Voltar ao GERENCIAR Cliente\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha_filtro);
        while (getchar() != '\n')
          ;
        switch (escolha_filtro) {
        case 1:
          listar_clientes_pelo_cpf();
          break;
        case 2:
          listar_clientes_pelo_email();
          break;
        case 0:
          printf("Voltando ao GERENCIAR Cliente...\n");
          break;
        default:
          printf("Opção inválida! Tente novamente.\n");
          break;
        }
        if (escolha_filtro != 0) {
          printf("\nPressione Enter para continuar...");
          while (getchar() != '\n')
            ;
          clearTerminal();
        }
      } while (escolha_filtro != 0);
      break;
    case 3:
      listar_clientes();
      break;
    case 4:
      atualizar_cliente();
      break;
    case 5:
      remover_cliente();
      break;
    case 0:
      printf("\nVoltando ao Menu Principal...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
      break;
    }
    if (choice != 0) {
      printf("\nPressione Enter para continuar...");
      while (getchar() != '\n')
        ;
      clearTerminal();
    }
  } while (choice != 0);
}