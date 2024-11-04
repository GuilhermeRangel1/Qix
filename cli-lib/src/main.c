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

void exibirOpcoesMenu(int opcaoSelecionada) {
    screenSetColor(YELLOW, BLACK);  
    screenGotoxy(MAXX / 2 - 3, 2); 
    printf("QIX GAME");

    
    screenGotoxy(MAXX / 2 - 4, 5); 
    printf("%sJOGAR", opcaoSelecionada == 1 ? "-> " : "   ");

    screenGotoxy(MAXX / 2 - 7, 7); 
    printf("%sINSTRUÇÕES", opcaoSelecionada == 2 ? "-> " : "   ");

    screenGotoxy(MAXX / 2 - 6, 9); 
    printf("%sCRÉDITOS", opcaoSelecionada == 3 ? "-> " : "   ");

    screenGotoxy(MAXX / 2 - 4, 11); 
    printf("%sSAIR", opcaoSelecionada == 4 ? "-> " : "   ");
}

void comeco() {
    screenSetColor(CYAN, BLACK); 
    screenGotoxy(player.x, player.y); 
    printf("%s", player.personagem);
}

void mov(int proxX, int proxY) {
    screenSetColor(BLACK, BLACK); 
    screenGotoxy(player.x, player.y);
    printf(" "); 

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

void menu() {
    int opcao = 1; 
    int tecla;

    while (1) {
        screenClear();
        desenhaMoldura();
        
        
        screenSetColor(YELLOW, BLACK);
        screenGotoxy(MAXX / 2 - 3, 2);
        printf("QIX GAME");
        screenGotoxy(MAXX / 2 - 4, 5);
        printf("%s JOGAR", opcao == 1 ? "->" : "  ");
        screenGotoxy(MAXX / 2 - 7, 7);
        printf("%s INSTRUÇÕES", opcao == 2 ? "->" : "  ");
        screenGotoxy(MAXX / 2 - 6, 9);
        printf("%s CRÉDITOS", opcao == 3 ? "->" : "  ");
        screenGotoxy(MAXX / 2 - 4, 11);
        printf("%s SAIR", opcao == 4 ? "->" : "  ");
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
                    break;
                case 2:
                    screenClear();
                    desenhaMoldura();
                    screenGotoxy(MAXX / 2 - 8, MAXY / 2);
                    printf("Instruções: A ser definido");
                    screenUpdate();
                    getchar();  
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
