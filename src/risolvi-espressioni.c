#include <stdio.h>
#include "interpreta.h"
#include "espressione.h"
#include "calcola.h"

int main(void) {
    Elemento *InizioExpr, *FineExpr, *Iter;
    if (Interpreta(stdin, &InizioExpr, &FineExpr)) {
        return 1;
    }

    /* Iter = InizioExpr;
    while (Iter != NULL) {
        switch (Iter->Tipo) {
        case Op:
            switch (Iter->Op) {
            case Somma:
                printf("-> + ");
                break;
            case Diff:
                printf("-> - ");
                break;
            case Molt:
                printf("-> * ");
                break;
            case Div:
                printf("-> / ");
                break;
            }
            break;
        case Par:
            if (Iter->Par == Aperta) {
                printf("-> ( ");
            } else {
                printf("-> ) ");
            }
            break;
        case Val:
            printf("-> %.2f ", Iter->Val);
            break;
        }
        Iter = Iter->Succ;
    }

    printf("\n");

    Iter = FineExpr;
    while (Iter != NULL) {
        switch (Iter->Tipo) {
        case Op:
            switch (Iter->Op) {
            case Somma:
                printf("<- + ");
                break;
            case Diff:
                printf("<- - ");
                break;
            case Molt:
                printf("<- * ");
                break;
            case Div:
                printf("<- / ");
                break;
            }
            break;
        case Par:
            if (Iter->Par == Aperta) {
                printf("<- ( ");
            } else {
                printf("<- ) ");
            }
            break;
        case Val:
            printf("<- %.2f ", Iter->Val);
            break;
        }
        Iter = Iter->Prec;
    }

    printf("\n"); */
    printf("%.2f\n", Calcola(InizioExpr, FineExpr));

    return 0;
}