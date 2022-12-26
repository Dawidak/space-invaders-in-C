#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//definy dotyczace dzwieku
// Czy muzyka wlaczona 1 TRUE 0 FALSE
#define musicON 1
#define musicLocation "mainTheme.bat"
#define death "mainDeathSound.bat"

//define kolorki
#define RED "\033[1;31m"

/*          *\
|   OPTIONS  |
\*          */          

//Tryb wyswietlania
#define displayMode "%c"

//rozmiary obszarow roboczych
//rozmiar ekranu gry -> windowHeight x windowWidth
//rozmiar HUD -> hudHeight x windowWidth
#define windoWidth 21
#define windowHeight 15
#define hudHeight 1

void killMusic();

typedef struct{
    //position
    int position[1][2];
    //hit points
    int hp;
    //score
    int score;
} player;

typedef struct{
    //position
    int position[1][2];
    //hit points
    int hp;
    //status 1 for dead, 0 for alive
    int dead;
} enemy;

typedef struct{
    //position
    int x, y;
    //how much gamage
    int dmg;
    //speeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeed (how many +y)
    int speed;
    //if hit something and "dead"
    int hit;
} bullet;

//tworenie globalnie dostępnych: gracza, tablicy wrogów i tablicy pocisków
player ship;
enemy horde[20];
bullet bullets[15];

//procedura do twarzenia linii poziomych
// znak podaj w ascii
// newLine czy ma byc nowa linaia
void line(int znak, int dlugosc,int newLine){
    for (int i = 1; i < dlugosc; i++){printf(displayMode,znak);}
    printf(displayMode,219);
    if(newLine>0)printf("\n");
}

//wyswietla znaki ascii z ich kodem
void ascii(){
    for (int i = 33; i < 300; i++)
    {printf("%d %c \n",i,i);}
};

//menu
//zraca wartosc typu int pozyskana od urzytkownika
char displayMenu(){
    printf(displayMode,219);line(254,windoWidth+1,1);
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("     C INVADERS      ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);line(254,windoWidth+1,1);
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("  1.How to Play      ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("  2.Graj             ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("  3.Ostatni wynik    ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("  4.Wyjdz            ");printf(displayMode,219);printf("\n");
    for (int i = 0; i < 7; i++)
    {
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    }
    printf(displayMode,219);line(254,windoWidth+1,1);
    char input = '0';
    input = getche();
    return input;
}

//procedura do wyswietlania "klatki" ekranu z akcja
void displayGame(int ekran[][windoWidth]){
    system("cls");

    printf(displayMode,219);
    line(254,windoWidth+1,1);

    for (int i = 0; i < windowHeight; i++){
        
        printf(displayMode,219);

            for (int j = 0; j < windoWidth; j++){
                if(ekran[i][j] == 0){printf(displayMode,ekran[i][j]+32);}
                else printf(displayMode,ekran[i][j]);
            }
        
        printf(displayMode,219);
        printf("\n");
    }

    printf(displayMode,219);
    line(254,windoWidth+1,1);

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
            if(hud[i][j]==222 && ship.hp != 0){printf(RED); printf(displayMode, 32);}
            else{
                if(hud[i][j] == 0) printf(displayMode,hud[i][j]+32);
                else printf(displayMode,hud[i][j]);
            }
        }
        if(ship.hp != 0) printf("\033[0m"); //idk how but it doesn't work eather way
        
        printf(displayMode,219);
        printf("\n");
    }

    printf(displayMode,219);
    line(254,windoWidth+1,1);

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

//ekran game over
void gameOver(const char music[]){
    int zmienna = 0,again = 0;
    
        printf("\033[1;31m");
        
        for (int i = 0; i < windowHeight+3; i++)
        {
            //printf("%2d",i);
            if(i!=8){
                for (int j = 0; j < windoWidth+2; j++){
                printf("%c",177);
                

                }
            }else{
                line(177,6,0);
                printf(" GAME OVER ");
                line(177,6,0);
            }
            printf("\n");
        
        }
        printf("\n");
    system("pause");
    system("cls");   
    printf("\033[0m ");

}

// tu testujemy funkcje lub odpalamy kombajn
int main (int argc, char *argv[]) {
    int ekranGry[windowHeight][windoWidth] = {0},
        ekranHud[hudHeight][windoWidth+1] = {0};
    ship.hp = 3;

    if(musicON) system("start \"Music\" /MIN mainTheme.bat");

    displayMenu();
    system("cls");
    displayGame(ekranGry);
    displayHud(ekranHud,ship.hp);
    //ascii();
    

    system("pause");
    killMusic();
    return EXIT_SUCCESS;
}

void killMusic()
{
    if(musicON)
    {
        //system("taskkill /im wmplayer.exe /t /f");
        //system("taskkill /FI \"WindowTitle eq Music*\" /T /F");
        system("start \"Stop\" /MIN killMusic.bat");
        system("taskkill /FI \"WindowTitle eq Stop*\" /F");
    }
}

/*
Logika gry:
    Wyświetla sie ekran
    Gracz klika co chce zrobić (lewo, prawo, strzał)
    if lewo lub prawo: movePlayer(char input) zamienia w ekranie pozycję gracza na puste pole, przesuwa gracza jeśli jest taka możliwość, a następnie wpisuje w ekranie, w tym miejscu statek
    if strzał: PlayerShoot() sprawdza czy może strzelić, tworzy nowy pocisk w tablicy i wpisuje go do ekranu
    Nastepnie moveBullets() pętlą szuka lecących pocisków i przesuwa je tak samo jak gracza, ale w osi Y. Dodatkowo sprawdza czy w nowym miejscu nie ma przeciwnika i jesli jest, to szuka przeciwnika o danej pozycji w horde[] i ded = 1
    Następnie moveAI(int i) pętlą przesuwa każdego przeciwnika w dół, tak samo jak gracza, jeśli i >= 3
    Powrót do początku
*/