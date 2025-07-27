#include "data.h"
#include <stdio.h>
#include <time.h>

int data_valida(int dia, int mes, int ano) {
  return (dia >= 1 && dia <= 31) && (mes >= 1 && mes <= 12) &&
         (ano >= 2020 && ano <= 2100);
}

Data ler_data(const char *mensagem) {
  Data d;
  int valido = 0;

  do {
    printf("%s (dd mm aaaa): ", mensagem);
    scanf("%d %d %d", &d.dia, &d.mes, &d.ano);
    while (getchar() != '\n')
      ;
    if (!data_valida(d.dia, d.mes, d.ano)) {
      printf("Data inválida. Tente novamente.\n");
    } else {
      valido = 1;
    }
  } while (!valido);
  return d;
}

void formatar_data(Data d, char *destino) {
  sprintf(destino, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

int eh_bissexto(int ano) {
  return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int data_valida_completa(int dia, int mes, int ano) {
  if (ano < 2020 || ano > 2100)
    return 0;
  if (mes < 1 || mes > 12)
    return 0;

  int dias_no_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (eh_bissexto(ano)) {
    dias_no_mes[2] = 29;
  }

  if (dia < 1 || dia > dias_no_mes[mes])
    return 0;

  return 1;
}

int comparar_datas(Data d1, Data d2) {
  if (d1.ano != d2.ano)
    return d1.ano - d2.ano;
  if (d1.mes != d2.mes)
    return d1.mes - d2.mes;
  return d1.dia - d2.dia;
}

Data get_data_atual() {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  Data hoje;
  hoje.dia = tm.tm_mday;
  hoje.mes = tm.tm_mon + 1;     // tm_mon é 0-11
  hoje.ano = tm.tm_year + 1900; // tm_year é anos desde 1900
  return hoje;
}

Data ler_data_completa(const char *mensagem) {
  Data d;
  int valido = 0;
  do {
    printf("%s (dd mm aaaa): ", mensagem);
    scanf("%d %d %d", &d.dia, &d.mes, &d.ano);
    while (getchar() != '\n')
      ;
    if (!data_valida_completa(d.dia, d.mes, d.ano)) {
      printf("Data inválida. Verifique o formato e a validade da data. Tente "
             "novamente.\n");
    } else {
      valido = 1;
    }
  } while (!valido);
  return d;
}