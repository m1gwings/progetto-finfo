#include "calcola.h"
#include <stdio.h>

double Calcola(Elemento* InizioExpr, Elemento* FineExpr) {
    /* Rappresenta la priorità delle operazioni, leggendo da destra verso sinistra,
     * incrementa di 2 con le parentesi chiuse e decrementa di 2 con quelle aperte
     *
     * L'incremento di 2 è dovuto al fatto che altrimenti il + dentro la parentesi avrebbe 
     * la stessa priorità del * all'esterno e quindi ad esempio in
     * 2*(1+3) il programma cercherebbe di eseguire prima il + del * */
    int Prior;

    /* Priorità minima nell'espressione */
    int MinPrior;
    /* Elemento con priorità minima nell'espressione */
    Elemento* MinPriorElem;
    /* Itera sull'espressione */
    Elemento* Iter;

    /* Valore del blocco precedente e di quello successivo all'operazione con
     * minor priorità */
    double BloccoPrec, BloccoSucc;

    /* Elimina le parentesi aperte all'inizio e quelle chiuse alla fine finchè ci sono
     * esempio: (4 + 5) diventa 4 + 5 */
    while ( InizioExpr->Tipo == Par && InizioExpr->Par == Aperta &&
            FineExpr->Tipo == Par && FineExpr->Par == Chiusa) {
        InizioExpr = InizioExpr->Succ;
        FineExpr = FineExpr->Prec;
    }

    /* Se la sottoespressione inizia o finisce con un'operazione c'è stato un errore */
    if (InizioExpr->Tipo == Op || FineExpr->Tipo == Op) {
        fprintf(stderr, "\033[0;31mtrovata sottoespressione che inizia o finisce con una operazione\033[0m\n");
        return 0;
    }

    Prior = 0;
    MinPriorElem = NULL;

    /* Cerca l'elemento con priorità minore,
     * la lettura parte da destra perchè a parità di Prior gli elementi con
     * meno priorità sono quelli più a destra */
    Iter = FineExpr;
    while (Iter != InizioExpr) {
        if (Iter->Tipo == Par) {
            if (Iter->Par == Aperta) {
                Prior -= 2;
            } else {
                Prior += 2;
            }
        } else if (Iter->Tipo == Op) {
            if (MinPriorElem == NULL) {
                MinPriorElem = Iter;
                
                if (Iter->Op == Somma || Iter->Op == Diff) {
                    MinPrior = Prior;
                } else {
                    MinPrior = Prior + 1;
                }
            } else {
                if ((Iter->Op == Somma || Iter->Op == Diff) && Prior < MinPrior) {
                    MinPriorElem = Iter;
                    MinPrior = Prior;
                } else if (Prior + 1 < MinPrior) {
                    MinPriorElem = Iter;
                    MinPrior = Prior + 1;
                }
            }
        }
        Iter = Iter->Prec;
    }

    /* Se non ci sono operazioni restituisce il valore che trova in mezzo alle parentesi */
    if (MinPriorElem == NULL) {
        Iter = FineExpr;
        while (Iter != NULL) {
            if (Iter->Tipo == Val) {
                return Iter->Val;
            }

            Iter = Iter->Prec;
        }

        fprintf(stderr, "\033[0;31mtrovata sottoespressione priva di valori\033[0m\n");
        return 0;
    }

    /* Richiama ricorsivamente se stessa svolgendo le operazioni */
    BloccoPrec = Calcola(InizioExpr, MinPriorElem->Prec);
    BloccoSucc = Calcola(MinPriorElem->Succ, FineExpr);

    switch (MinPriorElem->Op) {
    case Somma:
        return BloccoPrec + BloccoSucc;
    case Diff:
        return BloccoPrec - BloccoSucc;
    case Molt:
        return BloccoPrec * BloccoSucc;
    case Div:
        return BloccoPrec / BloccoSucc;
    }
}