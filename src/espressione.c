#include "espressione.h"
#include <stdio.h>

void StampaEspressione(Elemento* InizioExpr, Elemento* FineExpr) {
    /* Iteratore utilizzato per scorrere lungo l'espressione */
    Elemento* Iter;

    /* Itera finchè non si è superato l'ultimo elemento, oppure è finita
     * l'espressione completa */
    for (Iter = InizioExpr; Iter != NULL && Iter->Prec != FineExpr; Iter = Iter->Succ) {
        switch (Iter->Tipo) {
        case Op:
            switch (Iter->Op) {
            case Somma:
                printf("+");
                break;
            case Diff:
                printf("-");
                break;
            case Molt:
                printf("*");
                break;
            case Div:
                printf("/");
                break;
            }
            break;
        case Par:
            switch (Iter->Par) {
            case Aperta:
                printf("(");
                break;
            case Chiusa:
                printf(")");
                break;
            }
            break;
        case Val:
            printf("%f", Iter->Val);
            break;
        }
    }
    printf("\n");
}
