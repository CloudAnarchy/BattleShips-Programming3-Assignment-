#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 10
#define N_S1 4
#define N_S2 3
#define N_S3 2
#define N_S4 1
#define SHIPS N_S1 + N_S2 + N_S3 + N_S4
#define N_AMMO 10

enum BattleShips {S1 = 1, S2 = 2, S3 = 3, S4 = 4};
enum TileState {Destroyed = 'X', Empty = ' ', Occupied = 'S', CleanHit = 'O'};
//enum GameState { };

typedef struct Cordinates{
    int x;
    int y;
}Coordinates;

typedef struct Player{
    int totalHp;
    int** ships;
    int tiles[BOARD_WIDTH][BOARD_HEIGHT];
}Player;

typedef struct Game{
    Player* player1;
    Player* player2;
}Game;

void printBoard(Game* game){
    for(int i = 0; i < BOARD_WIDTH; i++){
        for(int j = 0; j < BOARD_HEIGHT; j++){
             printf(" [%c] ", game->player1->tiles[i][j]);
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

        printf("shipSize: %d\n", shipSize[i]);
        player->ships[i] = (int*) malloc(shipSize[i] * sizeof(int));
    }


    int rNum = 0;
    for(int i = 0; i < SHIPS; i++){
        int limit = shipSize[i] + rNum;
        //printf("len: %d rNum: %d limit: %d ", limit - rNum, rNum, limit);
        for(int k = 0; k < shipSize[i]; k++){
            player->tiles[i+rNum][k+rNum] = Occupied;
        }
        //printf("ship: %d\n", i);
        rNum += 1;
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
            game->player1->tiles[i][j] = Empty;
            game->player2->tiles[i][j] = Empty;
        }
    }

    // Initialize players HP
    game->player1->totalHp = N_S1 * S1 + N_S2 * S2 + N_S3 * S3 + N_S4 * S4;
    printf("totalHP: %d\n", game->player1->totalHp);

    // Initialize players ships
    initShips(game->player1);
    initShips(game->player2);
}

int gameLoop(){
    printf("Game Started!\n");
    Game* game = (Game*) malloc(sizeof(Game));

    initializeGame(game);
    printBoard(game);
}


int main() {

    gameLoop();
    return 0;
}