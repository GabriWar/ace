//----------------------------------------------------------------------------
// opa fessor, bao?
// deve funcionar tudo normal sem precisar de criar arquivo algum
// mas caso queira criar o senhor mesmo pode criar, com o nome de "ctts" sem
// .txt nem nd valeu
// ---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define arquivo "ctts"
int chave = 10;
// usar preferencialmente numeros entre -26 e 26
// 0 = sem criptografia
typedef struct {
  char Nome[50];
  char numero[50];
} Contatos;
typedef struct {
  int numContatos;
  Contatos *contatos;
} Agenda;
char crypto(char *criptografar, int chave) {
  if (chave > 26) {
    chave = 26;
  } else if (chave < -26) {
    chave = -26;
  }
  for (int i = 0; i < strlen(criptografar); i++) {
    // lidandno com numeros
    if (criptografar[i] >= '0' && criptografar[i] <= '9') {
      if (chave > 10 || chave < -10) {
        chave = 0 + chave % 10; // pior jeito de clampar a chave
      }
      if (criptografar[i] + chave < '0') {
        criptografar[i] = criptografar[i] + 10 + chave;
      } else if (criptografar[i] + chave > '9') {
        criptografar[i] = criptografar[i] - 10 + chave;
      } else {
        criptografar[i] = criptografar[i] + chave;
      }
    }
    // letras e varios casos bem especificos
    else if (criptografar[i] >= 'a' && criptografar[i] <= 'z') {
      if (criptografar[i] + chave < 'a') {
        criptografar[i] = criptografar[i] + 26 + chave;
      } else if (criptografar[i] + chave > 'z') {
        criptografar[i] = criptografar[i] - 26 + chave;
      } else {
        criptografar[i] = criptografar[i] + chave;
      }
    } else if (criptografar[i] >= 'A' && criptografar[i] <= 'Z') {
      if (criptografar[i] + chave < 'A') {
        criptografar[i] = criptografar[i] + 26 + chave;
      } else if (criptografar[i] + chave > 'Z') {
        criptografar[i] = criptografar[i] - 26 + chave;
      } else {
        criptografar[i] = criptografar[i] + chave;
      }
    }
  }
  return *criptografar;
}
void listarCtts(Agenda *agenda) {
  if (agenda->numContatos == 0) {
    printf("Nenhum contato para listar\n");
    return;
  }
  for (int i = 0; i < agenda->numContatos; i++) {

    printf("────────────────────────────\n");
    printf("(%d) Nome: %s\n", i + 1, agenda->contatos[i].Nome);
    printf("(+) Numero: %s\n", agenda->contatos[i].numero);
    printf("────────────────────────────\n");
  }
}
void limparMemoria(Agenda *agenda) {
  free(agenda->contatos);
  agenda->contatos = NULL;
}

