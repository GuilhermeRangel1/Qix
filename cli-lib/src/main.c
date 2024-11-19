#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <time.h> 

#define MAXX 160    
#define MAXY 48
#define MINX 0
#define MINY 0

time_t start, end;

struct jogador {
    int x;
    int y;
    char personagem[2];
};

struct inimigo {
    int x;
    int y;
    int incX;
    int incY;
    int tipoMovimento;  
    char matriz[5][5];  
};

struct grid {
    int x;
    int y;
    char exib[2];
    int borda;
};

struct node {
    int x;
    int y;
    struct node *next;
};

struct no {
    char nome[5];
    double tempo;
    struct no *proximo;
};

struct jogador player = {(MAXX / 2), (MAXY - 2), {'@', '\0'}};
struct inimigo enemy = {2, 2, 2, 2, 0, {  
    {'#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#'}
}};
struct grid matriz[MAXY][MAXX];


char bordaInterna [2] = "O";
char preenchimento [2] = "#";
int total = ((MAXX - 2) * (MAXY - 2) * 75) / 100; 

int maiorAst = 1;

int contador(char *caractere) {
    int count = 0;
    
    for (int i = MINY + 1; i < MAXY - 1; i++) {  
        for (int j = MINX + 1; j < MAXX - 1; j++) {
            if (strcmp(matriz[i][j].exib, caractere) == 0) {
                count++;  
            }
        }
    }

    return count;
}


struct no* criarNo(char* nome, double tempo) {
    struct no* novoNo = (struct no*)malloc(sizeof(struct no));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó.\n");
        return NULL;
    }
    strncpy(novoNo->nome, nome, 5);
    novoNo->tempo = tempo;
    novoNo->proximo = NULL;
    return novoNo;
}

void inserirOrdenado(struct no** head, char* nome, double tempo) {
    struct no* novoNo = criarNo(nome, tempo);
    if (novoNo == NULL) return;

    if (*head == NULL || (*head)->tempo > tempo) {
        
        novoNo->proximo = *head;
        *head = novoNo;
    } else {
       
        struct no* atual = *head;
        while (atual->proximo != NULL && atual->proximo->tempo <= tempo) {
            atual = atual->proximo;
        }
        novoNo->proximo = atual->proximo;
        atual->proximo = novoNo;
    }
}

