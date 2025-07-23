#include "./interface/menu.h"
#include "./repository/quarto.h"
#include <stdio.h>

int main() {
  int choice = 0;
  do {
    // Chamada da função
    show_menu("Quarto");
    scanf("%d", &choice);
    while (getchar() != '\n')
      ;
    switch (choice) {
    case 1:
      // Função de Inserir
      create_room();
      break;
    case 2:
      // Função de Buscar
      get_room(101);
      break;
    case 3:
      // Função de Buscar todos
      get_rooms();
      break;
    case 4:
      // Função de Atualizar
      break;
    case 5:
      // Função de Excluir
      break;
    case 0:
      printf("Saindo do programa...\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
      break;
    }
  } while (choice != 0);
  return 0;
}