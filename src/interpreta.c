#include "interpreta.h"
#include <stdlib.h>
#include <math.h>

/* Lunghezza massima della stringa contenente l'espressione */
#define LUNG_MAX 100

int LeggiEspressione(FILE* FlussoIn, char* Espressione) {
    /* L'array associa a ciascun carattere ASCII il valore
     *  - 0 nel caso in cui il carattere non dovrebbe comparire all'interno dell'espressione 
     *  - 1 altrimenti */
    int CratteriValidi[128] = {0};

    /* Se il carattere letto è uguale a Terminatore, il ciclo di lettura si interrompe */
    int Terminatore;
    /* Conta il numero di caratteri letti e memorizzati in espressione,
     * i caratteri di separazione vengono ignorati */
    int ContaCar;
    /* Carattere letto durante il ciclo */
    char c;

    /* Si definisce differentemente il Terminatore a seconda del flusso di input */
    if (FlussoIn == stdin) {
        Terminatore = '\n';
    } else {
        Terminatore = EOF;
    }

    /* Cifre */
    CratteriValidi['0'] = 1;
    CratteriValidi['1'] = 1;
    CratteriValidi['2'] = 1;
    CratteriValidi['3'] = 1;
    CratteriValidi['4'] = 1;
    CratteriValidi['5'] = 1;
    CratteriValidi['6'] = 1;
    CratteriValidi['7'] = 1;
    CratteriValidi['8'] = 1;
    CratteriValidi['9'] = 1;

    /* Punto */
    CratteriValidi['.'] = 1;

    /* Operatori */
    CratteriValidi['+'] = 1;
    CratteriValidi['-'] = 1;
    CratteriValidi['*'] = 1;
    CratteriValidi['/'] = 1;

    /* Parantesi */
    CratteriValidi['('] = 1;
    CratteriValidi[')'] = 1;

    ContaCar = 0;

    /* Il ciclo continua a leggere caratteri fino al terminatore o fino a quando
     * il numero di caratteri memorizzati è minore della lunghezza massima dell'espressione meno 2,
     * dato che alla fine viene inserito il tappo \x00 e che se c'è un '-' dopo un'operazione
     * o una parentesi aperta allora vengono inseriti due caratteri nella stessa iterazione */
    while (c = fgetc(FlussoIn), c != Terminatore && ContaCar < LUNG_MAX-2) {
        /* I caratteri di separazione vengono ignorati */
        if (c == ' ' || c == '\t' || c == '\n') {
            continue;
        }

        /* Se il carattere letto non è valido la funzione restituisce un errore */
        if (!CratteriValidi[c]) {
            fprintf(stderr, "\033[0;31malcuni dei caratteri inseriti non sono validi\033[0m\n");
            return 1;
        }

        if (c == '-' && (ContaCar == 0 ||
            Espressione[ContaCar-1] == '+' ||
            Espressione[ContaCar-1] == '-' ||
            Espressione[ContaCar-1] == '(')) {
            Espressione[ContaCar++] = '0';
        }

        Espressione[ContaCar++] = c;
    }

    Espressione[ContaCar] = '\x00';

    return 0;
}

Elemento* InterpretaFrammento(int* Inizio, char* Espressione) {
    /* Puntatore all'elemento interpretato dalla funzione */
    Elemento* Elem;

    /* Alloca dinamicamente il nuovo elemento */
    Elem = (Elemento*)malloc(sizeof(Elemento));

    Elem->Succ = NULL;
    Elem->Prec = NULL;

    /* A seconda del carattere letto inizializza Elem */
    if (Espressione[*Inizio] == '+') {
        Elem->Tipo = Op;
        Elem->Op = Somma;
    } else if (Espressione[*Inizio] == '-') {
        Elem->Tipo = Op;
        Elem->Op = Diff;
    } else if (Espressione[*Inizio] == '*') {
        Elem->Tipo = Op;
        Elem->Op = Molt;
    } else if (Espressione[*Inizio] == '/') {
        Elem->Tipo = Op;
        Elem->Op = Div;
    } else if (Espressione[*Inizio] == '(') {
        Elem->Tipo = Par;
        Elem->Par = Aperta;
    } else if (Espressione[*Inizio] == ')') {
        Elem->Tipo = Par;
        Elem->Par = Chiusa;
    } else if (Espressione[*Inizio] == '.') {
        /* Nel caso in cui venisse inserito un punto senza cifre prima o dopo
         * la funziona restituisce NULL */
        free(Elem);
        fprintf(stderr, "\033[0;31mil punto inserito in posizione %d è un errore sintattico\033[0m\n", *Inizio);
        return NULL;
    } else {
        /* Ha valore 0 se il ciclo sta iterando sulla parte intera del numero da interpretare,
         * dopo aver raggiunto il carattere '.' ha valore incrementale */
        int ParteDecimale = 0;
        Elem->Tipo = Val;

        Elem->Val = 0;
        /* Questo ciclo serve ad incrementare Inizio fino all'inizo del
         * frammento successivo e converte i caratteri nel valore double */
        while ( Espressione[*Inizio] != '+' &&
                Espressione[*Inizio] != '-' &&
                Espressione[*Inizio] != '*' &&
                Espressione[*Inizio] != '/' &&
                Espressione[*Inizio] != '(' &&
                Espressione[*Inizio] != ')' &&
                Espressione[*Inizio] != '\x00') {
            if (Espressione[*Inizio] == '.') {
                ParteDecimale = 1;
                (*Inizio)++;
                continue;
            }

            if (ParteDecimale > 0) {
                Elem->Val += (double)(Espressione[*Inizio]-'0') * (double)pow(10, -ParteDecimale);
                ParteDecimale++;
            } else {
                Elem->Val *= 10;
                Elem->Val += Espressione[*Inizio]-'0';
            }
            (*Inizio)++;
        }

        if (ParteDecimale == 1) {
            fprintf(stderr, "\033[0;31mil punto inserito in posizione %d è un errore sintattico\033[0m\n", *Inizio-1);
            return NULL;
        }

        /* Inizio viene decrementato perchè sarà poi incrementato alla fine della funzione */
        (*Inizio)--;
    }

    /* Incrementa l'iteratore che ora punta all'inizio del frammento successivo */
    (*Inizio)++;

    return Elem;
}

