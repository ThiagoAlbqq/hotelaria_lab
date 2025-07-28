#ifndef CLIENTE_H
#define CLIENTE_H

#define MAX_NOME 100
#define MAX_EMAIL 100
#define MAX_TELEFONE 20
#define MAX_LINE_LENGTH 500

typedef struct {
  int id;
  char nome[MAX_NOME];
  char cpf[15];
  char telefone[MAX_TELEFONE];
  char email[MAX_EMAIL];
  int entradas;
} Cliente;

// Funções CRUD
void adicionar_cliente();
void remover_cliente();
void atualizar_cliente();
int listar_clientes();
void listar_clientes_pelo_cpf();
void listar_clientes_pelo_email();

// Funções auxiliares
int buscar_cliente_por_id(int id);
int buscar_cliente_por_cpf(const char *cpf, Cliente *cliente_encontrado);
int gerar_novo_id();
void relatorio_clientes();

#endif
