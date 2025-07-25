#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIENTE_DB "./database/cliente.txt"
#define TEMP_DB "./database/temp_cliente.txt"

#define MAX_NOME 100
#define MAX_EMAIL 100
#define MAX_TELEFONE 20

typedef struct {
    int id;
    char nome[MAX_NOME];
    char cpf[15];
    char telefone[MAX_TELEFONE];
    char email[MAX_EMAIL];
} Cliente;

// Função para verificar se CPF já existe
int buscar_cliente_por_cpf(const char *cpf, Cliente *cliente_encontrado) {
    FILE *file = fopen(CLIENTE_DB, "r");
    if (!file) return 0;

    Cliente c;
    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%[^\n]\n", &c.id, c.nome, c.cpf, c.telefone, c.email) == 5) {
        if (strcmp(c.cpf, cpf) == 0) {
            if (cliente_encontrado) *cliente_encontrado = c;
            fclose(file);
            return 1; // Encontrado
        }
    }

    fclose(file);
    return 0; // Não encontrado
}

// Gera um novo ID automático
int gerar_novo_id() {
    FILE *file = fopen(CLIENTE_DB, "r");
    if (!file) return 1;
    int max_id = 0, id;
    char linha[300];
    while (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%d;", &id);
        if (id > max_id) max_id = id;
    }
    fclose(file);
    return max_id + 1;
}

// Adiciona novo cliente
void adicionar_cliente() {
    Cliente c;
    char cpf[15];
    Cliente existente;

    printf("Digite o CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    if (buscar_cliente_por_cpf(cpf, &existente)) {
        printf("Cliente já cadastrado:\n");
        printf("ID: %d | Nome: %s | Telefone: %s | Email: %s\n", existente.id, existente.nome, existente.telefone, existente.email);
        printf("Deseja reutilizar esse cadastro? (s/n): ");
        char opcao;
        scanf(" %c", &opcao);
        while (getchar() != '\n');

        if (opcao == 's' || opcao == 'S') {
            printf("Cadastro reutilizado.\n");
            return;
        } else {
            printf("Você escolheu cadastrar um novo cliente com o mesmo CPF.\n");
        }
    }

    c.id = gerar_novo_id();
    strcpy(c.cpf, cpf);

    printf("Digite o nome: ");
    fgets(c.nome, MAX_NOME, stdin);
    c.nome[strcspn(c.nome, "\n")] = 0;

    printf("Digite o telefone: ");
    fgets(c.telefone, MAX_TELEFONE, stdin);
    c.telefone[strcspn(c.telefone, "\n")] = 0;

    printf("Digite o email: ");
    fgets(c.email, MAX_EMAIL, stdin);
    c.email[strcspn(c.email, "\n")] = 0;

    FILE *file = fopen(CLIENTE_DB, "a");
    if (!file) {
        perror("Erro ao abrir arquivo de clientes");
        return;
    }

    fprintf(file, "%d;%s;%s;%s;%s\n", c.id, c.nome, c.cpf, c.telefone, c.email);
    fclose(file);

    printf("Cliente cadastrado com sucesso! ID: %d\n", c.id);
}

// Remove cliente por CPF
void remover_cliente() {
    char cpf[15];
    printf("Digite o CPF do cliente a ser removido: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    FILE *original = fopen(CLIENTE_DB, "r");
    FILE *temp = fopen(TEMP_DB, "w");

    if (!original || !temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    Cliente c;
    int removido = 0;

    while (fscanf(original, "%d;%[^;];%[^;];%[^;];%[^\n]\n", &c.id, c.nome, c.cpf, c.telefone, c.email) == 5) {
        if (strcmp(c.cpf, cpf) != 0) {
            fprintf(temp, "%d;%s;%s;%s;%s\n", c.id, c.nome, c.cpf, c.telefone, c.email);
        } else {
            removido = 1;
        }
    }

    fclose(original);
    fclose(temp);

    if (removido) {
        remove(CLIENTE_DB);
        rename(TEMP_DB, CLIENTE_DB);
        printf("Cliente removido com sucesso.\n");
    } else {
        remove(TEMP_DB);
        printf("Cliente com CPF %s não encontrado.\n", cpf);
    }
}

// Atualiza cliente
void atualizar_cliente() {
    char cpf[15];
    printf("Digite o CPF do cliente que deseja atualizar: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    FILE *original = fopen(CLIENTE_DB, "r");
    FILE *temp = fopen(TEMP_DB, "w");

    if (!original || !temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    Cliente c;
    int atualizado = 0;

    while (fscanf(original, "%d;%[^;];%[^;];%[^;];%[^\n]\n", &c.id, c.nome, c.cpf, c.telefone, c.email) == 5) {
        if (strcmp(c.cpf, cpf) == 0) {
            printf("Digite o novo nome: ");
            fgets(c.nome, MAX_NOME, stdin);
            c.nome[strcspn(c.nome, "\n")] = 0;

            printf("Digite o novo telefone: ");
            fgets(c.telefone, MAX_TELEFONE, stdin);
            c.telefone[strcspn(c.telefone, "\n")] = 0;

            printf("Digite o novo email: ");
            fgets(c.email, MAX_EMAIL, stdin);
            c.email[strcspn(c.email, "\n")] = 0;

            atualizado = 1;
        }

        fprintf(temp, "%d;%s;%s;%s;%s\n", c.id, c.nome, c.cpf, c.telefone, c.email);
    }

    fclose(original);
    fclose(temp);

    if (atualizado) {
        remove(CLIENTE_DB);
        rename(TEMP_DB, CLIENTE_DB);
        printf("Cliente atualizado com sucesso.\n");
    } else {
        remove(TEMP_DB);
        printf("Cliente com CPF %s não encontrado.\n", cpf);
    }
}

// Listar todos os clientes
void listar_clientes() {
    FILE *file = fopen(CLIENTE_DB, "r");
    if (!file) {
        printf("Nenhum cliente cadastrado ainda.\n");
        return;
    }

    Cliente c;
    printf("\n--- LISTA DE CLIENTES ---\n");
    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%[^\n]\n", &c.id, c.nome, c.cpf, c.telefone, c.email) == 5) {
        printf("ID: %d | Nome: %s | CPF: %s | Tel: %s | Email: %s\n", c.id, c.nome, c.cpf, c.telefone, c.email);
    }

    fclose(file);
}
