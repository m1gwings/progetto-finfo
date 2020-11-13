#include "interpreta.h"
#include <stdlib.h>

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
     * il numero di caratteri memorizzati è minore della lunghezza massima dell'espressione meno 1,
     * dato che alla fine viene inserito il tappo \x00 */
    while (c = fgetc(FlussoIn), c != Terminatore && ContaCar < LUNG_MAX-1) {
        /* I caratteri di separazione vengono ignorati */
        if (c == ' ' || c == '\t') {
            continue;
        }

        /* Se il carattere letto non è valido la funzione restituisce un errore */
        if (!CratteriValidi[c]) {
            fprintf(stderr, "\033[0;31malcuni dei caratteri inseriti non sono validi\033[0m\n");
            return 1;
        }

        Espressione[ContaCar++] = c;
    }

    return 0;
}

Elemento* InterpretaFrammento(int* Inizio, char* Espressione) {
    /* Puntatore all'elemento interpretato dalla funzione */
    Elemento* Elem;

    /* Alloca dinamicamente il nuovo elemento */
    Elem = (Elemento*)malloc(sizeof(Elemento));

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
        Elem->Op = Aperta;
    } else if (Espressione[*Inizio] == ')') {
        Elem->Tipo = Par;
        Elem->Op = Chiusa;
    } else if (Espressione[*Inizio] == '.') {
        /* Nel caso in cui venisse inserito un punto senza cifre prima o dopo
         * la funziona restituisce NULL */
        free(Elem);
        fprintf(stderr, "\033[0;31mil punto inserito in posizione %d è un errore sintattico\033[0m\n", *Inizio);
        return NULL;
    } else {
        Elem->Tipo = Val;
        /* Sfrutta atof per interpretare il valore float */
        Elem->Val = atof(Espressione+*Inizio);

        /* Questo ciclo serve ad incrementare Inizio fino all'inizo del
         * frammento successivo */
        while ( Espressione[*Inizio] != '+' &&
                Espressione[*Inizio] != '-' &&
                Espressione[*Inizio] != '*' &&
                Espressione[*Inizio] != '/' &&
                Espressione[*Inizio] != '(' &&
                Espressione[*Inizio] != ')' &&
                Espressione[*Inizio] != '\x00') {
            *Inizio++;
        }
    }

    return Elem;
}

int Interpreta(FILE* FlussoIn, Elemento* InizioExpr, Elemento* FineExpr) {
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

    i = 0;
    /* Itera sui caratteri dell'espressione fino al tappo */
    while (Espressione[i] != '\x00') {
        Elem = InterpretaFrammento(&i, Espressione);

        if (Elem == NULL) {
            return 1;
        }


    }

    printf("%s\n", Espressione);

    return 0;
}
