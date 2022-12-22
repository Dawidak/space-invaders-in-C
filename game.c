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

//procedura do odtwarzania muzyki
void music(){
    system("START /MIN CMD.EXE /C audio.bat");
}

//procedura do twarzenia linii
void line(int znak, int dlugosc,const char how[]){
    if(how=="style"){
        printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    }else{for (int i = 1; i < dlugosc; i++){printf(how,znak);}}
    printf(how,219);
    printf("\n");
}

//procedura do wyswietlania "klatki" ekranu z akcja
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

//procedura do wyswietlania "klatki" ekranu z HUD-em
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

//ekran startowy dla gry
void gameWelcome(const char sound[],const char mode[]){
    if (sound=="unmuted"){music();}
    line(219,windoWidth+2,mode);
    printf(mode,219); printf("     C INVADERS      ");printf("%c\n",219);
    printf(mode,219);line(254,windoWidth+1,mode);
    
}

//funkcja z przebiegiem gry
//cale badziewie tu ma byc
void gameGame(const char music[],const char mode[]){
    
    int ekran[windowHeight][windoWidth]={0},
        hud  [hudHeight]   [windoWidth]={0};
    player ship;
    ship.hp = 3;    
    
    displayGame(ekran,mode);
    displayHud(hud,ship.hp,mode);
}

//ekran game over
void gameOver(const char music[],const char mode[]){}

// tu testujemy funkcje lub odpalamy kombajn
int main (int argc, char *argv[]) {

    gameGame("muted","%3d");
    gameWelcome("muted","%3d");


    
    
    system("pause");
    return EXIT_SUCCESS;
}