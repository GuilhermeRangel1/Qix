#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int main(){

    screenInit(1); 

    screenSetColor(LIGHTGREEN, BLACK);

    int x = (MAXX - 12) / 2;
    int y = MAXY / 2; 
    screenGotoxy(x, y); 
    printf("Hello, World!\n"); 
    
    getchar(); 
    screenDestroy();

    return 0;}
