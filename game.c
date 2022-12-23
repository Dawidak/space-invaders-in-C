#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Tryb wyswietlania
#define displayMode "%c"

//rozmiary obszarow roboczych
//rozmiar ekranu gry -> windowHeight x windowWidth
//rozmiar HUD -> hudHeight x windowWidth
#define windoWidth 21
#define windowHeight 15
#define hudHeight 1

//definy dotyczace dzwieku
#define musicON 1
#define musicLocation "space_invaders_loop.mp3"
#define death "deadth.wav"

typedef struct{
    //position
    int x, y;
    //hit points
    int hp;
    //score
    int score;
} player;

typedef struct{
    //podition
    int x, y;
    //hit points
    int hp;
    //status 1 for dead, 0 for alive
    int dead;
} enemy;

typedef struct{
    int x, y;
    int dmg;
    int speed;
    int hit;
} bullet;

player ship;

//procedura do twarzenia linii poziomych
// znak podaj w ascii
void line(int znak, int dlugosc){
    for (int i = 1; i < dlugosc; i++){printf(displayMode,znak);}
    printf(displayMode,219);
    printf("\n");
}

//procedura do wyswietlania "klatki" ekranu z akcja
void displayGame(int ekran[][windoWidth]){
    ship.x = 3;
    ship.y = 4;
    system("cls");
    ekran[ship.x][ship.y]==69;

    if(ship.hp == 0) printf("\033[0;30m"); //czarny tekst if ded gracz

    printf(displayMode,219);
    line(254,windoWidth+1);

    for (int i = 0; i < windowHeight; i++){
        printf(displayMode,219);
        
        if(i == 7 && ship.hp == 0) //czerwony game over if ded gracz
            printf("\033[1;31m     GAME OVER      \033[0;30m ");
        else
            for (int j = 0; j < windoWidth; j++){
                if(ekran[i][j] == 0){printf(displayMode,ekran[i][j]);}
                else printf(displayMode,ekran[i][j]);
            }
        
        printf(displayMode,219);
        printf("\n");
    }

    printf(displayMode,219);
    line(254,windoWidth+1);

}

//procedura do wyswietlania "klatki" ekranu z HUD-em
void displayHud(int hud[][windoWidth],int health){
 
    hud[0][1] = 72;
    hud[0][2] = 80;
    hud[0][3] = 222;
 
    switch (health)
    {
    case 1:
        hud[0][5]=003;
        hud[0][6]=32;
        hud[0][7]=32;
        break;
    case 2:
        hud[0][5]=003;
        hud[0][6]=003;
        hud[0][7]=32;
        break;
    case 3:
        hud[0][5]=003;
        hud[0][6]=003;
        hud[0][7]=003;
        break;
    default:
        
        break;
    }
   
    
    for (int i = 0; i < hudHeight; i++){
        printf(displayMode,219);
        
        for (int j = 0; j < windoWidth; j++){
            if(hud[i][j]==222 && ship.hp != 0){printf(" \033[1;31m");}
            else{
                if(hud[i][j] == 0) printf(" ");
                else printf(displayMode,hud[i][j]);
            }
        }
        if(ship.hp != 0) printf("\033[0m"); //idk how but it doesn't wor eather way
        
        printf(displayMode,219);
        printf("\n");
    }

    printf(displayMode,219);
    line(254,windoWidth+1);

}

//ekran startowy dla gry
void gameWelcome(){
    
    int height = 6;
    char colors[] = "#50C1D9";

    for (int i = 0; i < height; i++)
    {
        system("cls");

        for (int j = 0; j < i; j++){printf("\n");}
        
        printf("       C INVADERS       \n\n\n\n\n");
        
        if(i%2==0){system("color 70");}
        else{system("color 07");}
        
        for (int j = 0; j < height-i; j++){printf("\n");}
        
        system("timeout 1 > NUL");
    }
    system("color 07");
    system("timeout 2 > NUL");
    
    
}

//funkcja z przebiegiem gry
//cale badziewie tu ma byc
void gameGame(const char music[]){
    
    int ekran[windowHeight][windoWidth]={0},
        hud  [hudHeight]   [windoWidth]={0};
    ship.hp = 3;

    displayGame(ekran);
    //displayHud(hud,ship.hp);
}

//ekran game over
void gameOver(const char music[]){}

// tu testujemy funkcje lub odpalamy kombajn
int main (int argc, char *argv[]) {

    //gameGame("muted","%c");  //%3d dla cyferek | %c dla znakow
    //gameWelcome("unmuted","%c"); // to co wyżej
    gameGame("muted");


    //system("pause");
    return EXIT_SUCCESS;
}