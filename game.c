#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

//definy dotyczace dzwieku
// Czy muzyka wlaczona 1 TRUE 0 FALSE
#define musicON 1
#define musicLocation "mainTheme.bat"
#define death "mainDeathSound.bat"

//define kolorki
#define DEFAULTC "\033[0m"
#define RED "\033[1;31m"
#define YELLOW "\033[0;33m"

/*          *\
|   OPTIONS  |
\*          */          

//Tryb wyswietlania
#define displayMode "%2c"
#define enemyPixel '#'
#define shipPixel '&'
#define bulletPixel '*'
#define explosionPixel '@'

//rozmiary obszarow roboczych
//rozmiar ekranu gry -> windowHeight x windowWidth
//rozmiar HUD -> hudHeight x windowWidth
#define windoWidth 11
#define windowHeight 15
#define hudHeight 1
#define maxEnemies windoWidth * 2
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

typedef struct{
    char name[15];
    int points;
} score;

//funkcje wyświetlające
int displayMenu();
void displayGame(int ekran[][windoWidth]);
void displayHud(int hud[][windoWidth], int health);
void howToPlay();

//funkcje ogólne
void gameWelcome();
void gameOver(int rounds);
void killMusic();
void line(int znak, int dlugosc, int newLine);
void ascii();
void fillEkran(int c);
void addScore(int rounds);
void readScores();
void difficultyChange();

//funkcje mechaniki gry
void playGame();
void initializeGame();
void movePlayer(int move);
void shootPlayer();
void moveBullets();
void moveAI();
int isHit(enemy enemy, bullet bullet);
int isHordeDead();
void victory(int rounds);
void spawnEnemy();

//tworenie globalnie dostępnych: gracza, tablicy wrogów i tablicy pocisków
player ship;
enemy horde[maxEnemies] = {0};
bullet bullets[maxBullets] = {0};

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
    printf(displayMode,219);printf("  2.Play             ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("  3.Scores           ");printf(displayMode,219);printf("\n");
    printf(displayMode,219);printf("  4.Exit             ");printf(displayMode,219);printf("\n");
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
                else if(ekran[i][j] == 6) printf(displayMode, explosionPixel);
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
void gameOver(int rounds){
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
    printf(DEFAULTC);
    addScore(rounds);
    system("cls");   
    printf("\033[0m ");
}

// tu testujemy funkcje lub odpalamy kombajn
int main (int argc, char *argv[]) {
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
            //difficultyChange();
            playGame();
            break;

            case '3':
            readScores();
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
    int Counter = 0;
    initializeGame();

    displayGame(ekranGry);
    displayHud(ekranHud,ship.hp);

    while(1)
    {
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
        if(Counter%3 == 0)
        {
            moveAI();
        }

        if(Counter%(12/difficulty) == 0)
        {
            spawnEnemy();
        }

        if(isHordeDead() == 0)
        {
            victory(Counter);
            return;
        }
        else if(ship.hp <= 0)
        {
            gameOver(Counter);
            return;
        }

        displayGame(ekranGry);
        displayHud(ekranHud,ship.hp);
        Counter += 1;
    }
}

//inicjalizuje początkowe wartości gry
void initializeGame()
{
    fillEkran(0);
    ship.position.x = windoWidth/2;
    ship.position.y = windowHeight-1;
    ship.hp = 3;
    ekranGry[ship.position.y][ship.position.x] = 3;

    for(int i = 0; i < windoWidth / 2; i++)
    {
        horde[i].position.x = (windoWidth/(windoWidth / 2)) * (i+1) - 1;
        horde[i].position.y = 2;
        horde[i].hp = 1;
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
            bullets[i].position.y = ship.position.y;
            bullets[i].hit = 0;

            //ekranGry[bullets[i].position.y][bullets[i].position.x] = 4;
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

            if(bullets[i].position.y != ship.position.y) 
                ekranGry[bullets[i].position.y][bullets[i].position.x] = 0;
            bullets[i].position.y -= bullets[i].speed;

            for (int j = 0; j < maxEnemies; j++)
            {
                if(isHit(horde[j], bullets[i]) == 1)
                {
                    horde[j].hp -= bullets[i].dmg;
                    if(horde[j].hp <= 0) horde[j].dead = 1;
                    bullets[i].hit = 2;
                    ekranGry[bullets[i].position.y][bullets[i].position.x] = 6;
                    break;
                }
            }
            if(bullets[i].hit == 0)
                ekranGry[bullets[i].position.y][bullets[i].position.x] = 4;
        }
        else if(bullets[i].hit == 2)
        {
            ekranGry[bullets[i].position.y][bullets[i].position.x] = 0;
            bullets[i].hit = 1;
        }
    }
}

