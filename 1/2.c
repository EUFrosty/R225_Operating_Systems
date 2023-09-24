#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define checkError(expr, usrMsg)\
 do {\
        if(!(expr)){\
            fprintf(stderr, "%s", usrMsg);\
            perror(usrMsg);\
            exit(EXIT_FAILURE);\
        }\
    }while (0)\

int main(){
    
    errno = 0;
    
    FILE *ulaz = fopen("ulaz.txt" , "r");
    
    checkError(ulaz != NULL, "Nije uspelo otvaranje\n");
    
    return 0;
}
