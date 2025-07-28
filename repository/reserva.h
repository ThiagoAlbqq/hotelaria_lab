#ifndef RESERVA_H
#define RESERVA_H

#include "../utils/data.h"
typedef struct {
  int id;
  int quarto_id;      // Refere ao Quarto
  int cliente_id;     // Refere ao Cliente
  char check_in[11];  // "YYYY-MM-DD"
  char check_out[11]; // "YYYY-MM-DD"
  int quantidade_pessoas;
  float preco_total;
} Reserva;

void esperar_enter();
void limpar_buffer();
int data_valida(int dia, int mes, int ano);
Data ler_data(const char *mensagem);
void formatar_data(Data d, char *destino);
void listar_clientes_mock();

int gerar_novo_id_reserva();

// void get_reservas();
// void get_reserva(int id_procurado);
void create_reserva();
void delete_reserva();
void update_reserva();

#endif