void lerScores(struct no** head) {
    FILE* arquivo = fopen("scoreboard.txt", "r");
    if (arquivo != NULL) {
        char nome[5];
        double tempo;
        while (fscanf(arquivo, "%s %lf", nome, &tempo) == 2) {
            inserirOrdenado(head, nome, tempo);
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para leitura.\n");
    }
}

void salvarScores(struct no* head) {
    FILE* arquivo = fopen("scoreboard.txt", "w"); 
    if (arquivo != NULL) {
        struct no* atual = head;
        while (atual != NULL) {
            fprintf(arquivo, "%s %.2f\n", atual->nome, atual->tempo);
            atual = atual->proximo;
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para salvar os scores.\n");
    }
}

void liberarLista(struct no* head) {
    struct no* atual = head;
    while (atual != NULL) {
        struct no* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}


void vitoria(struct no** head) {
    end = time(NULL);
    double tempo = difftime(end, start); 
    screenClear();
    screenSetColor(GREEN, BLACK);
    screenGotoxy(MAXX / 2 - 5, MAXY / 2 - 1);
    printf("Você ganhou!");
    screenUpdate();

    while (getchar() != '\n') {}

    screenClear();
    screenSetColor(WHITE, BLACK);
    screenGotoxy(MAXX / 2 - 8, MAXY / 2);
    printf("DIGITE SEU APELIDO:");
    screenUpdate();

    char nome[5] = "";  
    int i = 0;

    while (i < 4) {
        char ch = getchar(); 
        if (ch == 27) {  
            break;
        }

        if (ch != '\n' && ch != '\r') {  
            nome[i] = ch;  
            screenGotoxy(MAXX / 2 + i, MAXY / 2 + 1);
            printf("%c", ch);  
            screenUpdate();
            i++;  
        }
    }
    nome[i] = '\0'; 

    screenClear();
    screenGotoxy(MAXX / 2 - 5, MAXY / 2 + 1);
    printf("Nome: %s | Tempo: %.2f segundos", nome, tempo);  
    screenUpdate();

    inserirOrdenado(head, nome, tempo);

    salvarScores(*head);

    while (getchar() != '\n') {} 
}



void iniciarMatriz() {
    for (int i = MINY + 2; i < MAXY - 1; i++) {  
        for (int j = MINX + 2; j < MAXX - 1; j++) {  
            strcpy(matriz[i][j].exib, " ");
            matriz[i][j].borda = 0;
        }
    }
}

int verificarBordaParaPLayer() {
    if (strcmp(matriz[player.y][player.x].exib, "═") == 0 ||
        strcmp(matriz[player.y][player.x].exib, "║") == 0 ||
        strcmp(matriz[player.y][player.x].exib, "╔") == 0 ||
        strcmp(matriz[player.y][player.x].exib, "╗") == 0 ||
        strcmp(matriz[player.y][player.x].exib, "╚") == 0 ||
        strcmp(matriz[player.y][player.x].exib, "╝") == 0 ||
        strcmp(matriz[player.y][player.x].exib, "O") == 0) {
        return 1;
    } else {
        return 0;
    }
}

void desenhaMoldura() {
    screenSetColor(CYAN, BLACK);
    char exib[4];
    for (int i = MINY + 1; i < MAXY; i++) {
        for (int j = MINX + 1; j < MAXX; j++) {
            screenGotoxy(j, i);
            if (i == MINY + 1) {
                if (j == MINX + 1) strcpy(exib, "╔");
                else if (j == MAXX - 1) strcpy(exib, "╗");  
                else strcpy(exib, "═");
                printf("%s", exib);
                strcpy(matriz[i][j].exib, exib);
            } else if (MINY + 1 < i && i < MAXY - 1) {
                if (j == MINX + 1 || j == MAXX - 1)  {
                    strcpy(exib, "║");
                    printf("%s", exib);
                    strcpy(matriz[i][j].exib, exib);
                }
            } else if (i == MAXY - 1) {
                if (j == MINX + 1) strcpy(exib, "╚");
                else if (j == MAXX - 1) strcpy(exib, "╝");  
                else strcpy(exib, "═");
                printf("%s", exib);
                strcpy(matriz[i][j].exib, exib);
            }
            matriz[i][j].borda = 0;
        }
    }
}

void desenhaInimigo() {
    screenSetColor(RED, BLACK);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int posX = enemy.x + j;
            int posY = enemy.y + i;
            if (enemy.matriz[i][j] == '#') {
                screenGotoxy(posX, posY);
                printf("#");
                strcpy(matriz[posY][posX].exib, "#");
            }
        }
    }
}

void apagarInimigo() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int posX = enemy.x + j;
            int posY = enemy.y + i;
            if (enemy.matriz[i][j] == '#') {
                screenGotoxy(posX, posY);
                printf(" ");
                strcpy(matriz[posY][posX].exib, " ");
            }
        }
    }
}

void comeco() {
    screenSetColor(WHITE, WHITE);
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);

    desenhaInimigo();
}

void preencher (struct node** head) {
    struct node* current = *head;  

    for (int i = MINY + 1; i < MAXY; i++) {
        int j = MINX + 1;
        int flag = 0;

        while (current != NULL) {
            if (i != current->y) {
                current = current->next;
            } else {
                flag = 1;
                break;  
            }
        }

        if (flag == 1) {
            int dif = 0;
            int checkAst = 0;
            while(strcmp(matriz[i][j].exib, " ") != 0 && j < MAXX) {
                j++; 
            } 
            while(j < MAXX) {
                if (strcmp(matriz[i][j].exib, "*") == 0) {
                    checkAst = 1;
                    break;
                }
                else {
                    j++;
                    dif++;
                }            
            }
            j = j - dif;
            
            if (checkAst){
                while(strcmp(matriz[i][j].exib, "*") != 0 && j < MAXX) {
                    screenSetColor(LIGHTMAGENTA, WHITE);
                    strcpy(matriz[i][j].exib, "#");
                    matriz[i][j].borda = 1;
                    screenGotoxy(j, i);
                    printf("#");
                    j++;
                }
            }   
        }
        current = *head;
    }

    
    current = *head;
    while (current != NULL) {  
        screenGotoxy(current->x, current->y);
        screenSetColor(CYAN, BLACK);
        printf("O");
        strcpy(matriz[current->y][current->x].exib, "O");
        current = current->next;  
    }

    
    struct node* proximo;
    current = *head;
    while (current != NULL) {
        proximo = current->next; 
        free(current);           
        current = proximo;       
    }
    *head = NULL;  
}


