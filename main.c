#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "misc_functions.h"

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 10
#define N_S1 4
#define N_S2 3
#define N_S3 2
#define N_S4 1
#define SHIPS N_S1 + N_S2 + N_S3 + N_S4
#define N_AMMO 10

enum BattleShips {S1 = 1, S2 = 2, S3 = 3, S4 = 4};
enum TileState {Missed = 'X', Empty = ' ', Occupied = 'S', CleanHit = 'O'};


typedef struct Cordinates{
    int x;
    int y;
}Coordinates;

typedef struct Player{
    int totalHp;
    int** ships;
    int ourTiles[BOARD_WIDTH][BOARD_HEIGHT];
    int enemyTiles[BOARD_WIDTH][BOARD_HEIGHT];
}Player;

typedef struct Game{
    Player* player1;
    Player* player2;
}Game;

void printBoards(Game* game, int playerNum){
    Player* player;
    if(playerNum == 1) player = game->player1;
    else if(playerNum == 2) player = game->player2;
    else return;

    // Print player info
    printfColored(GREEN, "\n\nPlayer ");
    printfColored(ORANGE, "%d\n", playerNum);
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
//player->shipsLength[0] = Aircraft;
//player->shipsLength[1] = Battleship;
//player->shipsLength[2] = Submarine;
//player->shipsLength[3] = Cruiser;
//player->shipsLength[4] = Destroyer;
void initShips(Player* player){
    player->ships = (int**) malloc(SHIPS * sizeof(int*));
    int shipSize[SHIPS];

    for(int i = SHIPS - 1; i >= 0; i--){
        shipSize[i] = i < 3 ? i + 1 : i;
        if(i == 0) shipSize[i] = 5;
        //printf("shipSize: %d\n", shipSize[i]);
        player->ships[i] = (int*) malloc(shipSize[i] * sizeof(int));
    }

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

    // Initialize players HP
    game->player1->totalHp = N_S1 * S1 + N_S2 * S2 + N_S3 * S3 + N_S4 * S4;

    // Initialize players ships
    initShips(game->player1);
    initShips(game->player2);

    printBoards(game, 1);
    free(game->player1->ships);
    free(game->player2->ships);
    free(game->player1);
    free(game->player2);
}

int gameLoop(){

    printfColored(PURPLE, "Game Started!\n");

    Game* game = (Game*) malloc(sizeof(Game));
    initializeGame(game);


    free(game);
}


int main() {

    gameLoop();
    return 0;
}