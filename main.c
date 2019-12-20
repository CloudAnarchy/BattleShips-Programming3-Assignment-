#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "misc_functions.h"

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 10
#define N_UNIQUE_SHIPS 4
#define N_S1 2
#define N_S2 3
#define N_S3 2
#define N_S4 1
#define SHIPS N_S1 //+ N_S2 + N_S3 + N_S4
#define N_AMMO 10

enum BattleShips {S1 = 1, S2 = 2, S3 = 3, S4 = 4};
enum TileState {Destroyed = 'X', Missed = 'X', Empty = ' ', Occupied = 'S', CleanHit = 'O'};

typedef struct Player{
    unsigned short playerNum;
    int totalHp;
    unsigned short ammo;
    //int ships[SHIPS][N_S4];
    int ourTiles[BOARD_WIDTH][BOARD_HEIGHT];
    int enemyTerritory[BOARD_WIDTH][BOARD_HEIGHT];
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

    printfColored(BG_CYAN,"This is your board:\t\t\t\t\t\t\t\t\t\t\t\t\t\t ");
    printfColored(BG_RED, "Enemy Territory:\n");

    // Print player board
    printf(" ");
    for(int j = 0; j < BOARD_HEIGHT; j++)
        printfColored(ORANGE, "   %d   ", j);

    // Enemy Territory
    printf("\t\t ");
    for(int j = 0; j < BOARD_HEIGHT; j++)
        printfColored(ORANGE, "   %d   ", j);

    printf("\n");
    for(int i = 0; i < BOARD_WIDTH; i++){
        // Our Tiles
        printfColored(ORANGE, "%d", i);
        for(int j = 0; j < BOARD_HEIGHT; j++){
            printfColored(WHITE, " [ ");
            // If its not Occupied is it Destroyed? if its false for both print set color to WHITE.
            player->ourTiles[i][j] == Occupied  ? MAGENTA : player->ourTiles[i][j] == Destroyed ? RED : WHITE;

            putchar(player->ourTiles[i][j]);
            printfColored(WHITE, " ] ");
        }

        // Enemy Territory
        printfColored(ORANGE, "\t\t%d", i);
        for(int y = 0; y < BOARD_HEIGHT; y++){
            printfColored(WHITE, " [ ");

            if (player->enemyTerritory[i][y] == CleanHit) GREEN;
            else if (player->enemyTerritory[i][y] == Missed) RED;
            putchar(player->enemyTerritory[i][y]);
            printfColored(WHITE, " ] ");
        }

        printf("\n");
    }
    printf("\n\n");

    // Print ENEMY player board
//    printfColored(BG_RED, "Enemy Territory:\n");
//    printf(" ");
//    for(int j = 0; j < BOARD_HEIGHT; j++)
//        printfColored(ORANGE, "   %d   ", j);
//    printf("\n");
//    for(int i = 0; i < BOARD_WIDTH; i++){
//        printfColored(ORANGE, "%d", i);
//        for(int j = 0; j < BOARD_HEIGHT; j++){
//            printfColored(WHITE, " [ ");
//
//            if (player->enemyTerritory[i][j] == CleanHit) GREEN;
//            else if (player->enemyTerritory[i][j] == Missed) RED;
//            putchar(player->enemyTerritory[i][j]);
//            printfColored(WHITE, " ] ");
//        }
//        printf("\n");
//    }

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

int* askForCoordinates(int* coords){

    // Ask the player to give coordinates.
    printfColored(CYAN, "Give x coordinate: ");
    coords[0] = inputInt(0, 9);

    printfColored(CYAN, "Give y coordinate: ");
    coords[1] = inputInt(0, 9);

    printf("\n");
    return coords;
}



void initShips(Player* player){
    int* shipSize = (int*) malloc(SHIPS * sizeof(int));

    initShipsSize(shipSize, N_S1, S1);
    initShipsSize(shipSize, N_S2, S2);
    initShipsSize(shipSize, N_S3, S3);
    initShipsSize(shipSize, N_S4, S4);

//    int newShipSize[SHIPS];
//    for(int i = 0; i < SHIPS; i++)
//        newShipSize[i] = shipSize[i];

    // Auto placements
//    for(int i = 0; i < SHIPS; i++){
//        for(int k = 0; k < shipSize[i]; k++){
//            player->ourTiles[i][k] = Occupied;
//        }
//    }

    printfColored(BG_PURPLE, "Player%d\n", player->playerNum);
    int* tmpCoords;
    int* coords = (int*) malloc(2 * sizeof(int));

    for(int i = 0; i < SHIPS; i++){
        for(int k = 0; k < shipSize[i]; k++){
            if(k == 0){
                printf("Ship[%d]", i);
                printf(" of size ");
                printfColored(ORANGE, "%d\n", shipSize[i]);
            }
            int count = 0;
            do{
                if(count > 0) printfColored(RED, "Enter a coordinate that isn't set already!\n");

                tmpCoords = askForCoordinates(coords);
                count++;
            }while(player->ourTiles[tmpCoords[0]][tmpCoords[1]] == Occupied);
            player->ourTiles[tmpCoords[0]][tmpCoords[1]] = Occupied;
        }
    }

    // Produce sigFault
    //free(shipSize);
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
            game->player1->ourTiles[i][j]          = Empty;
            game->player1->enemyTerritory[i][j]    = Empty;
            game->player2->ourTiles[i][j]          = Empty;
            game->player2->enemyTerritory[i][j]    = Empty;
        }
    }

    // Initialize players HP and their names and ammos
    // Player 1
    game->player1->totalHp = N_S1 * S1 + N_S2 * S2 + N_S3 * S3 + N_S4 * S4; //N_S1 * S1;
    game->player1->playerNum = 1;
    game->player1->ammo = N_AMMO;
    // Player 2
    game->player2->totalHp = N_S1 * S1 + N_S2 * S2 + N_S3 * S3 + N_S4 * S4; //N_S1 * S1;
    game->player2->playerNum = 2;
    game->player2->ammo = N_AMMO;


    // Initialize players ships
    initShips(game->player1);
    initShips(game->player2);

}

