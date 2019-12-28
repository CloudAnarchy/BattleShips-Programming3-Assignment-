#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "misc_functions.h"
#include "structsEnumsDefines.h"

void printBoards(Player* player){


    // Print player info
    printfColored(GREEN, "\n\nPlayer ");
    printfColored(ORANGE, "%d\n", player->playerNum);
    printfColored(WHITE, "HP: ");
    printfColored(GREEN, "%d\n", player->totalHp);
    printfColored(WHITE, "Ammo: ");
    printfColored(ORANGE, "%d\n\n", player->ammo);
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

}

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
    printfColored(CYAN, "Getting x coordinate: ");
    coords[0] = inputInt(0, 9);

    printfColored(CYAN, "Getting y coordinate: ");
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

    for(int i = 0; i < SHIPS; i++){
        player->shipsSize[i] = shipSize[i];
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
            game->player1->ourTiles[i][j]          = Empty;
            game->player1->enemyTerritory[i][j]    = Empty;
            game->player2->ourTiles[i][j]          = Empty;
            game->player2->enemyTerritory[i][j]    = Empty;
        }
    }

//    game->player1->ourTiles[3][0]    = Occupied;
//    game->player1->ourTiles[3][1]    = Occupied;
//    game->player1->ourTiles[3][2]    = Occupied;
//    game->player1->ourTiles[3][3]    = Occupied;

    // Initialize players HP and their names and ammos
    // Player 1
    game->player1->totalHp = 2;//N_S1 * S1 + N_S2 * S2 + N_S3 * S3 + N_S4 * S4; //N_S1 * S1;
    game->player1->playerNum = 1;
    game->player1->ammo = N_AMMO;
    // Player 2
    game->player2->totalHp = 2;//N_S1 * S1 + N_S2 * S2 + N_S3 * S3 + N_S4 * S4; //N_S1 * S1;
    game->player2->playerNum = 2;
    game->player2->ammo = N_AMMO;

    // Initialize players ships size
    initShips(game->player1);
    initShips(game->player2);
}
//int sendStateToClient(ConnectInfo* connectInfo, int sock){
//    int scores[2];
//    scores[0] = connectInfo->player->totalHp;
//    scores[1] = connectInfo->enemyPlayer->totalHp;
//    send(sock, scores, 2 * sizeof(int), 0);
//
//    //int ep;
//    // If someone lost found who was it if our
//    // player didn't lose the make him a winner
//    if(scores[0] == 0 || scores[1] == 0){
//        // if our player lost return -1 else announce him winner
//        //ep = connectInfo->player->totalHp == 0 ? -1 : 1;
//        return connectInfo->player->totalHp == 0 ? -1 : 1;
//    }else return 0;
//    //return ep;
//}

void playerMove(ConnectInfo* connectInfo, int sock){
    Player* player = connectInfo->player;
    Player* enemyPlayer = connectInfo->enemyPlayer;

    // He will send this info with tcp/ip protocol to the server
    int* coords = (int*) malloc(2 * sizeof(int));
    if(recv(sock, coords, 2 * sizeof(int), 0) < 0){
        perror("Recv failed!\n");
        exit(-5);
    }
    if(player->playerNum == 1){
        printfColored(BG_PURPLE, "Player%d made a move:\n", player->playerNum);
    }else{
        printfColored(BG_LADI, "Player%d made a move:\n", player->playerNum);
    }

    printCoords(coords);
    printfColored(WHITE, "\n");
    int hitSymbol = Destroyed;
    // If there is a Submarine in that tile update the enemy
    // board his hp and our board.
    if(enemyPlayer->ourTiles[coords[0]][coords[1]] == Occupied){
        enemyPlayer->totalHp--;
        enemyPlayer->ourTiles[coords[0]][coords[1]] = Destroyed;
        hitSymbol = CleanHit;
    }
    player->enemyTerritory[coords[0]][coords[1]] = hitSymbol;
    player->ammo--;
}


void addShips(Player* player, int sock){
    // Auto placements
//    for(int i = 0; i < SHIPS; i++){
//        for(int k = 0; k < shipSize[i]; k++){
//            player->ourTiles[i][k] = Occupied;
//        }
//    }
    int* tmpCoords = (int*) malloc(2 * sizeof(int));
    for(int i = 0; i < SHIPS; i++){
        for(int k = 0; k < player->shipsSize[i]; k++){
            if(recv(sock, (int*) tmpCoords, 2 * sizeof(int), 0) < 0){
                perror("Recv failed!\n");
                exit(-5);
            }
            printCoords(tmpCoords);
            player->ourTiles[tmpCoords[0]][tmpCoords[1]] = Occupied;
        }
    }
    printfColored(CYAN, "Player%d placed all his ships!\n", player->playerNum);
}

