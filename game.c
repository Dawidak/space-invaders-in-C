#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

//definy dotyczace dzwieku
// Czy muzyka wlaczona 1 TRUE 0 FALSE
#define musicON 1
#define musicLocation "mainTheme.bat"
#define death "mainDeathSound.bat"

//define kolorki
#define DEFAULTC "\033[0m "
#define RED "\033[1;31m"
#define YELLOW "\033[0;33m"

/*          *\
|   OPTIONS  |
\*          */          

//Tryb wyswietlania
#define displayMode "%2c"
#define enemyPixel '#'
#define shipPixel '&'
#define bulletPixel '|'

//rozmiary obszarow roboczych
//rozmiar ekranu gry -> windowHeight x windowWidth
//rozmiar HUD -> hudHeight x windowWidth
#define windoWidth 11
#define windowHeight 15
#define hudHeight 1
#define maxEnemies windoWidth/2
#define maxBullets 20

typedef struct{
    int x, y;
} point;

typedef struct{
    //position
    point position;
    //hit points
    int hp;
    //score
    int score;
} player;

typedef struct{
    //position
    point position;
    //hit points
    int hp;
    //status 1 for dead, 0 for alive
    int dead;
} enemy;

typedef struct{
    //position
    point position;
    //how much gamage
    int dmg;
    //speeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeed (how many +y)
    int speed;
    //if hit something and "dead"
    int hit;
} bullet;

//funkcje wyświetlające
int displayMenu();
void displayGame(int ekran[][windoWidth]);
void displayHud(int hud[][windoWidth], int health);
void howToPlay();

//funkcje ogólne
void gameWelcome();
void gameOver();
void killMusic();
void line(int znak, int dlugosc, int newLine);
void ascii();
void fillEkran(int c);

//funkcje mechaniki gry
void playGame();
void initializeGame();
void movePlayer(int move);
void shootPlayer();
void moveBullets();
int moveAI();
int isHit(enemy enemy, bullet bullet);
int isHordeDead();
void victory();

//tworenie globalnie dostępnych: gracza, tablicy wrogów i tablicy pocisków
player ship;
enemy horde[maxEnemies];
bullet bullets[maxBullets];

int ekranGry[windowHeight][windoWidth] = {0},
    ekranHud[hudHeight][windoWidth] = {0};

int difficulty = 1;

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
int displayMenu(){
    system("cls");
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
                if(ekran[i][j] == 0){printf(displayMode, ' ');}
                else if(ekran[i][j] == 5) printf(displayMode, enemyPixel);
                else if(ekran[i][j] == 3) printf(displayMode, shipPixel);
                else if(ekran[i][j] == 4) printf(displayMode, bulletPixel);
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
    
    //debug position
    //printf("%d", ship.position.x);
}

//ekran startowy dla gry
void gameWelcome(){
    
    int height = 6;
    char colors[] = "#50C1D9";

    for (int i = 0; i < height; i++)
    {
        system("cls");

        for (int j = 0; j < i; j++){printf(" \n");}
        
        printf("       C INVADERS       ");
        
        if(i%2==0){system("color 70");}
        else{system("color 07");}
        
        for (int j = 0; j < height-i; j++){printf(" \n");}
        
        Sleep(200);
    }
    system("color 07");
    Sleep(2000);
}