void playerMove(Player* player, Player* enemyPlayer){
    printBoards(player);
    printfColored(MAGENTA, "Make a move");
    printf(" Player");

    printfColored(GREEN, "%d", player->playerNum);
    printfColored(MAGENTA, " !\n");
    // He will send this info with tcp/ip protocol to the server
    int* coords = (int*) malloc(2 * sizeof(int));
    askForCoordinates(coords);

    // but for the demo now..:
    int hitSymbol = 'X';

    // If there is a Submarine in that tile update the enemy
    // board his hp and our board.
    if(enemyPlayer->ourTiles[coords[0]][coords[1]] == Occupied){
        enemyPlayer->totalHp--;
        enemyPlayer->ourTiles[coords[0]][coords[1]] = 'X';
        hitSymbol = 'O';
    }
    player->enemyTerritory[coords[0]][coords[1]] = hitSymbol;
    player->ammo--;
}

bool declareWinner(Game* game){
    Player* player = NULL;

    // Player 1 wins!
    if(game->player2->totalHp == 0)
        player = game->player1;
    // Player 2 wins!
    if(game->player1->totalHp == 0)
        player = game->player2;


    if(player == NULL) return false;
    // If we get to this point it means either player1 is "dead" or player2
    printfColored(WHITE, "          ___\n"
           "        ,\"---\".\n"
           "        :     ;\n"
           "         `-.-'\n"
           "          | |\n"
           "          | |\n"
           "          | |\n"
           "       _.-\\_/-._\n"
           "    _ / |     | \\ _\n"
           "   / /   `---'   \\ \\\n"
           "  /  `-----------'  \\\n"
           " /,-\"\"-.       ,-\"\"-.\\\n"
           "( i-..-i       i-..-i )\n"
           "|`|    |-------|    |'|\n"
           "\\ `-..-'  ,=.  `-..-'/\n"
           " `--------|=|-------'\n"
           "          | |\n"
           "          \\ \\\n"
           "           ) )\n"
           );
    printfColored(GREEN, "Player%d WON!!\n\n", player->playerNum);
}

int gameLoop(){

    printfColored(PURPLE, "Game Started!\n");

    Game* game = (Game*) malloc(sizeof(Game));
    initializeGame(game);

    int bothPlayersAmmo = N_AMMO * 2;
    int roundsCounter = 1;
    while(bothPlayersAmmo > 0){

        // Every round the next playerMoves.
        if(roundsCounter % 2 != 0 && game->player1->ammo > 0){
            playerMove(game->player1, game->player2);
        }else if (roundsCounter % 2 == 0 && game->player2->ammo > 0){
            playerMove(game->player2, game->player1);
        }

        // Check if there is a winner
        if(declareWinner(game))
            break;

        bothPlayersAmmo--;
        roundsCounter++;
    }

    free(game->player1);
    free(game->player2);
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