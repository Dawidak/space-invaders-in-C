#include <stdio.h>
#include <stdlib.h>
#define windoWidth 21
#define windowHeight 15
#define hudHeight 3


void box(int ekran[][windoWidth]){
    for (int i = 0; i < windowHeight; i++){for (int j = 0; j < windoWidth; j++){ekran[i][j] = 219;}} 
    for (int i = 1; i < windowHeight-1; i++){for (int j = 1; j < windoWidth-1; j++){ekran[i][j] = 45;}}
}

void line(int znak, int dlugosc,const char how[],const char color[]){
    if(how=="style"){
        printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    }else{for (int i = 0; i < dlugosc; i++){printf(color);printf(how,znak);}}
    printf("\n");
}

void display(int ekran[][windoWidth],int hud[][windoWidth],const char how[] ){
    //box(ekran);
    line(219,windoWidth+2,how,"\033[1;31m");
    
    for (int i = 0; i < windowHeight; i++){
        printf(how,219);
        
        for (int j = 0; j < windoWidth; j++){
            printf(how,ekran[i][j]);
        }
        
        printf(how,219);
        printf("\n");
    }

    line(219,windoWidth+2,how,"");

    for (int i = 0; i < hudHeight; i++){
        printf(how,219);
        
        for (int j = 0; j < windoWidth; j++){
            printf(how,ekran[i][j]);
        }
        
        printf(how,219);
        printf("\n");
    }
    
    line(219,windoWidth+2,how,"");
}

int main (int argc, char *argv[]) {
    int ekran[windowHeight][windoWidth]={0},
        hud  [hudHeight][windoWidth]={0};
    display(ekran,hud,"%3d");
    display(ekran,hud,"%c");
    
    system("pause");
    return EXIT_SUCCESS;
}