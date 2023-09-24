#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(){
    
    errno = 0;
    
    FILE *ulaz = fopen("ulaz.txt" , "r");
    if(ulaz == NULL){
        if(errno == ENOENT){
            fprintf(stderr, "Ne postoji fajl\n");
            exit(1);
        }else if(errno == EACCES){
            fprintf(stderr, "Nemam prava pristupa\n");
            exit(1);
        }else{ 
            fprintf(stderr, "Otvaranje fajla nije uspelo\n");
            exit(1);
        }
    }
    
    return 0;
}
