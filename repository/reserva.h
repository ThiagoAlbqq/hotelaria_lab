#ifndef RESERVA_H
#define RESERVA_H

typedef struct {
  int id;
  int quarto_id;      // Refere ao Quarto
  int cliente_id;     // Refere ao Cliente
  char check_in[11];  // "YYYY-MM-DD"
  char check_out[11]; // "YYYY-MM-DD"
  int quantidade_pessoas;
  float preco_total;
} Reserva;

// void get_reservas();
// void get_reserva(int id_procurado);
void create_reserva(int id, int quarto_id, int cliente_id, char *check_in,
                    char *check_out, int quantidade_pessoas, float preco_total);
// void update_reserva(int id_procurado);
// void delete_reserva(int id_procurado);

#endif