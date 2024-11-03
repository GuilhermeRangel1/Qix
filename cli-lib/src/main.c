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

struct jogador player = {(MAXX / 2), (MAXY - 2), {'@', '\0'}}; 

void desenhaMoldura() {
    screenSetColor(CYAN, BLACK);
    
    for (int x = 0; x < MAXX; x++) {
        screenGotoxy(x, 0);
        printf("═");
        screenGotoxy(x, MAXY - 1);
        printf("═");
    }

    for (int y = 0; y < MAXY; y++) {
        screenGotoxy(0, y);
        printf("║");
        screenGotoxy(MAXX - 1, y);
        printf("║");
    }

    screenGotoxy(0, 0);
    printf("╔");
    screenGotoxy(MAXX - 1, 0);
    printf("╗");
    screenGotoxy(0, MAXY - 1);
    printf("╚");
    screenGotoxy(MAXX - 1, MAXY - 1);
    printf("╝");
}

void exibirOpcoesMenu() {
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(MAXX / 2 - 4, 2);
    printf("QIX GAME");

    screenGotoxy(MAXX / 2 - 8, 5);
    printf("1. JOGAR");

    screenGotoxy(MAXX / 2 - 8, 7);
    printf("2. INSTRUÇÕES");

    screenGotoxy(MAXX / 2 - 8, 9);
    printf("3. CRÉDITOS");

    screenGotoxy(MAXX / 2 - 8, 11);
    printf("4. SAIR");

    screenGotoxy(MAXX / 2 - 10, 13);
    printf("Pressione ENTER para começar...");
}

void menu() {
    screenClear();       
    desenhaMoldura();      
    exibirOpcoesMenu();    
    screenUpdate();

    getchar(); 
    screenClear(); 
    desenhaMoldura();
}

void comeco() {
    int centerX = MAXX / 2;
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(centerX, player.y); 
    printf("%s", player.personagem);
}

void mov(int proxX, int proxY) {
    screenSetColor(BLACK, BLACK); 
    screenGotoxy(player.x, player.y);
    printf(" "); 

    player.x = proxX;
    player.y = proxY;

    screenSetColor(CYAN, DARKGRAY); 
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);
}

void iniciarJogo() {
    desenhaMoldura(); 
    comeco();         
    screenUpdate();

    int ch = 0;
    while (ch != 27) { 
        if (keyhit()) {
            ch = readch();
            switch (ch) {
                case 119: 
                    if (player.y - 1 > MINY) { 
                        mov(player.x, player.y - 1);
                    }
                    break;
                case 115: 
                    if (player.y + 1 < MAXY - 1) {
                        mov(player.x, player.y + 1);
                    }
                    break;
                case 97: 
                    if (player.x - 1 > MINX) { 
                        mov(player.x - 1, player.y);
                    }
                    break;
                case 100: 
                    if (player.x + 1 < MAXX - 1) { 
                        mov(player.x + 1, player.y);
                    }
                    break;
                default:
                    break;
            }
            screenUpdate();
        }
    }
}
int main() {
    screenInit(1);
    keyboardInit();
    
    menu();         
    iniciarJogo();  

    keyboardDestroy();
    screenDestroy();
    return 0;
}

