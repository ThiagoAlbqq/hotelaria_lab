#include "reserva.h"
#include <stdio.h>
#include <string.h>

#define RESERVA_DB "./database/reserva.txt"

void create_reserva(int id, int quarto_id, int cliente_id, char *check_in, char *check_out, int quantidade_pessoas, float preco_total){
  Reserva nova;
  nova.id = id;
  nova.quarto_id = quarto_id;
  nova.cliente_id = cliente_id;
  strcpy(nova.check_in, check_in);
  strcpy(nova.check_out, check_out);
  nova.quantidade_pessoas = quantidade_pessoas;
  nova.preco_total = preco_total;

  FILE * reserva = fopen(RESERVA_DB, "a");
  fprintf(reserva, "%d;%d;%d;%s;%s;%d;%.2f\n", id, quarto_id, cliente_id, check_in, check_out, quantidade_pessoas, preco_total);
  fclose(reserva);
}