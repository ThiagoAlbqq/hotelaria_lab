#ifndef DATA_H
#define DATA_H

typedef struct data {
  int dia;
  int mes;
  int ano;
} Data;

Data ler_data_completa(const char *mensagem);
Data get_data_atual();
int comparar_datas(Data d1, Data d2);
int data_valida_completa(int dia, int mes, int ano);
int eh_bissexto(int ano);
void formatar_data(Data d, char *destino);
Data ler_data(const char *mensagem);
int data_valida(int dia, int mes, int ano);


#endif