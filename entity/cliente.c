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
    int entradas;
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
    Cliente existente;
    int cpf_valido = 0;

    while (!cpf_valido) {
        printf("Digite o CPF: ");
        fgets(c.cpf, sizeof(c.cpf), stdin);
        c.cpf[strcspn(c.cpf, "\n")] = 0;

        // Verifica se o CPF já está cadastrado
        if (buscar_cliente_por_cpf(c.cpf, &existente)) {
            printf("\nCliente já cadastrado:\n");
            printf("ID: %d | Nome: %s | Telefone: %s | Email: %s | Entradas: %d\n", existente.id, existente.nome, existente.telefone, 
                existente.email, existente.entradas);

            printf("Deseja reutilizar esse cadastro? (s/n): ");
            char opcao;
            scanf(" %c", &opcao);
            while (getchar() != '\n'); // limpa o buffer

            if (opcao == 's' || opcao == 'S') {
                // Atualiza contador de entradas no arquivo
                FILE *original = fopen(CLIENTE_DB, "r");
                FILE *temp = fopen(TEMP_DB, "w");

                if (!original || !temp) {
                    printf("Erro ao abrir arquivos para atualização.\n");
                    return;
                }

                Cliente cliente_temp;
                char linha[300];

                while (fgets(linha, sizeof(linha), original)) {
                    sscanf(linha, "%d;%[^;];%[^;];%[^;];%[^;];%d", &cliente_temp.id, cliente_temp.nome, cliente_temp.cpf, cliente_temp.telefone, 
                        cliente_temp.email, &cliente_temp.entradas);

                    if (strcmp(cliente_temp.cpf, existente.cpf) == 0) {
                        cliente_temp.entradas++;
                    }

                    fprintf(temp, "%d;%s;%s;%s;%s;%d\n", cliente_temp.id, cliente_temp.nome, cliente_temp.cpf, cliente_temp.telefone, cliente_temp.email,
cliente_temp.entradas);
                }

                fclose(original);
                fclose(temp);
                remove(CLIENTE_DB);
                rename(TEMP_DB, CLIENTE_DB);

                printf("Cadastro reutilizado. Número de entradas atualizado.\n");
                return;
            } else {
                printf("Você deve informar um CPF diferente.\n\n");
            }
        } else {
            cpf_valido = 1;
        }
    }

    // Cadastro novo
    c.id = gerar_novo_id();

    printf("Digite o nome: ");
    fgets(c.nome, MAX_NOME, stdin);
    c.nome[strcspn(c.nome, "\n")] = 0;

    printf("Digite o telefone: ");
    fgets(c.telefone, MAX_TELEFONE, stdin);
    c.telefone[strcspn(c.telefone, "\n")] = 0;

    printf("Digite o email: ");
    fgets(c.email, MAX_EMAIL, stdin);
    c.email[strcspn(c.email, "\n")] = 0;

    c.entradas = 1;

    FILE *file = fopen(CLIENTE_DB, "a");
    if (!file) {
        perror("Erro ao abrir arquivo de clientes");
        return;
    }

    fprintf(file, "%d;%s;%s;%s;%s;%d\n", c.id, c.nome, c.cpf, c.telefone, c.email, c.entradas);
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

//relatorio de clientes mais recorrentes
void relatorio_clientes() {
    FILE *file = fopen(CLIENTE_DB, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    Cliente lista[500];
    int count = 0;

    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%[^;];%d\n",
                  &lista[count].id,
                  lista[count].nome,
                  lista[count].cpf,
                  lista[count].telefone,
                  lista[count].email,
                  &lista[count].entradas) == 6) {
        count++;
    }
    fclose(file);

    // Ordenar por número de entradas
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (lista[i].entradas < lista[j].entradas) {
                Cliente temp = lista[i];
                lista[i] = lista[j];
                lista[j] = temp;
            }
        }
    }

    printf("\n=== RELATÓRIO DE CLIENTES MAIS RECORRENTES ===\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Nome: %s | CPF: %s | Entradas: %d\n", lista[i].id, lista[i].nome, lista[i].cpf, lista[i].entradas);
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