void add (struct node ** head, int x, int y){
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    struct node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = newNode;
}

void printList(struct node* head) {
    struct node* current = head;  
    if (current == NULL) {
        printf("A lista está vazia.\n");
        return;
    }

    printf("Lista encadeada: ");
    while (current != NULL) {
        printf("x = %d y = %d -> ", current->x, current->y);  
        current = current->next;         
    }
    printf("NULL\n");  
}

struct node *head = NULL;
void mov(int proxX, int proxY) {
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(player.x, player.y);
    if (verificarBordaParaPLayer() == 1) {
        screenSetColor(CYAN, BLACK);
        printf("%s", matriz[player.y][player.x].exib);
        preencher(&head);
    } else {
        printf("*");
        strcpy(matriz[player.y][player.x].exib, "*");
        add(&head, player.x, player.y);
        
    }
    
    player.x = proxX;
    player.y = proxY;

    if (player.x > maiorAst) maiorAst = player.x;

    screenSetColor(WHITE, WHITE);
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);
    screenGotoxy(MINX + 10,  MAXY);
    screenGotoxy(player.x, player.y);
}

int verificarColisao() {
    return (player.x >= enemy.x && player.x < enemy.x + 5 &&
            player.y >= enemy.y && player.y < enemy.y + 5);
}

void verificarcolisaoinimigo() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int posX = enemy.x + j;
            int posY = enemy.y + i;
            if (enemy.matriz[i][j] == '@') {
                screenGotoxy(posX, posY);
                printf(" ");
                strcpy(matriz[posY][posX].exib, " ");
            }
        }
    }
}

int verificarColisaoRastroNaMatriz() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int posX = enemy.x + j;
            int posY = enemy.y + i;

            if (posX >= 0 && posX < MAXX && posY >= 0 && posY < MAXY) {
                if (strcmp(matriz[posY][posX].exib, "*") == 0 && enemy.matriz[i][j] == '#') {
                    return 1;  
                }
            }
        }
    }
    return 0;  
}

