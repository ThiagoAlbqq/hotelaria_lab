#include "./interface/menu.h"
#include "./repository/quarto.h"
#include <stdio.h>

int main() {
  int choice = 0;
  int cod;
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
      get_rooms();
      printf("Digite o codigo do Quarto a ser listado: ");
      scanf("%d", &cod);
      get_room(cod);
      break;
    case 3:
      // Função de Buscar todos
      get_rooms();
      break;
    case 4:
      // Função de Atualizar
      get_rooms();
      printf("Digite o codigo do Quarto a ser atualizado: ");
      scanf("%d", &cod);
      update_room(cod);
      break;
    case 5:
      // Função de Excluir
      get_rooms();
      printf("Digite o codigo do Quarto a ser deletado: ");
      scanf("%d", &cod);
      delete_room(cod);
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