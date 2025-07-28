#ifndef QUARTO_H
#define QUARTO_H

#define MAX_NOME 100
#define MAX_DESC 200
#define MAX_LINE_LENGTH 500

typedef struct {
  int id;
  int tipo_id;
  char nome[MAX_NOME];
  char descricao[MAX_DESC];
  float diaria_por_pessoa;
  int capacidade;
  int reservas;
} Quarto;

void get_rooms();
Quarto get_room_details(int id_procurado, int interno);
int get_room(int id_procurado);
void create_room();
void update_room(int id_procurado);
void delete_room(int id_procurado);
void remover_reserva(int id_procurado);
void adicionar_reserva(int id_procurado);
void relatorio_quartos_mais_reservados();

#endif