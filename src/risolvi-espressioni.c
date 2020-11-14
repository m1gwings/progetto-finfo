#include <stdio.h>
#include "interpreta.h"
#include "espressione.h"
#include "calcola.h"
#include <string.h>

int main(int argc, char *argv[]) {
    Elemento *InizioExpr, *FineExpr, *Iter;
    FILE* FlussoIn;

    if (argc > 2) {
        if (strncmp(argv[1], "-f", 2) == 0) {
            FlussoIn = fopen(argv[2], "r");

            if (FlussoIn == NULL) {
                fprintf(stderr, "\033[0;31mimpossibile aprire il file %s\033[0m\n", argv[2]);
                return 1;
            }
        } else {
            fprintf(stderr, "\033[0;31ml'unica opzione disponibile è -f\033[0m\n");
            return 1;
        }
    } else {
        printf("Inserisci l'espressione da valutare\n[Termina con \\n]\n");
        FlussoIn = stdin;
    }

    if (Interpreta(FlussoIn, &InizioExpr, &FineExpr)) {
        return 1;
    }
    
    printf("%.2f\n", Calcola(InizioExpr, FineExpr));

    return 0;
}