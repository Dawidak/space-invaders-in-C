//plik testowy
#include <stdio.h>
#include <stdlib.h>
#define hudHeight 1
#define windowWidth 21
#define windowHeight 15
#define displayMode "%3d"
#define music "space_invaders_loop.mp3"
#define death "deadth.wav"

typedef struct player{
    int positionX,
        positionY,
        hitPoints,
        score;
}player;

typedef struct enemy{
    int positionX,
        positionY,
        hitPoints,
        status,
        type;
}enemy;

typedef struct bullet
{
    int positionX,
        positionY,
        damage,
        speed;
}bullet;

void musicPlay(const int sciezka){}

void displayGame          (int ekran[][windowWidth]){}

void displayMenu          (int ekran[][windowWidth]){}

void generateGameEkran    (int ekran[][windowWidth]){}

void generateWelcomeEkran (int ekran[][windowWidth]){}

void generateHowEkran     (int ekran[][windowWidth]){}


int main (int argc, char *argv[]) {
    return EXIT_SUCCESS;
}