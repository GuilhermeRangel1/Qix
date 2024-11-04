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
        screenGotoxy(x, MAXY);
        printf("═");
    }

    for (int y = 0; y <= MAXY; y++) {
        screenGotoxy(0, y);
        printf("║");
        screenGotoxy(MAXX - 1, y);
        printf("║");
    }

    screenGotoxy(0, 0);
    printf("╔");
    screenGotoxy(MAXX - 1, 0);
    printf("╗");
    screenGotoxy(0, MAXY);
    printf("╚");
    screenGotoxy(MAXX - 1, MAXY);
    printf("╝");
}

void comeco() {
    screenSetColor(CYAN, BLACK); 
    screenGotoxy(player.x, player.y); 
    printf("%s", player.personagem);
}

int check_XY(int playerX, int playerY) {
    int n = 0;
    
    if (playerX == MAXX - 1 && playerY == MAXY) {
        n = 6;
    }
    else if (playerX == MINX && playerY == MAXY) {
        n = 5;
    }
    else if (playerX == MAXX -1 && playerY == 1) {
        n = 4;
    }
    else if (playerX == MINX && playerY == 1) {
        n = 3;
    }
    else if (playerY == MINY || playerY == MAXY) {
        n = 2;
    }
    else if (playerX == MINX || playerX == MAXX - 1) {
        n = 1;
    }
    return n;
}

void mov(int proxX, int proxY) {
    screenSetColor(CYAN, BLACK); 
    screenGotoxy(player.x, player.y);
    if (check_XY(player.x, player.y) == 1){
        printf("║"); 
    }
    else if (check_XY(player.x, player.y) == 2){
        printf("═");
    }
    else if (check_XY(player.x, player.y) == 3){
        printf("╔");
    }
    else if (check_XY(player.x, player.y) == 4){
        printf("╗");
    }
    else if (check_XY(player.x, player.y) == 5){
        printf("╚");
    }
    else if (check_XY(player.x, player.y) == 6){
        printf("╝");
    }
    else if (check_XY(player.x, player.y) == 0){
        printf("*");
    }
    

    player.x = proxX;
    player.y = proxY;

    screenSetColor(CYAN, BLACK); 
    screenGotoxy(player.x, player.y);
    printf("%s", player.personagem);
}



void iniciarJogo() {
    screenClear();  
    desenhaMoldura(); 
    comeco();  
    screenUpdate();

    int ch = 0;
    while (ch != 27) {  
        if (keyhit()) {
            ch = readch();
            switch (ch) {
                case 119: 
                        if (player.y > MINY){
                            mov(player.x, player.y - 1);
                        }
                        
                    break;
                case 115: 
                    if (player.y < MAXY) {
                        mov(player.x, player.y + 1);
                    }
                    break;
                case 97: 
                    if (player.x > MINX) { 
                        mov(player.x - 1, player.y);
                    }
                    break;
                case 100: 
                    if (player.x < MAXX - 1) { 
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

void menu() {
    int opcao = 1; 
    int tecla;

    screenClear();  
    desenhaMoldura();  

    while (1) {
        
        screenSetColor(YELLOW, BLACK);
        screenGotoxy(MAXX / 2 - 3, 2);
        printf("QIX GAME");

        screenGotoxy(MAXX / 2 - 4, 5);
        printf("%sJOGAR", opcao == 1 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 7, 7);
        printf("%sINSTRUÇÕES", opcao == 2 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 6, 9);
        printf("%sCRÉDITOS", opcao == 3 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 4, 11);
        printf("%sSAIR", opcao == 4 ? "-> " : "   ");
        
        screenUpdate();

        tecla = getchar();

        if (tecla == 'w') {  
            opcao--;
            if (opcao < 1) opcao = 4;  
        } else if (tecla == 's') {  
            opcao++;
            if (opcao > 4) opcao = 1;  
        } else if (tecla == '\n') { 
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
                    getchar();  
                    screenClear();
                    desenhaMoldura();
                    break;
                case 3:
                    screenClear();
                    desenhaMoldura();
                    screenGotoxy(MAXX / 2 - 8, MAXY / 2);
                    printf("Créditos: Guilherme Vinícius");
                    screenGotoxy(MAXX / 2 - 8, MAXY / 2 + 1);
                    printf("Créditos: Arthur Xavier");
                    screenGotoxy(MAXX / 2 - 8, MAXY / 2 + 2);
                    printf("Créditos: Antônio Laprovitera");
                    
                    screenUpdate();
                    getchar();  
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
        }
    }
}

int main() {
    screenInit(1);
    keyboardInit();
    
    menu();  

    keyboardDestroy();
    screenDestroy();
    return 0;
}
