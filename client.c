
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "misc_functions.h"
#include "structsEnumsDefines.h"

#define WINNER 1
#define LOSER -1

void printBoards(Player* player);

//void printBoards(Player* player){
//
//    // Print player info
//    printfColored(GREEN, "\n\nPlayer ");
//    printfColored(ORANGE, "%d\n", player->playerNum);
//    printfColored(WHITE, "HP: ");
//    printfColored(GREEN, "%d\n", player->totalHp);
//    printfColored(WHITE, "Ammo: ");
//    printfColored(ORANGE, "%d\n\n", player->ammo);
//    ////////////////////////////////////////
//
//    printfColored(BG_CYAN,"This is your board:\t\t\t\t\t\t\t\t\t\t\t\t\t\t ");
//    printfColored(BG_RED, "Enemy Territory:\n");
//
//    // Print player board
//    printf(" ");
//    for(int j = 0; j < BOARD_HEIGHT; j++)
//        printfColored(ORANGE, "   %d   ", j);
//
//    // Enemy Territory
//    printf("\t\t ");
//    for(int j = 0; j < BOARD_HEIGHT; j++)
//        printfColored(ORANGE, "   %d   ", j);
//
//    printf("\n");
//    for(int i = 0; i < BOARD_WIDTH; i++){
//        // Our Tiles
//        printfColored(ORANGE, "%d", i);
//        for(int j = 0; j < BOARD_HEIGHT; j++){
//            printfColored(WHITE, " [ ");
//            // If its not Occupied is it Destroyed? if its false for both print set color to WHITE.
//            player->ourTiles[i][j] == Occupied  ? MAGENTA : player->ourTiles[i][j] == Destroyed ? RED : WHITE;
//
//            putchar(player->ourTiles[i][j]);
//            printfColored(WHITE, " ] ");
//        }
//
//        // Enemy Territory
//        printfColored(ORANGE, "\t\t%d", i);
//        for(int y = 0; y < BOARD_HEIGHT; y++){
//            printfColored(WHITE, " [ ");
//
//            if (player->enemyTerritory[i][y] == CleanHit) GREEN;
//            else if (player->enemyTerritory[i][y] == Missed) RED;
//            putchar(player->enemyTerritory[i][y]);
//            printfColored(WHITE, " ] ");
//        }
//
//        printf("\n");
//    }
//    printf("\n\n");
//
//}

void decryptMsg(Player* player, const char* msg){

    int n = 0, hp = 0, ammo = N_AMMO;
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            player->ourTiles[i][j] = msg[n++];
            hp = player->ourTiles[i][j] == Occupied ? ++hp : hp;
        }
    }

    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            player->enemyTerritory[i][j] = msg[n++];
            ammo = player->enemyTerritory[i][j] == Empty ? --ammo : ammo;
        }
    }

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
void initShips(Player* player, int sock){

    printfColored(BG_PURPLE, "Player%d\n", player->playerNum);
    int* tmpCoords;
    int* coords = (int*) malloc(2 * sizeof(int));

    for(int i = 0; i < SHIPS; i++){
        for(int k = 0; k < player->shipsSize[i]; k++){
            if(k == 0){
                printf("Ship[%d]", i);
                printf(" of size ");
                printfColored(ORANGE, "%d\n", player->shipsSize[i]);
            }
            int count = 0;
            do{
                if(count > 0) printfColored(RED, "Enter a coordinate that isn't set already!\n");

                tmpCoords = askForCoordinates(coords);
                count++;
            }while(player->ourTiles[tmpCoords[0]][tmpCoords[1]] == Occupied);
            player->ourTiles[tmpCoords[0]][tmpCoords[1]] = Occupied;

            send(sock, tmpCoords, 2 * sizeof(int), 0);
        }
    }

}
int waitForServer(Player* player, int sock){
    int scores[2];

    if(recv(sock, scores, 2 * sizeof(int), 0) < 0){
        perror("Recv failed!\n");
        exit(-5);
    }

    int num = player->playerNum - 1;
    int returnNum = 0;

    // If someone lost found who was it if our
    // player didn't lose the make him a winner
    if(scores[0] == 0 || scores[1] == 0){
        // if our player lost return -1 else announce him winner
        return scores[num] == 0 ? LOSER : WINNER;
    }else return 0;

    return returnNum;

}
void youLost(int sock){
    printfColored(RED, "YOU LOST!!\n");
    close(sock);
}
void youWon(int sock){
    printfColored(GREEN, "YOU WON!!\n");
    close(sock);
}
void playerMove(Player* player, int sock){
    printBoards(player);

    switch(waitForServer(player, sock)){
        case 1:
            youLost(sock);
            return;
        case -1:
            youWon(sock);
            return;
        default:;
    }

    printfColored(MAGENTA, "Make a move");
    printf(" Player");

    printfColored(GREEN, "%d", player->playerNum);
    printfColored(MAGENTA, " !\n");
    // He will send this info with tcp/ip protocol to the server
    int* coords = (int*) malloc(2 * sizeof(int));
    askForCoordinates(coords);

    send(sock, coords, 2 * sizeof(int), 0);
}

int main(int argc , char *argv[]){

    int sock;
    struct sockaddr_in server;
    char server_reply[256];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);

    if (sock == -1){
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    int counter = 0;

    Player* player;
    //keep communicating with server
    while(counter < N_AMMO){

        //Receive player info from the server
        if(recv(sock , server_reply , sizeof(Player) , 0) < 0){
            puts("recv failed");
            break;
        }

        player = (Player*) server_reply;

        if(counter == 0)  initShips(player, sock);
        else              playerMove(player, sock);
        counter++;
    }

    close(sock);
    return 0;
}

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
    printfColored(BG_RED, "Enemy Territory:");

    // Print player board
    printf("\n ");
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