void moverInimigo() {
    static int contadorMovimento = 0;

    if (contadorMovimento++ >= 100) {  
        enemy.tipoMovimento = (enemy.tipoMovimento + 1) % 3;  
        contadorMovimento = 0;
    }

    if (verificarColisaoRastroNaMatriz()) {
        screenClear();
        desenhaMoldura();
        screenGotoxy(MAXX / 2 - 5, MAXY / 2);
        printf("GAME OVER");
        screenUpdate();

        while (1) {
            if (keyhit()) {
                int ch = readch();
                if (ch == '\n' || ch == 27) {
                    return;  
                }
            }
        }
    }

    apagarInimigo();

    int newX, newY;

   
    int verificaZonaDeSeguranca(int x, int y) {
        for (int i = -1; i < 5; i++) { 
            for (int j = -1; j < 5; j++) { 
                int posX = x + j;
                int posY = y + i;

                
                if (posX >= MINX && posX < MAXX && posY >= MINY && posY < MAXY) {
                    if (strcmp(matriz[posY][posX].exib, "O") == 0) {
                        return 1; 
                    }
                }
            }
        }
        return 0; 
    }

   
    void calculaProximoMovimento(int *incX, int *incY) {
        newX = enemy.x + *incX;
        newY = enemy.y + *incY;

       
        if (newX <= MINX + 1 || newX >= MAXX - 5 || verificaZonaDeSeguranca(newX, newY)) {
            *incX = -*incX;  
        }
        if (newY <= MINY + 1 || newY >= MAXY - 5 || verificaZonaDeSeguranca(newX, newY)) {
            *incY = -*incY;  
        }
    }

    
    if (enemy.tipoMovimento == 0) {  
        calculaProximoMovimento(&enemy.incX, &enemy.incY);

        enemy.x += enemy.incX;
        enemy.y += enemy.incY;

    } else if (enemy.tipoMovimento == 1) { 
        static int passosRestantes = 0;

        if (passosRestantes <= 0) {
            enemy.incX = (rand() % 3) - 1;  
            enemy.incY = (rand() % 3) - 1;  

            if (enemy.incX == 0 && enemy.incY == 0) {
                enemy.incX = 1;  
            }

            passosRestantes = (rand() % 10) + 5;  
        }

        calculaProximoMovimento(&enemy.incX, &enemy.incY);

        enemy.x += enemy.incX;
        enemy.y += enemy.incY;

        passosRestantes--;

    } else if (enemy.tipoMovimento == 2) {  
        if (player.x > enemy.x) {
            enemy.incX = 1;  
        } else if (player.x < enemy.x) {
            enemy.incX = -1;  
        } else {
            enemy.incX = 0;  
        }

        if (player.y > enemy.y) {
            enemy.incY = 1;  
        } else if (player.y < enemy.y) {
            enemy.incY = -1;  
        } else {
            enemy.incY = 0;  
        }

        calculaProximoMovimento(&enemy.incX, &enemy.incY);

        enemy.x += enemy.incX;
        enemy.y += enemy.incY;
    }

    if (verificarColisaoRastroNaMatriz()) {
        screenClear();
        desenhaMoldura();
        screenGotoxy(MAXX / 2 - 5, MAXY / 2);
        printf("GAME OVER");
        screenUpdate();

        while (1) {
            if (keyhit()) {
                int ch = readch();
                if (ch == '\n' || ch == 27) {
                    return;  
                }
            }
        }
    }

    desenhaInimigo();
}


void exibirScoreboard() {
    FILE* arquivo = fopen("scoreboard.txt", "r");
    if (arquivo == NULL) {
        printf("\n");
        return;
    }

    screenClear();
    desenhaMoldura();

    screenGotoxy((MAXX / 2) - (12 / 2), 2);
    printf("===== TOP SCOREBOARD =====");

    char nome[5];
    double tempo;
    int linha = 4;

    while (fscanf(arquivo, "%s %lf", nome, &tempo) == 2) {
        screenGotoxy((MAXX / 2), linha);  
        printf("%s - %.2f", nome, tempo);
        linha++;
    }
    fclose(arquivo);
    screenUpdate();
    getchar(); 
    screenClear();
    desenhaMoldura();
}

void iniciarJogo() {
    start = time(NULL);
    player.x = 1;
    player.y = MAXY /2;
    enemy.x = 2;
    enemy.y = 2;
    screenClear();
    desenhaMoldura();
    comeco();
    screenUpdate();

    int ch = 0;
    const int inimigoDelay = 100;  

    timerInit(inimigoDelay);

    while (ch != 27) {
        if (keyhit()) {
            ch = readch();
            switch (ch) {
                case 119: 
                    if (player.y  > 1 && matriz[player.y - 1][player.x].borda != 1) {
                        mov(player.x, player.y - 1);
                    }
                    break;
                case 115: 
                    if (player.y < MAXY - 1 && matriz[player.y + 1][player.x].borda != 1) {
                        mov(player.x, player.y + 1);
                    }
                    break;
                case 97: 
                    if (player.x > 1 && matriz[player.y][player.x - 1].borda != 1) {
                        mov(player.x - 1, player.y);
                    }
                    break;
                case 100:  
                    if (player.x  < MAXX - 1 && matriz[player.y][player.x + 1].borda != 1) {
                        if (player.y == MINY + 1 || player.y == MAXY - 1) {
                            if (player.x + 1 <= maiorAst) mov(player.x + 1, player.y);
                        }
                        else mov(player.x + 1, player.y);
                    }
                    break;
                default:
                    break;
            }
        }
         
        if((contador(bordaInterna)+ contador(preenchimento)) > total){
            struct no* head = NULL;
            lerScores(&head);
            vitoria(&head);
            liberarLista(head);
                        while (1) {
                if (keyhit()) {
                    ch = readch();
                    if (ch == '\n' || ch == 27) {
                        return;
                    }
                }
            }
        } 

        if (timerTimeOver()) {  
            moverInimigo();
            timerUpdateTimer(inimigoDelay);
        }

        if (verificarColisaoRastroNaMatriz() || verificarColisao()) {
            screenClear();
            desenhaMoldura();
            screenGotoxy(MAXX / 2 - 5, MAXY / 2);
            printf("GAME OVER");
            screenUpdate();

            while (1) {
                if (keyhit()) {
                    ch = readch();
                    if (ch == '\n' || ch == 27) {
                        return;
                    }
                }
            }
        }

        screenUpdate();
    }
}

