#define BOARD_WIDTH  10
#define BOARD_HEIGHT 10
#define N_UNIQUE_SHIPS 4
#define N_S1 2
#define N_S2 3
#define N_S3 2
#define N_S4 1
#define N_PLAYERS 2
#define SHIPS N_S1 //+ N_S2 + N_S3 + N_S4
#define N_AMMO 22
#define PORT 8888

#define ARR_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

enum BattleShips {S1 = 1, S2 = 2, S3 = 3, S4 = 4};
enum TileState {Destroyed = 'X', Missed = 'X', Empty = ' ', Occupied = 'S', CleanHit = 'O'};


typedef struct Player{
    unsigned short playerNum;
    int totalHp;
    unsigned short ammo;
    int shipsSize[SHIPS];
    int ourTiles[BOARD_WIDTH][BOARD_HEIGHT];
    int enemyTerritory[BOARD_WIDTH][BOARD_HEIGHT];
}Player;

typedef struct Game{
    Player* player1;
    Player* player2;
}Game;

typedef struct ConnectInfo{
    int* clientSock;
    Player* player;
    Player* enemyPlayer;
}ConnectInfo;

//void strreverse(char* begin, char* end) {
//
//    char aux;
//
//    while(end > begin)
//        aux = *end, *end-- = *begin, *begin++ = aux;
//
//}
//
//void itoa(int value, char* str, int base) {
//
//    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
//
//    char* wstr = str;
//
//    int sign;
//
//    div_t res;
//
//    // Validate base
//    if (base < 2 || base > 35){ *wstr = '\0'; return; }
//
//    // Take care of sign
//    if ((sign = value) < 0) value = -value;
//
//
//    // Conversion. Number is reversed.
//    do {
//        res = div(value,base);
//        *wstr++ = num[res.rem];
//    }while(value = res.quot);
//
//    if(sign < 0) *wstr++ = '-';
//
//    *wstr = '\0';
//
//    // Reverse string
//    strreverse(str, wstr-1);
//
//}
