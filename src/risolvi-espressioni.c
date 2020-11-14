#include <stdio.h>
#include "interpreta.h"
#include "espressione.h"
#include "calcola.h"

int main(void) {
    Elemento *InizioExpr, *FineExpr, *Iter;

    if (Interpreta(stdin, &InizioExpr, &FineExpr)) {
        return 1;
    }
    
    printf("%.2f\n", Calcola(InizioExpr, FineExpr));

    return 0;
}