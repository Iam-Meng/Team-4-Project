#include "tigr.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <bird.h>
#include <obstacle.h>

// Time you've survived in the game
typedef struct{
    int hour;
    int min;
    int sec;
} Time;

// Score structure to keep track of the player's score and highest score
typedef struct{
    int score;
    int highestScore;
} Scores;

int main() {

    Tigr* screen = tigrWindow(680, 720, "Flappy Bird", 0);
    

    while (!tigrClosed(screen)) {

        

        tigrUpdate(screen);
    }

    tigrFree(screen);
    return 0;
}