void* playerHandler(void* data){

    // Get the socket descriptor
    ConnectInfo* connectInfo = (ConnectInfo*) data;
    int sock = *(int*)connectInfo->clientSock;

    int rounds = 0;

    while(connectInfo->player->ammo > 0){

        if(connectInfo->player->totalHp == 0 || connectInfo->enemyPlayer->totalHp == 0){

            if(connectInfo->player->totalHp != 0) connectInfo->player->totalHp = -9185;

            printfColored(CYAN, "\t\t\tSCORES\n");
            printfColored(RED, "P%d[HP] : ", connectInfo->player->playerNum);
            printfColored(GREEN, "%d        ", connectInfo->player->totalHp);
            printfColored(RED, "P%d[HP] : ", connectInfo->enemyPlayer->playerNum);
            printfColored(GREEN, "%d\n\n", connectInfo->enemyPlayer->totalHp);
            // The winning code

            send(sock, connectInfo->player, sizeof(Player), 0);
            break;
        }

        // First send is the initialized player
        send(sock, connectInfo->player, sizeof(Player), 0);

        if(rounds == 0){
            addShips(connectInfo->player, sock);
        }else{
            playerMove(connectInfo, sock);
        }


        rounds++;
    }

    if(connectInfo->player->totalHp == 0){
        printfColored(BG_RED, "Player%d won!\n", connectInfo->enemyPlayer->playerNum);
    }else if(connectInfo->enemyPlayer->totalHp == 0){
        printfColored(BG_GREEN, "Player%d won!\n", connectInfo->player->playerNum);
    }else{
        printfColored(ORANGE, "No one won!\n");
    }

    close(sock);
    pthread_exit(NULL);
}

void* serverConnection(void* game){
    int sock, playerSock, sizeOfStruct;
    struct sockaddr_in server , client;

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);

    if (sock == -1){
        printf("Could not create socket");
        exit(-5);
    }
    puts("Socket created");

    // Bind
    if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("bind failed. Error");
        exit(1);
    }
    printfColored(GREEN, "Bind done\n");

    // Listen to that socket.Changed to 8 to increase the maximum pending.
    listen(sock , 100);

    //Accept and incoming connection
    printfColored(ORANGE, "Waiting for incoming connections...\n");
    sizeOfStruct = sizeof(struct sockaddr_in);

    pthread_t playerThread[2];
    for(int i = 0; i < N_PLAYERS; i++){
        playerSock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&sizeOfStruct);

        ConnectInfo* connectInfo = (ConnectInfo*) malloc(sizeof(ConnectInfo));
        connectInfo->clientSock  = malloc(1);
        *connectInfo->clientSock = playerSock;

        if(i == 0){
            connectInfo->player      = ((Game*) game)->player1;
            connectInfo->enemyPlayer = ((Game*) game)->player2;
        }else{
            connectInfo->player      = ((Game*) game)->player2;
            connectInfo->enemyPlayer = ((Game*) game)->player1;
        }

        printfColored(MAGENTA, "Player%d connected!\n", connectInfo->player->playerNum);
        if(pthread_create(&playerThread[i] , NULL, playerHandler, (void*) connectInfo) < 0){
            perror("could not create thread");
            exit(-4);
        }

        if(playerSock < 0){
            perror("accept failed");
            exit(1);
        }
    }

    for(int i = 0; i < N_PLAYERS; i++){
        //pthread_detach(playerThread[i]);
        pthread_join(playerThread[i], NULL);
    }

    return NULL;
}

void gameConnected(){
    pthread_t server;
    //int args = 0;
    Game* game = (Game*) malloc(sizeof(Game));
    initializeGame(game);

    if(pthread_create(&server, NULL, serverConnection, (void*) game) != 0){
        printfColored(RED, "Failed to create a thread!\n");
        exit(-4);
    }

    if(pthread_join(server, NULL) != 0){
        printfColored(RED, "Failed to join a thread!\n");
        exit(-4);
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
    gameConnected();
    //gameLoop();
    return 0;
}