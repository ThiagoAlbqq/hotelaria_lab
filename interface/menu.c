#include "menu.h"
#include <stdio.h>

void show_menu(const char *type) {
  printf("----------------- MENU -----------------\n");
  printf("1. Inserir %s\n", type);
  printf("2. Buscar %s\n", type);
  printf("3. Buscar todos os %ss\n", type);
  printf("4. Atualizar %s\n", type);
  printf("5. Excluir %s\n", type);
  printf("0. Voltar\n");
  printf("----------------------------------------\n");
  printf("Escolha uma opcao: ");
}