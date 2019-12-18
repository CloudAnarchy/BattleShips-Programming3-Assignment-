#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "misc_functions.h"

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 10
#define N_UNIQUE_SHIPS 4
#define N_S1 4
#define N_S2 3
#define N_S3 2
#define N_S4 1
#define SHIPS N_S1 + N_S2 + N_S3 + N_S4
#define N_AMMO 10

enum BattleShips {S1 = 1, S2 = 2, S3 = 3, S4 = 4};
enum TileState {Missed = 'X', Empty = ' ', Occupied = 'S', CleanHit = 'O'};

typedef struct Player{
    unsigned short playerNum;
    int totalHp;
    int ships[SHIPS][N_S4];
    int ourTiles[BOARD_WIDTH][BOARD_HEIGHT];
    int enemyTiles[BOARD_WIDTH][BOARD_HEIGHT];
}Player;

typedef struct Game{
    Player* player1;
    Player* player2;
}Game;

void printBoards(Player* player){


    // Print player info
    printfColored(GREEN, "\n\nPlayer ");
    printfColored(ORANGE, "%d\n", player->playerNum);
    printfColored(WHITE, "HP: ");
    printfColored(GREEN, "%d\n", player->totalHp);
    printfColored(WHITE, "Ammo: ");
    printfColored(ORANGE, "%d\n\n", N_AMMO);
    ////////////////////////////////////////

    printfColored(BG_CYAN,"This is your board:\n");

    // Print player board
    printf(" ");
    for(int j = 0; j < BOARD_HEIGHT; j++)
        printfColored(ORANGE, "   %d   ", j);
    printf("\n");
    for(int i = 0; i < BOARD_WIDTH; i++){
        printfColored(ORANGE, "%d", i);
        for(int j = 0; j < BOARD_HEIGHT; j++){
            printfColored(WHITE, " [ ");
            player->ourTiles[i][j] == Occupied ? MAGENTA : WHITE;
            putchar(player->ourTiles[i][j]);
            printfColored(WHITE, " ] ");
        }
        printf("\n");
    }
    printf("\n\n");

    // Print ENEMY player board
    printfColored(BG_RED, "Enemy Territory:\n");
    printf(" ");
    for(int j = 0; j < BOARD_HEIGHT; j++)
        printfColored(ORANGE, "   %d   ", j);
    printf("\n");
    for(int i = 0; i < BOARD_WIDTH; i++){
        printfColored(ORANGE, "%d", i);
        for(int j = 0; j < BOARD_HEIGHT; j++){
            printfColored(WHITE, " [ ");

            if (player->enemyTiles[i][j] == CleanHit) GREEN;
            else if (player->enemyTiles[i][j] == Missed) RED;
            putchar(player->enemyTiles[i][j]);
            printfColored(WHITE, " ] ");
        }
        printf("\n");
    }

}
#define ARR_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

void initShipsSize(int* shipSize, int nShips, int nSize){
    static int shipCounter = 0;
    static int functionCounter = 0;

    // Basically every time there is a new player reset the shipCounter
    if(functionCounter % N_UNIQUE_SHIPS == 0) shipCounter = 0;

    int oldCounter = shipCounter;
    for(; shipCounter < oldCounter + nShips; shipCounter++)
        shipSize[shipCounter] = nSize;

    functionCounter++;
}

void initShips(Player* player){
    int* shipSize = (int*) malloc(SHIPS * sizeof(int));

    initShipsSize(shipSize, N_S1, S1);
    initShipsSize(shipSize, N_S2, S2);
    initShipsSize(shipSize, N_S3, S3);
    initShipsSize(shipSize, N_S4, S4);

    for(int i = 0; i < SHIPS; i++){
        for(int k = 0; k < shipSize[i]; k++){
            player->ourTiles[i][k] = Occupied;
        }
    }
}

void initializeGame(Game* game){
    game->player1 = (Player*) malloc(sizeof(Player));
    game->player2 = (Player*) malloc(sizeof(Player));
    if(game->player1 == NULL || game->player2 == NULL){
        printf("Malloc failed!\n");
        exit(-3);
    }

    // Initialize players board
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            game->player1->ourTiles[i][j]      = Empty;
            game->player1->enemyTiles[i][j]    = Empty;
            game->player2->ourTiles[i][j]      = Empty;
            game->player2->enemyTiles[i][j]    = Empty;
        }
    }

    // Initialize players HP and their names
    game->player1->totalHp = N_S1 * S1 + N_S2 * S2 + N_S3 * S3 + N_S4 * S4;
    game->player1->playerNum = 1;
    game->player2->playerNum = 2;

    // Initialize players ships
    initShips(game->player1);
    initShips(game->player2);




    free(game->player1);
    free(game->player2);
}

int gameLoop(){

    printfColored(PURPLE, "Game Started!\n");

    Game* game = (Game*) malloc(sizeof(Game));
    initializeGame(game);

    printBoards(game->player1);

    free(game);
}


int main() {
//    int i, j = 0;
//    for(i = 0;i < 10; i++){
//    }
//    printf("first: %d\n", i);
//    j = i;
//
//    for(;i < j + 3; i++){
//        printf("second: %d\n", i);
//    }
//    j = i;
//    printf("last: %d\n", i);
    gameLoop();
    return 0;
}