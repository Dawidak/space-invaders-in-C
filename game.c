#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define windoWidth 21
#define windowHeight 15
#define hudHeight 1

typedef struct{
    int x, y;
    int hp;
    int score;
} player;

typedef struct{
    int x, y;
    int hp;
    int dead;
} enemy;

typedef struct{
    int x, y;
    int dmg;
    int speed;
    int hit;
} bullet;




void line(int znak, int dlugosc,const char how[]){
    if(how=="style"){
        printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    }else{for (int i = 1; i < dlugosc; i++){printf(how,znak);}}
    printf(how,219);
    printf("\n");
}

void displayGame(int ekran[][windoWidth],const char how[] ){
    
    printf(how,219);
    line(254,windoWidth+1,how);
    
    for (int i = 0; i < windowHeight; i++){
        printf(how,219);
        
        for (int j = 0; j < windoWidth; j++){
            printf(how,ekran[i][j]);
        }
        
        printf(how,219);
        printf("\n");
    }

    printf(how,219);
    line(254,windoWidth+1,how);

}

void displayHud(int hud[][windoWidth],int health,const char how[]){
 
    hud[0][1] = 72;
    hud[0][2] = 80;
    hud[0][3] = 222;
 
    switch (health)
    {
    case 1:
        hud[0][5]=003;
        hud[0][6]=32;
        hud[0][7]=32;
    case 2:
        hud[0][5]=003;
        hud[0][6]=003;
        hud[0][7]=32;
    case 3:
        hud[0][5]=003;
        hud[0][6]=003;
        hud[0][7]=003;
        break;
    }
   
    
    for (int i = 0; i < hudHeight; i++){
        printf(how,219);
        
        for (int j = 0; j < windoWidth; j++){
            //if(hud[i][j]==222){printf(":");}
            //else{
                printf(how,hud[i][j]);
                //}
        }
        
        printf(how,219);
        printf("\n");
    }

    printf(how,219);
    line(254,windoWidth+1,how);

}


int main (int argc, char *argv[]) {
    int ekran[windowHeight][windoWidth]={0},
        hud  [hudHeight]   [windoWidth]={0};
    
    player ship;
    ship.hp = 3;
    displayGame(ekran,"%3d");
    displayHud(hud,ship.hp,"%3d");
    //displayGame(ekran,"%c");
    //displayHud(hud,ship.hp,"%c");
    
    system("pause");
    return EXIT_SUCCESS;
}