#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define MAGENTA  printf("\x1b[31m")
#define CYAN     printf("\x1b[36m")
#define RED      printf("\x1b[35m")
#define GREEN    printf("\x1b[32m")
#define ORANGE   printf("\x1b[33m")
#define WHITE    printf("\x1b[0m")
#define PURPLE   printf("\x1b[34m")

#define BG_LADI  printf("\x1b[40m")
#define BG_PINK   printf("\x1b[41m")
#define BG_ORAGNE   printf("\x1b[43m")
#define BG_GREEN   printf("\x1b[42m")
#define BG_PURPLE  printf("\x1b[44m")
#define BG_RED   printf("\x1b[45m")
#define BG_CYAN   printf("\x1b[46m")
#define BG_GRAY   printf("\x1b[47m")
#define BG_WHITE   printf("\x1b[7m")

#define RESET    printf("\x1b[0m")


//#ifndef ASKHSH2_PROG3_ASK2_H
//#define ASKHSH2_PROG3_ASK2_H
//#endif //ASKHSH2_PROG3_ASK2_H

int printfColored(int color, char* str, ...){

    // arg color here is the return of the printf in the defines.

    va_list arg;
    va_start(arg, str);
    int done = vfprintf(stdout, str, arg);
    va_end(arg);


    RESET; // Reset the color.
    return done;
}

void getSubString(char *target, const char *source, int from, int to){
    int i, j = 0;
    for(i = from, j = 0; i <= to; i++, j++){
        target[j] = source[i];
    }
    // assign NULL at the end of string
    target[j] = '\0';
}


char** separate_str(char* str, const char* delim){
    char **args = malloc(64 * sizeof(char*));
    char *token;
    int position = 0;

    token = strtok(str, delim);
    while(token != NULL){
        args[position] = token;
        position++;
        token = strtok(NULL, delim);
    }

    args[position] = NULL;

    return args;
}

void remove_spaces_from(char* ptr, int pos){
    int j = 0;
    for(int i = pos; ptr[i]; i++)
        if(ptr[i] != ' ')
            ptr[j++] = ptr[i];
    ptr[j] = '\0';
}