int SequenzaValida(Elemento* Prec, Elemento* Succ) {
    if (Succ->Tipo == Op) {
        /* Il susseguirsi di due operazioni è un errore sintattico */
        if (Prec->Tipo == Op) {
            return 0;
        }

        /* Un'operazione dopo una parentesi aperta è un errore sintattico */
        if (Prec->Tipo == Par && Prec->Par == Aperta) {
            return 0;
        }

        return 1;
    } else if (Succ->Tipo == Par) {
        if (Succ->Par == Aperta) {
            /* Una parentesi può stare dopo un'operazione oppure dopo un'altra
             * parentesi aperta */
            if (Prec->Tipo == Op || (Prec->Tipo == Par && Prec->Par == Aperta)) {
                return 1;
            }

            return 0;
        } else {
            /* Una parentesi chiuse dopo un'operazione opppure dopo una parentesi
             * aperta è un errore sintattico */
            if (Prec->Tipo == Op || (Prec->Tipo == Par && Prec->Par == Aperta)) {
                return 0;
            }

            return 1;
        }
    } else if (Succ->Tipo == Val) {
        /* Un valore può stare dopo un'operazione oppure dopo una parentesi aperta */
        if (Prec->Tipo == Op || (Prec->Tipo == Par && Prec->Par == Aperta)) {
            return 1;
        }

        return 0;
    }
}

int Interpreta(FILE* FlussoIn, Elemento** InizioExpr, Elemento** FineExpr) {
    /* Memorizza i caratteri che costituiscono l'espressione */
    char Espressione[LUNG_MAX];
    /* Indice per iterare sui caratteri in Espressione */
    int i;
    /* Puntatore all'elemento restituito dall'ultima chiamata
     * di InterpretaFrammento */
    Elemento* Elem;
    /* Memorizzano il numero di parentesi aperte e chiuse interpretate,
     * alla fine dell'interpretazione i due numeri devono combaciare e
     * durante l'esecuzione deve valere ParChiuse <= ParAperte */
    int ParAperte, ParChiuse;

    /* Legge i caratteri in ingresso e li memorizza in Espressione */
    if (LeggiEspressione(FlussoIn, Espressione)) {
        return 1;
    }

    (*InizioExpr) = NULL, (*FineExpr) = NULL;

    ParAperte = 0, ParChiuse = 0;

    /* La variabile i viene opportunamente incrementata da InterpretaFrammento */
    i = 0;
    /* Itera sui caratteri dell'espressione fino al tappo */
    while (Espressione[i] != '\x00') {
        Elem = InterpretaFrammento(&i, Espressione);

        if (Elem == NULL) {
            return 1;
        }

        if ((*InizioExpr) == NULL) {
            /* L'inizio non è definito, quindi è il primo elemento */
            /* Un'espressione può iniziare solo con un valore o con '(' */
            if (Elem->Tipo == Val || (Elem->Tipo == Par && Elem->Par == Aperta)) {
                (*InizioExpr) = Elem;
                (*FineExpr) = Elem;
            } else {
                fprintf(stderr, "\033[0;31ml'espressione non può iniziare con %c\033[0m\n", Espressione[0]);
                return 1;
            }
        } else {
            /* Dal secondo elemento in poi */
            if (SequenzaValida((*FineExpr), Elem)) {
                (*FineExpr)->Succ = Elem;
                Elem->Prec = (*FineExpr);
                (*FineExpr) = Elem;
            } else {
                fprintf(stderr, "\033[0;31merrore sintattico prima dell'indice %d\033[0m\n", i);
                return 1;
            }
        }

        if (Elem->Tipo == Par) {
            if (Elem->Par == Aperta) {
                ParAperte++;
            } else {
                ParChiuse++;
            }
        }
    }

    if (ParAperte != ParChiuse) {
        fprintf(stderr, "\033[0;31mci sono %d parentesi aperte e %d chiuse\033[0m\n", ParAperte, ParChiuse);
        return 1;
    }

    return 0;
}
