#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

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
    char personagem[4][4]; // Matriz 4x4 para o inimigo circular
};

struct grid {
    int x;
    int y;
    char exib[2];
    int borda;
};

struct jogador player = {(MAXX / 2), (MAXY - 2), {'@', '\0'}};
struct inimigo enemy = {2, 2, 1, 1, {{'#', '#', '#', '#'},
                                      {'#', ' ', ' ', '#'},
                                      {'#', ' ', ' ', '#'},
                                      {'#', '#', '#', '#'}}}; // Inimigo circular 4x4
struct grid matriz[MAXY][MAXX];

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
    for (int i = 0; i < 4; i++) { // Exibe a matriz 4x4 do inimigo circular
        for (int j = 0; j < 4; j++) {
            screenGotoxy(enemy.x + j, enemy.y + i);
            printf("%c", enemy.personagem[i][j]);
        }
    }
}

void mov(int proxX, int proxY) {
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(player.x, player.y);
    if (verificarBordaParaPLayer() == 1){
        screenSetColor(CYAN, BLACK);
        printf("%s", matriz[player.y][player.x].exib);
    }
    else {
        printf("a");
        strcpy(matriz[player.y][player.x].exib, "a");  // Marca o rastro do jogador na matriz
    }
    
    player.x = proxX;
    player.y = proxY;

    screenSetColor(WHITE, WHITE);
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);
}

// Função para verificar colisão do inimigo com o rastro do jogador
int verificarColisaoComRastro() {
    for (int i = 0; i < 4; i++) { // Loop sobre a matriz 4x4 do inimigo
        for (int j = 0; j < 4; j++) {
            int posX = enemy.x + j;
            int posY = enemy.y + i;

            // Verifica se a posição do inimigo colide com o rastro ("a")
            if (enemy.personagem[i][j] == '#' && strcmp(matriz[posY][posX].exib, "a") == 0) {
                return 1; // Colisão detectada com o rastro
            }
        }
    }
    return 0; // Sem colisão
}

int verificarColisao() {
    // Verifica colisão entre o jogador e o inimigo circular 4x4
    for (int i = 0; i < 4; i++) { // Loop sobre a matriz 4x4 do inimigo
        for (int j = 0; j < 4; j++) {
            int posX = enemy.x + j;
            int posY = enemy.y + i;

            // Verifica se há colisão com o jogador
            if (enemy.personagem[i][j] == '#' && player.x == posX && player.y == posY) {
                return 1; // Colisão detectada com o jogador
            }
        }
    }
    return 0; // Sem colisão com o jogador
}

void moverInimigo() {
    screenSetColor(BLACK, BLACK);
    for (int i = 0; i < 4; i++) { // Apaga o inimigo circular de 4x4
        for (int j = 0; j < 4; j++) {
            if (enemy.y + i >= MINY && enemy.y + i < MAXY && enemy.x + j >= MINX && enemy.x + j < MAXX) {
                screenGotoxy(enemy.x + j, enemy.y + i);
                printf(" ");
                strcpy(matriz[enemy.y + i][enemy.x + j].exib, " ");
            }
        }
    }

    int newX = enemy.x + enemy.incX;
    int newY = enemy.y + enemy.incY;

    // Checa colisão com bordas, considerando a largura e altura do inimigo
    if (newX <= MINX + 1 || newX + 3 >= MAXX - 1) {
        enemy.incX = -enemy.incX; // Rebater nas bordas laterais
    }
    if (newY <= MINY + 1 || newY + 3 >= MAXY - 1) {
        enemy.incY = -enemy.incY; // Rebater na borda superior ou inferior
    }

    enemy.x += enemy.incX;
    enemy.y += enemy.incY;

    screenSetColor(RED, BLACK);
    for (int i = 0; i < 4; i++) { // Desenha o inimigo circular de 4x4
        for (int j = 0; j < 4; j++) {
            if (enemy.y + i >= MINY && enemy.y + i < MAXY && enemy.x + j >= MINX && enemy.x + j < MAXX) {
                screenGotoxy(enemy.x + j, enemy.y + i);
                printf("%c", enemy.personagem[i][j]);
                if (enemy.personagem[i][j] == '#') {
                    matriz[enemy.y + i][enemy.x + j].exib[0] = '#';
                }
            }
        }
    }
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

        // Verifica colisão com o jogador ou com o rastro
        if (verificarColisao() || verificarColisaoComRastro()) {
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
    for (int i = MINY + 1; i <= MAXY - 1; i++){
        for (int j = MINX + 1; j <= MAXX - 1; j++) {
            strcpy(matriz[i][j].exib, " ");
        }
    }
    screenInit(1);
    keyboardInit();
    timerInit(30);  

    menu();

    keyboardDestroy();
    screenDestroy();
    return 0;
}
