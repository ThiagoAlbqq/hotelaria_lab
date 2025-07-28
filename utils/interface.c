#include <stdio.h>
#include <stdlib.h>

void esperar_enter() {
  printf("\nPressione Enter para continuar...");
  while (getchar() != '\n')
    ;
}

void limpar_buffer() {
  while (getchar() != '\n')
    ;
}