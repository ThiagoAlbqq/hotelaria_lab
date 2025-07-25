#include "./interface/menu.h"
#include "./repository/quarto.h"
#include "./repository/reserva.h"
#include "./utils/clear_terminal.h"
#include <stdio.h>

int main() {
  int main_choice = 0;

  do {
    clearTerminal();
    printf("--- MENU PRINCIPAL ---\n");
    printf("1. Gerenciar Quartos\n");
    printf("2. Gerenciar Reservas\n");
    printf("3. Gerenciar Clientes\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &main_choice);
    while (getchar() != '\n')
      ;

    switch (main_choice) {
    case 1:
      // Chama a função para o menu de Quartos
      menu_quartos();
      break;
    case 2:
      // Chama a função para o menu de Reservas
      menu_reservas();
      break;
    case 3:
      // Chama a função para o menu de Clientes
      menu_clientes();
      break;
    case 0:
      printf("Saindo do programa...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
      break;
    }
  } while (main_choice != 0);

  return 0;
}