//ekran game over
void gameOver(){
    int zmienna = 0,again = 0;

    system("cls");    
    printf("\033[1;31m");
        
    for (int i = 0; i < windowHeight+3; i++)
    {
        //printf("%2d",i);
        if(i!=8){
            for (int j = 0; j < windoWidth+2; j++)
            {
                printf(displayMode,177);
            }
        }
        else
        {
            line(177,4,0);
            printf(" GAME OVER");
            line(177,4,0);
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
    ship.hp = 3;

    if(musicON) system("start \"Music\" /MIN mainTheme.bat");
    gameWelcome();

    while(1){
        int wybor = displayMenu();
        system("cls");
        switch (wybor)
        {
            case '1':
            howToPlay();
            break;
            
            case '2':
            playGame();
            break;

            case '3':
            //scoreboard
            break;

            case '4':
            killMusic();
            return EXIT_SUCCESS;
            break;

            default:
            printf("Wrong number!");
            break;
        }
    }

    //ascii();

    system("pause");
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

void playGame()
{
    char ruch = 0;
    int aiCounter = 0;
    initializeGame();

    while(1)
    {
        displayGame(ekranGry);
        displayHud(ekranHud,ship.hp);

        ruch = getch();
        switch (ruch)
        {
            //lewo
            case 'a':
            movePlayer(-1);
            break;

            //prawo
            case 'd':
            movePlayer(1);
            break;

            //strzał
            case 'w':
            shootPlayer();
            break;

            //na wypadek gdyby gra się nie skończyła można wyjść manualnie
            case 'q':
            return;
            break;

            //ponów zapytanie o klawisz
            default:
            continue;
        }

        moveBullets();
        aiCounter++;
        if(aiCounter > 3)
        {
            aiCounter = 0;
            if(moveAI() == 2) 
                return;
        }

        if(isHordeDead() == 0)
        {
            victory();
            return;
        } 
    }
}

//inicjalizuje początkowe wartości gry
void initializeGame()
{
    fillEkran(0);
    ship.position.x = windoWidth/2;
    ship.position.y = windowHeight-1;
    ekranGry[ship.position.y][ship.position.x] = 3;

    for(int i = 0; i < maxEnemies; i++)
    {
        horde[i].position.x = (windoWidth/(maxEnemies)) * (i+1) - 1;
        horde[i].position.y = 2;
        horde[i].hp = 1 * difficulty;
        horde[i].dead = 0;

        ekranGry[horde[i].position.y][horde[i].position.x] = 5;
    }

    for(int i = 0; i < maxBullets; i++)
    {
        bullets[i].position.x = 0;
        bullets[i].position.y = 0;
        bullets[i].dmg = 1;
        bullets[i].speed = 1;
        bullets[i].hit = 1;
    }
}

//przesuwa gracza w lewo lub prawo
void movePlayer(int move)
{
    if(ship.position.x == 0 && move > 0)
    {
        ekranGry[ship.position.y][ship.position.x] = 0;
        ship.position.x += move;
        ekranGry[ship.position.y][ship.position.x] = 3;
    }
    else if(ship.position.x == windoWidth-1 && move < 0)
    {
        ekranGry[ship.position.y][ship.position.x] = 0;
        ship.position.x += move;
        ekranGry[ship.position.y][ship.position.x] = 3;
    }
    else if(ship.position.x > 0 && ship.position.x < windoWidth-1)
    {
        ekranGry[ship.position.y][ship.position.x] = 0;
        ship.position.x += move;
        ekranGry[ship.position.y][ship.position.x] = 3;
    }
}

//tworzy pocisk przed graczem
void shootPlayer()
{
    for (int i = 0; i < maxBullets; i++)
    {
        if(bullets[i].hit == 1)
        {
            bullets[i].position.x = ship.position.x;
            bullets[i].position.y = ship.position.y-1;
            bullets[i].hit = 0;

            ekranGry[bullets[i].position.y][bullets[i].position.x] = 4;
            break;
        }
    }
    
}

//przesuwa pociski i pokonuje wrogów
void moveBullets()
{
    for (int i = 0; i < maxBullets; i++)
    {
        if(bullets[i].hit == 0)
        {
            if(bullets[i].position.y == 0) 
            {
                ekranGry[bullets[i].position.y][bullets[i].position.x] = 0;
                bullets[i].hit = 1;
                continue;
            }

            ekranGry[bullets[i].position.y][bullets[i].position.x] = 0;
            bullets[i].position.y -= bullets[i].speed;
            ekranGry[bullets[i].position.y][bullets[i].position.x] = 4;
            for (int j = 0; j < maxEnemies; j++)
            {
                if(isHit(horde[j], bullets[i]) == 1)
                {
                    horde[j].hp -= bullets[i].dmg;
                    if(horde[j].hp <= 0) horde[j].dead = 1;
                    bullets[i].hit = 1;
                    break;
                }
            }
            
        }
        else if(bullets[i].position.y < windowHeight-2)
        {
            ekranGry[bullets[i].position.y][bullets[i].position.x] = 0;
        }
    }
    
}

//przesuwa wrogów
int moveAI()
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if(horde[i].dead == 0)
        {
            ekranGry[horde[i].position.y][horde[i].position.x] = 0;
            horde[i].position.y += 1;
            ekranGry[horde[i].position.y][horde[i].position.x] = 5;

            if(horde[i].position.y >= windowHeight-1)
            {
                gameOver();
                return 2;
            }
        }
    }
}

//sprawdza czy wróg i pocisk są w tym samym miejscu i zwraca 1 jeśli tak
int isHit(enemy enemy, bullet bullet)
{
    if(enemy.position.x == bullet.position.x && enemy.position.y == bullet.position.y) return 1;
    else return 0; 
}

//wypełnia ekran podana cyfrą
void fillEkran(int c)
{
    for (int i = 0; i < windowHeight; i++)
    {
        for(int j = 0; j < windoWidth; j++)
        {
            ekranGry[i][j] = c;
        }
    }
}

//sprawdza czy jakiś wrój jeszcze nie jest dead i zwraca ich ilość
int isHordeDead()
{
    int notdead = 0;
    for (int i = 0; i < maxEnemies; i++)
    {
        if(horde[i].dead == 0) notdead++;
    }
    return notdead;
}

//akopiowana funkcja gameOver() z zmienionym napisem
void victory()
{
    //Wygrana (najlepiej jakaś animacja)
    int zmienna = 0,again = 0;

    system("cls");    
    printf(YELLOW);
        
    for (int i = 0; i < windowHeight+3; i++)
    {
        //printf("%2d",i);
        if(i!=8){
            for (int j = 0; j < windoWidth+2; j++)
            {
                printf(displayMode,177);
            }
        }
        else
        {
            line(177,4,0);
            printf(" VICTORY  ");
            line(177,4,0);
        }
        printf("\n");    
    }
    printf("\n");
    system("pause");
    system("cls");   
    printf(DEFAULTC);
}

void howToPlay(){
    system("cls");
    printf(displayMode,219);line(254,windoWidth+1,1);
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("    How To Play      ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);line(254,windoWidth+1,1);
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("        %c - A        ",273);printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("        %c - D        ",272);printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("     Shoot - W       ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    for (int i = 0; i < 7; i++)
    {
        printf(displayMode,219);printf("                     ");printf(displayMode,219);printf("\n");
    }
    printf(displayMode,219);line(254,windoWidth+1,1);
    char input = '0';
    input = getche();
    //return input;
}