void parseFile(char *filename, Agenda *agenda) {
  FILE *file = fopen(filename, "r"); // fechado no final
  if (file == NULL) {
    printf("database de contatos nao encontrada, criando...\n");
    file = fopen(filename, "w"); // fechado?????
    fclose(file);
    printf("database de contatos criada com sucesso, favor reiniciar\n");
    exit(0);
    return;
  }
  char line[256];
  int i = 0;
  int numOfLines = 0;
  while (fgets(line, sizeof(line), file)) {
    numOfLines++;
  }
  if (numOfLines % 2 != 0) {
    printf("Erro: arquivo de contatos corrompido\n");
    printf("Por favor, delete o arquivo %s e reinicie o programa\n", filename);
    exit(1);
  } else {
    agenda->numContatos = numOfLines / 2;
  }
  agenda->contatos = (Contatos *)malloc(numOfLines / 2 * sizeof(Contatos));
  fseek(file, 0, SEEK_SET);

  i = 0;
  int lineNum = 0;
  while (fgets(line, sizeof(line), file)) {
    crypto(line, -chave);
    if (lineNum % 2 == 0) { // toda linha par e uma linha de nome
      strncpy(agenda->contatos[i].Nome, line, sizeof(agenda->contatos[i].Nome));
      agenda->contatos[i].Nome[strcspn(agenda->contatos[i].Nome, "\n")] =
          0; // remove o newline q ta quebrando tudo \n

    } else {
      strncpy(agenda->contatos[i].numero, line,
              sizeof(agenda->contatos[i].numero));
      agenda->contatos[i].numero[strcspn(agenda->contatos[i].numero, "\n")] = 0;
      i++;
    }
    lineNum++;
  }
  fclose(file);
}
void refrescarmemoria(Agenda *agenda) {
  limparMemoria(agenda);
  parseFile(arquivo, agenda);
}
char showmenu(Agenda *agenda) {
  char option;
  printf("╭───────────────────────╮\n");
  printf("│1 - Adicionar contato  │\n");
  printf("│2 - Remover contato    │\n");
  printf("│3 - Pesquisar contatos │\n");
  printf("│4 - listar contatos    │\n");
  printf("│5 - Sair               │\n");
  if (agenda->numContatos < 10) {
    printf("╰───────────────────(%d)─╯\n", agenda->numContatos);

  } else {
    printf("╰──────────────────(%d)─╯\n", agenda->numContatos);
  }
  printf(">");
  scanf(" %c", &option);
  if (option == '\n') {
    return showmenu(agenda);
  }
  return option;
}
void menu(Agenda *agenda) {
  while (1) {
    char option = showmenu(agenda);
    switch (option) {
    case '1':
      // adicionar
      printf("Digite o nome do contato: ");
      char nomeadd[50];
      fflush(stdin);
      scanf(" %[^\n]", nomeadd);
      fflush(stdin);
      printf("Digite o numero do contato: ");
      char numero[50];
      fflush(stdin);
      scanf(" %[^\n]", numero);
      fflush(stdin);
      agenda->contatos = (Contatos *)realloc(
          agenda->contatos, (agenda->numContatos + 1) * sizeof(Contatos));
      strcpy(agenda->contatos[agenda->numContatos].Nome, nomeadd);
      strcpy(agenda->contatos[agenda->numContatos].numero, numero);
      agenda->numContatos++;
      printf("Contato adicionado com sucesso\n");
      FILE *file = fopen(arquivo, "w"); // fechado no final
      for (int i = 0; i < agenda->numContatos; i++) {
        agenda->contatos[i].numero[strcspn(agenda->contatos[i].numero, "\n")] =
            0;

        agenda->contatos[i].Nome[strcspn(agenda->contatos[i].Nome, "\n")] = 0;
        crypto(agenda->contatos[i].Nome, chave);
        crypto(agenda->contatos[i].numero, chave);
        fprintf(file, "%s\n", agenda->contatos[i].Nome);
        fprintf(file, "%s\n", agenda->contatos[i].numero);
        crypto(agenda->contatos[i].Nome, -chave);
        crypto(agenda->contatos[i].numero, -chave);
      }
      fclose(file);
      break;
    case '2':
      // remover
      if (agenda->numContatos == 0) {
        printf("Nenhum contato para remover\n");
        getchar();
        getchar();
        break;
      }
      listarCtts(agenda);
      printf("Digite o numero do contato que voce deseja remover(0 para "
             "cancelar): ");

      int num;
      fflush(stdin);
      scanf(" %d", &num);
      if (num == 0) {
        printf("Operacao cancelada\n");
        break;
      }
      if (num > agenda->numContatos || num < 1) {
        printf("Contato nao encontrado\n");
        getchar();
        getchar();
        break;
      }
      num--;
      FILE *fileold = fopen(
          arquivo,
          "r"); // fechado no final//TODO: achar um jeito melhor de fazer isso
      FILE *filenew = fopen("cttsnew", "w"); // fechado no final
      char line[256];
      int i = 0;
      while (fgets(line, sizeof(line), fileold)) {
        if (i != num * 2 && i != num * 2 + 1) {
          fprintf(filenew, "%s", line);
        }
        i++;
      }
      fclose(fileold);
      fclose(filenew);
      remove("ctts.bak");
      rename(arquivo, "ctts.bak");
      rename("cttsnew", arquivo);
      refrescarmemoria(agenda);
      parseFile(arquivo, agenda);
      printf("Contato removido com sucesso\n");
      break;
    case '3':
      // pesquisar
      if (agenda->numContatos == 0) {
        printf("Nenhum contato para pesquisar\n");
        getchar();
        getchar();
        break;
      }
      printf("Digite a inicial do nome que voce deseja pesquisar ");
      char inicial;
      int achado = 0;
      fflush(stdin);
      scanf(" %c", &inicial);
      for (int i = 0; i < agenda->numContatos; i++) {
        if (agenda->contatos[i].Nome[0] == inicial) {
          printf("(%d) Nome: %s\n", i + 1, agenda->contatos[i].Nome);
          printf("(+) Numero: %s\n", agenda->contatos[i].numero);
          achado = 1;
        }
      }
      printf("Pressione enter para continuar\n");
      fflush(stdin);
      getchar();
      getchar();
      break;
    case '4':
      // listar
      listarCtts(agenda);
      printf("Pressione enter para continuar\n");
      fflush(stdin);
      getchar();
      getchar();
      break;
    case '5':
      // sair duh
      printf("Saindo...\n");
      return;
    }
  }
}
int main() {
  Agenda agenda;
  parseFile(arquivo, &agenda);
  menu(&agenda);
  limparMemoria(&agenda);
  getchar();
  getchar();
  return 0;
}
