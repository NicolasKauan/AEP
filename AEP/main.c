#include <stdio.h>
#include <string.h>

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Função para criptografar a senha
void encryptPassword(char *password) {
    int length = strlen(password);
    for (int i = 0; i < length / 2; i++) {
        char temp = password[i];
        password[i] = password[length - i - 1];
        password[length - i - 1] = temp;
    }

    for (int i = 0; i < length; i++) {
        password[i] = password[i] + 10;
    }
}

// Função para criar um novo usuário e salvar a senha criptografada no arquivo
void create(const char *filename) {
    char nome[50];
    char senha[50];

    FILE *pont = fopen(filename, "w");
    if (pont == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite a senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    // Criptografa a senha antes de salvar no arquivo
    encryptPassword(senha);

    fprintf(pont, "%s\n", nome);
    fprintf(pont, "%s\n", senha);
    fclose(pont);
    printf("Usuário criado com sucesso.\n");
}

// Função para ler o nome do usuário do arquivo
void readName(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char nome[50];
    if (fgets(nome, sizeof(nome), file) != NULL) {
        printf("Nome: %s\n", nome);
    } else {
        printf("Erro ao ler o nome.\n");
    }

    fclose(file);
}

// Função para ler a senha do arquivo
int readPassword(const char *filename, char *senha) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char linha[50];
    fgets(linha, sizeof(linha), file);  // Ignora a primeira linha (nome)

    if (fgets(senha, 50, file) != NULL) {
        senha[strcspn(senha, "\n")] = '\0';
        fclose(file);
        return 1;
    } else {
        printf("Erro ao ler a senha.\n");
        fclose(file);
        return 0;
    }
}

// Função para atualizar a senha
void updatePassword(const char *filename) {
    char nome[50];
    char senha[50];

    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    if (fgets(nome, sizeof(nome), file) == NULL) {
        printf("Erro ao ler o nome.\n");
        fclose(file);
        return;
    }

    printf("Digite a nova senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';
    encryptPassword(senha);

    rewind(file);
    fprintf(file, "%s\n", nome);
    fprintf(file, "%s\n", senha);

    fclose(file);
    printf("Senha atualizada e criptografada com sucesso.\n");
}

// Função para verificar login (nome e senha)
int verifyLogin(const char *filename) {
    char storedName[50];
    char storedPassword[50];
    char enteredName[50];
    char enteredPassword[50];

    // Lê o nome e a senha criptografada do arquivo
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    if (fgets(storedName, sizeof(storedName), file) == NULL || fgets(storedPassword, sizeof(storedPassword), file) == NULL) {
        printf("Erro ao ler dados do arquivo.\n");
        fclose(file);
        return 0;
    }

    storedName[strcspn(storedName, "\n")] = '\0';
    storedPassword[strcspn(storedPassword, "\n")] = '\0';
    fclose(file);

    // Solicita o nome e a senha do usuário
    printf("Digite o nome para login: ");
    fgets(enteredName, sizeof(enteredName), stdin);
    enteredName[strcspn(enteredName, "\n")] = '\0';

    printf("Digite a senha para login: ");
    fgets(enteredPassword, sizeof(enteredPassword), stdin);
    enteredPassword[strcspn(enteredPassword, "\n")] = '\0';
    encryptPassword(enteredPassword);

    // Verifica se nome e senha correspondem aos dados do arquivo
    if (strcmp(storedName, enteredName) == 0 && strcmp(storedPassword, enteredPassword) == 0) {
        printf("Login bem-sucedido.\n");
        return 1;
    } else {
        printf("Nome ou senha incorretos.\n");
        return 0;
    }
}

int main() {
    char password[50];
    const char *filename = "data.txt";
    int menu = 0;

    do {
        printf("\nMenu de escolhas: \n");
        printf("1 - Listar usuario \n");
        printf("2 - Criar usuario \n");
        printf("5 - Atualizar senha \n");
        printf("6 - Verificar login \n");
        printf("-1 - Fechar aplicacao \n");
        fgets(password, sizeof(password), stdin);
        sscanf(password, "%d", &menu);

        switch (menu) {
        case -1:
            printf("Fechando aplicacao...\n");
            break;
        case 1:
            readName(filename);
            break;
        case 2:
            printf("Criando novo usuario...\n");
            create(filename);
            break;
        case 5:
            updatePassword(filename);
            break;
        case 6:
            verifyLogin(filename);
            break;
        default:
            printf("Opcao invalida\n");
            break;
        }
    } while (menu != -1);

    return 0;
}
