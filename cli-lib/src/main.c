#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

struct jogador{
    int x;
    int y;
    char personagem[2];
};

struct qix{
    int x;
    int y;
};

struct jogador player = {0,0,'@'};

void comeco() {
    int centerX = (MAXX - strlen(player.personagem)) / 2;
    int fimTela = MAXY;
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(centerX, fimTela);
    printf("%s",player.personagem);
}

void mov(int proxX,int proxY){
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(player.x, player.y);
    printf(" ");
    player.x = proxX;
    player.y = proxY;
    screenGotoxy(proxX,proxY);
    printf("%s",player.personagem);
}

int main() {
    screenInit(1);
    keyboardInit();

    comeco();

    screenUpdate();
    getchar();

    keyboardDestroy();
    screenDestroy();
    return 0;}
