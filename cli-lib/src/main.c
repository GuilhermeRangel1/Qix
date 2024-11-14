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
    int tipoMovimento;  
    char matriz[5][5];  
};

struct grid {
    int x;
    int y;
    char exib[2];
    int borda;
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


void iniciarJogo();

void iniciarMatriz() {
    for (int i = MINY + 2; i < MAXY - 1; i++) {  
        for (int j = MINX + 2; j < MAXX - 1; j++) {  
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

void mov(int proxX, int proxY) {
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(player.x, player.y);
    if (verificarBordaParaPLayer() == 1) {
        screenSetColor(CYAN, BLACK);
        printf("%s", matriz[player.y][player.x].exib);
    } else {
        printf("*");
        strcpy(matriz[player.y][player.x].exib, "*");
    }
    
    player.x = proxX;
    player.y = proxY;

    screenSetColor(WHITE, WHITE);
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);
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

    if (enemy.tipoMovimento == 0) {  
        int newX = enemy.x + enemy.incX;
        int newY = enemy.y + enemy.incY;

        if (newX <= MINX + 1 || newX >= MAXX - 5) {
            enemy.incX = -enemy.incX;  
        }
        if (newY <= MINY + 1 || newY >= MAXY - 5) {
            enemy.incY = -enemy.incY;  
        }

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

        enemy.x += enemy.incX;
        enemy.y += enemy.incY;

        passosRestantes--;

        if (enemy.x <= MINX + 1) {
            enemy.x = MINX + 2;
            enemy.incX = 1;
            passosRestantes = 5;
        }
        if (enemy.x >= MAXX - 5) {
            enemy.x = MAXX - 6;
            enemy.incX = -1;
            passosRestantes = 5;
        }
        if (enemy.y <= MINY + 1) {
            enemy.y = MINY + 2;
            enemy.incY = 1;
            passosRestantes = 5;
        }
        if (enemy.y >= MAXY - 5) {
            enemy.y = MAXY - 6;
            enemy.incY = -1;
            passosRestantes = 5;
        }

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

      
        int newX = enemy.x + enemy.incX;
        int newY = enemy.y + enemy.incY;

       
        if (newX <= MINX + 1 || newX >= MAXX - 5) {
            enemy.incX = -enemy.incX;
        }
        if (newY <= MINY + 1 || newY >= MAXY - 5) {
            enemy.incY = -enemy.incY;
        }

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