void exibirInstrucoes() {
    screenClear();
    desenhaMoldura();
    screenGotoxy((MAXX / 2) - (12 / 2), MAXY / 4);
    printf("Instruções do Jogo");
    screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 4 + 1);
    printf("1. Use as teclas WASD para mover o jogador (@)");
    screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 4 + 2);
    printf("2. Evite os inimigos representados por #");
    screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 4 + 3);
    printf("3. Preencha a tela com #");
    screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 4 + 4);
    printf("4. Pressione 'Esc' para sair");
    screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 4 + 5);
    printf("5. Tente completar o jogo no menor tempo possível");
    screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 4 + 6);
    printf("Boa sorte!");
    screenUpdate();
    readch();
    screenClear();
    desenhaMoldura();
}

void menu() {
    int opcao = 1;
    int ch = 0;
    int tituloY = MAXY / 6;
    int primeiraOpcaoY = (MAXY / 6) * 2;

    screenClear();
    desenhaMoldura();

    while (1) {
        screenGotoxy(MAXX / 2 - 3, tituloY);
        printf("QIX GAME");

        screenGotoxy(MAXX / 2 - 4, primeiraOpcaoY);
        printf("%sJOGAR", opcao == 1 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 7, primeiraOpcaoY + 2);
        printf("%sINSTRUÇÕES", opcao == 2 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 6, primeiraOpcaoY + 4);
        printf("%sCRÉDITOS", opcao == 3 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 7, primeiraOpcaoY + 6);
        printf("%sSCOREBOARD", opcao == 4 ? "-> " : "   ");  

        screenGotoxy(MAXX / 2 - 4, primeiraOpcaoY + 8);
        printf("%sSAIR", opcao == 5 ? "-> " : "   ");  
        
        screenUpdate();

        if (keyhit()) {
            ch = readch();

            switch (ch) {
                case 'w':
                    opcao--;
                    if (opcao < 1) opcao = 5;
                    break;
                case 's':
                    opcao++;
                    if (opcao > 5) opcao = 1;
                    break;
                case '\n':
                    switch (opcao) {
                        case 1: 
                            iniciarMatriz();
                            iniciarJogo();
                            screenClear();
                            desenhaMoldura();
                            break;
                        case 2: 
                            exibirInstrucoes();
                            break;
                        case 3: 
                            screenClear();
                            desenhaMoldura();
                            screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 4);
                            printf("Programador e Game Designer: Guilherme Vinícius");
                            screenGotoxy((MAXX / 2) - (23 / 2), MAXY / 4 + 1);
                            printf("Programador: Arthur Xavier");
                            screenGotoxy((MAXX / 2) - (28 / 2), MAXY / 4 + 2);
                            printf("Programador: Antônio Laprovitera");
                            screenUpdate();
                            readch();
                            screenClear();
                            desenhaMoldura();
                            break;
                        case 4: 
                            exibirScoreboard(); 
                            break;
                        case 5:
                            screenClear();
                            desenhaMoldura();
                            screenGotoxy(MAXX / 2 - 8, MAXY / 2);
                            printf("Saindo do jogo...");
                            screenUpdate();
                            return;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

int main() {
    iniciarMatriz();
    screenInit(1);
    keyboardInit();
    timerInit(30);
    menu();
    keyboardDestroy();
    screenDestroy();
    return 0;
}