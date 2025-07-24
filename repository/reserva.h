#ifndef RESERVA_H
#define RESERVA_H

#include "../entity/reserva.c"
// void get_reservas();
// void get_reserva(int id_procurado);
void create_reserva(int id, int quarto_id, int cliente_id, char *check_in, char *check_out, int quantidade_pessoas, float preco_total);
// void update_reserva(int id_procurado);
// void delete_reserva(int id_procurado);

#endif