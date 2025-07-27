#include "reserva.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TEMP "temp.txt"
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

//função para deletar a reserva em caso do cliente desistir de reservar o quarto
void delete_reserva(int cliente){
  FILE * reserva = fopen(RESERVA_DB, "r");
  FILE * arq_temp = fopen(TEMP, "w");

  char line[MAX_LINE_LENGTH];

  if(reserva == NULL || arq_temp == NULL){
    perror("Falha ao abrir o arquivo. Tente novamente!");
    return;
  }

  while(fgets(line, sizeof(line), reserva)){
    char copy_line[MAX_LINE_LENGTH];
    strcpy(copy_line, line);
    char *token = strtok(copy_line, ";");
    token = strtok(NULL, ";");
    token = strtok(NULL, ";");

    if(token != NULL && atoi(token) != cliente){
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

//funcao para deletar a reserva caso o chekout tenha passado do prazo
void deadline(){
  struct tm *data_hora_atual;
  time_t sec;
  time(&sec);
  data_hora_atual = localtime(&sec);
}