//przesuwa wrogów
void moveAI()
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if(horde[i].dead == 0)
        {
            ekranGry[horde[i].position.y][horde[i].position.x] = 0;
            horde[i].position.y += 1;

            //TODO: Losowa szansa na uniknięcie trafienia

            //sprawdzanie czy czasem przeciwnik nie wleciał w pocisk
            for (int j = 0; j < maxBullets; j++)
            {
                if(isHit(horde[i], bullets[j]) == 1)
                {
                    horde[i].hp -= bullets[j].dmg;
                    if(horde[i].hp <= 0) horde[i].dead = 1;
                    bullets[j].hit = 2;
                    ekranGry[bullets[j].position.y][bullets[j].position.x] = 6;
                    break;
                }
            }

            if(horde[i].dead != 1)
            {
                if(horde[i].position.y >= ship.position.y)
                {
                    ship.hp -= 1;
                    horde[i].dead = 1;
                    horde[i].position.y = 0;
                }
                else
                {
                    ekranGry[horde[i].position.y][horde[i].position.x] = 5;
                }
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
void victory(int rounds)
{
    //Wygrana (najlepiej jakaś animacja)
    int zmienna = 0, again = 0;

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
    printf(DEFAULTC);
    addScore(rounds);
    system("cls");   
    printf(DEFAULTC);
}

//dodaje przeciwnika na początku planszy
void spawnEnemy()
{
    srand(time(NULL));
    for(int i = 0; i < maxEnemies; i++)
    {
        if(horde[i].dead == 1)
        {
            horde[i].position.x = rand()%windoWidth;
            horde[i].position.y = 2;
            horde[i].hp = 1 * difficulty;
            horde[i].dead = 0;

            ekranGry[horde[i].position.y][horde[i].position.x] = 5;
            break;
        }
    }
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

void addScore(int rounds)
{
    char name[15] = {0};
    char wybor;
    score bscore[1] = {"NUNAME", 0};
    do{
        printf("Type your name (max 10 characters): ");
        scanf("%s", name);
        if(strlen(name) < 10)
        {
            printf("Is %s your name? [Y / N]\n", name);
            fflush(stdin);
            wybor = getchar();

            if(wybor == 'Y') break;
            else if(wybor == 'N') continue;
            else printf("Try again.\n");
        }
    }while(1);

    strcpy(bscore[0].name, name);
    if(ship.hp > 0) bscore[0].points = rounds * ship.hp;
    else bscore[0].points = rounds;

    FILE *file = fopen("scores.bin", "a");
    if(file == NULL)
    {
        printf(RED);
        printf("ERROR Can't save score\n");
        printf(DEFAULTC);
        system("pause");
    }
    else
    {
        fwrite(bscore, sizeof(score), 1, file);
        fclose(file);
    }
}

void readScores()
{
    score bscore[1];
    score *scores;
    int number = 0;

    scores = (score*) calloc(1, sizeof(score));

    FILE *file = fopen("scores.bin", "r");
    if(file == NULL)
    {
        printf("There are no previous scores or can't open file\n");
        system("pause");
        return;
    }

    while(fread(bscore, sizeof(bscore), 1, file) == 1){
        number += 1;

        scores = (score*) realloc(scores, sizeof(score) * number);
        strcpy(scores[number-1].name, bscore[0].name);
        scores[number-1].points = bscore[0].points;
    }

    //TODO: Sortowanie

    printf(displayMode,219);line(254, 12, 1);
    for(int i = 0; i < number; i++)
    {
        printf(displayMode,219);
        printf(" %11s | %5d  ", scores[i].name, scores[i].points);
        printf(displayMode,219);
        printf("\n");
    }
    printf(displayMode,219);line(254, 12, 1);

    system("pause");
    fclose(file);
    free(scores);
}

void difficultyChange()
{
    printf("Choose dificulty\n [1]Easy\n [2]Normal\n [3]Hard\n [4]Endless\n");
    char wybor = getch();

    do
    {
        switch (wybor)
        {
            case '1':
            difficulty = 1;
            return;
            break;
            
            case '2':
            difficulty = 2;
            return;
            break;

            case '3':
            difficulty = 3;
            return;
            break;

            case '4':
            difficulty = 4;
            return;
            break;

            default:
            printf("Wrong number!");
            break;
        }
    } while (1);
}