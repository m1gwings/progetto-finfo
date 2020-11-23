#include "calcola.h"
#include "espressione.h"
#include "interpreta.h"
#include <stdio.h>

int ParSuperflue(Elemento* InizioExpr, Elemento* FineExpr) {
    /* Array utilizzato come pila di parentesi aperte */
    Elemento* PilaParAperte[LUNG_MAX];

    /* Iteratore che scorre lungo l'espressione */
    Elemento* Iter;
    /* Indice che gestisce la pila PilaParAperte */
    int i;

    for (i = 0, Iter = InizioExpr; Iter != NULL && Iter->Prec != FineExpr;
        Iter = Iter->Succ) {
        if (Iter->Tipo == Par) {
            if (Iter->Par == Aperta) {
                /* Ogni volta che l'iteratora incontra una parentesi aperta nell'espressione
                 * la aggiunge alla pila */
                PilaParAperte[i++] = Iter;
            } else {
                 /* Se invece incontra una parentesi chiusa allora */
                if (Iter == FineExpr && PilaParAperte[i-1] == InizioExpr) {
                    /* Se l'elemento corrispondente alla parentesi chiusa
                     * è l'ultimo dell'espressione e l'elemento in cima allo stack,
                     * che corrisponde alla parentesi aperta che viene chiusa da
                     * quella puntata da Iter, è il primo dell'espressione,
                     * allora la parentesi iniziale e quella finale sono superflue */
                    return 1;
                } else {
                    /* Altrimenti rimuove dalla pila l'ultima parentesi aperta */
                    i--;
                }
            }
        }
    }

    return 0;
}

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

    /* Elimina le parentesi superflue all'inizio ed alla fine finchè ci sono
     * esempio: (4 + 5) diventa 4 + 5
     * sfrutta il valore restituito da ParSuperflue */
    while (ParSuperflue(InizioExpr, FineExpr)) {
        InizioExpr = InizioExpr->Succ;
        FineExpr = FineExpr->Prec;
    }

    /* Se la sottoespressione inizia o finisce con un'operazione c'è stato un errore */
    if (InizioExpr->Tipo == Op || FineExpr->Tipo == Op) {
        fprintf(stderr, "\033[0;31mtrovata sottoespressione che inizia o finisce con una operazione\033[0m\n");
        return 0;
    }

    if (InizioExpr == FineExpr) {
        /* Se inizio e fine della sottoespressione coincidono */
        if (InizioExpr->Tipo == Val) {
            /* la funzione restituisce il valore dell'elemento se è un numero */
            return InizioExpr->Val;
        } else {
            /* altrimenti c'è stato un errore */
            fprintf(stderr, "\033[0;31mtrovata sottoespressione di un elemento che non è un numero\033[0m\n");
            return 0;
        }
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
