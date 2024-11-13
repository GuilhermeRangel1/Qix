#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#define MAXX 160
#define MAXY 48
#define MINX 0 
#define MINY 0
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
    char personagem[2];
};

struct grid {
    int x;
    int y;
    char exib[2];
    int borda;
};

struct jogador player = {(MAXX / 2), (MAXY - 2), {'@', '\0'}};
struct inimigo enemy = {2, 2, 1, 1, {'#', '\0'}};
struct grid matriz[MAXY][MAXX];

void iniciarMatriz(){
    for (int i = MINY + 1; i <= MAXY - 1; i++){
        for (int j = MINX + 1; j <= MAXX - 1; j++) {
            strcpy(matriz[i][j].exib, " ");
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
        strcmp(matriz[player.y][player.x].exib, "*") == 0) {
            return 1;
    } 
    else{
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
                else {strcpy(exib, "═");
                }
                printf("%s", exib);
                strcpy(matriz[i][j].exib, exib);
            
            } else if (MINY + 1< i && i < MAXY - 1) {
                if (j == MINX + 1 || j == MAXX - 1)  {
                    strcpy(exib, "║");
                    printf("%s", exib);
                    strcpy(matriz[i][j].exib, exib);
                }
            } else if (i == MAXY - 1) {
                if (j == MINX + 1) strcpy(exib, "╚");
                else if (j == MAXX - 1) strcpy(exib, "╝");  
                else {strcpy(exib, "═");
                }
                printf("%s", exib);
                strcpy(matriz[i][j].exib, exib);
            } 
            
        }
    }
}

void comeco() {
    screenSetColor(WHITE, WHITE);
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);

    screenSetColor(RED, BLACK);
    screenGotoxy(enemy.x, enemy.y);
    printf("%s", enemy.personagem);
}

void mov(int proxX, int proxY) {
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(player.x, player.y);
    if (verificarBordaParaPLayer() == 1){
        screenSetColor(CYAN, BLACK);
        printf("%s", matriz[player.y][player.x].exib);
    }
    else {
        printf("*");
        strcpy(matriz[player.y][player.x].exib, "*");
    }
    
    player.x = proxX;
    player.y = proxY;

    screenSetColor(WHITE, WHITE);
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);
    
}
int colisaoRastro(int x, int y) {
    if (strcmp(matriz[y][x].exib, "*") == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int verificarColisao() {
    return (player.x == enemy.x && player.y == enemy.y || strcmp(matriz[enemy.y + enemy.incY][enemy.x + enemy.incX].exib, "*") == 0);
}

void moverInimigo() {
    screenSetColor(BLACK, BLACK);
    screenGotoxy(enemy.x, enemy.y);
    printf(" ");
    strcpy(matriz[enemy.y][enemy.x].exib, " ");

    int newX = enemy.x + enemy.incX;
    int newY = enemy.y + enemy.incY;

    if (strcmp(matriz[newY][newX].exib, "║") == 0 || strcmp(matriz[newY][newX].exib, "*") == 0) {
        enemy.incX = -enemy.incX;
    }
    if (strcmp(matriz[newY][newX].exib, "═") == 0 || strcmp(matriz[newY][newX].exib, "*") == 0) {
        enemy.incY = -enemy.incY;
    }
    if (strcmp(matriz[newY][newX].exib, "╔") == 0 || 
        strcmp(matriz[newY][newX].exib, "╗") == 0 ||
        strcmp(matriz[newY][newX].exib, "╚") == 0 ||
        strcmp(matriz[newY][newX].exib, "╝") == 0 ) {
        enemy.incX = -enemy.incX;
        enemy.incY = -enemy.incY;
    }
    verificarColisao();
    enemy.x += enemy.incX;
    enemy.y += enemy.incY;
    
   

    screenSetColor(RED, BLACK);
    screenGotoxy(enemy.x, enemy.y);
    printf("%s", enemy.personagem);
    strcpy(matriz[enemy.y][enemy.x].exib, enemy.personagem);
}



void iniciarJogo() {
    player.x = MAXX / 2;
    player.y = MAXY - 1;
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
                    if (player.y  > 1) {
                        mov(player.x, player.y - 1);
                    }
                    break;
                case 115: 
                    if (player.y < MAXY - 1) {
                        mov(player.x, player.y + 1);
                    }
                    break;
                case 97: 
                    if (player.x > 1) {
                        mov(player.x - 1, player.y);
                    }
                    break;
                case 100:  
                    if (player.x  < MAXX - 1) {
                        mov(player.x + 1, player.y);
                    }
                    break;
                default:
                    break;
            }
        }

        if (timerTimeOver()) {  
            moverInimigo();
            timerUpdateTimer(inimigoDelay);  
        }

        if (verificarColisao()) {
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

void menu() {
    int opcao = 1; 
    int ch = 0;
    int tituloY = MAXY / 6;  
    int primeiraOpcaoY = (MAXY / 6) * 2;  

    screenClear();  
    desenhaMoldura(); 

    while (1) {
        screenSetColor(YELLOW, BLACK);
        screenGotoxy(MAXX / 2 - 3, tituloY);
        printf("QIX GAME");

        screenGotoxy(MAXX / 2 - 4, primeiraOpcaoY);
        printf("%sJOGAR", opcao == 1 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 7, primeiraOpcaoY + 2);
        printf("%sINSTRUÇÕES", opcao == 2 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 6, primeiraOpcaoY + 4);
        printf("%sCRÉDITOS", opcao == 3 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 4, primeiraOpcaoY + 6);
        printf("%sSAIR", opcao == 4 ? "-> " : "   ");
        
        screenUpdate();

        if (keyhit()) {
            ch = readch(); 

            switch (ch) {
                case 'w':  
                    opcao--;
                    if (opcao < 1) opcao = 4;  
                    break;
                case 's': 
                    opcao++;
                    if (opcao > 4) opcao = 1;  
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
                            screenClear();
                            desenhaMoldura();
                            screenGotoxy(MAXX / 2 - 8, MAXY / 2);
                            printf("Instruções: A ser definido");
                            screenUpdate();
                            readch();  
                            screenClear();
                            desenhaMoldura();
                            break;
                        case 3: 
                            screenClear();
                            desenhaMoldura();
                            screenGotoxy((MAXX / 2) - (42 / 2), MAXY / 2);
                            printf("Programador e Game Designer: Guilherme Vinícius");
                            screenGotoxy((MAXX / 2) - (23 / 2), MAXY / 2 + 1);
                            printf("Programador: Arthur Xavier");
                            screenGotoxy((MAXX / 2) - (28 / 2), MAXY / 2 + 2);
                            printf("Programador: Antônio Laprovitera");
                            screenUpdate();
                            readch();  
                            screenClear();
                            desenhaMoldura();
                            break;
                        case 4:  
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