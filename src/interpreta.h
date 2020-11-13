#ifndef INTERPRETA_H
#define INTERPRETA_H
#include <stdio.h>
#include "espressione.h"

/* LeggiEspressione legge caratteri da FlussoIn e li memorizza in Espressione
 * La lettura termina con \n nel caso in cui FlussoIn == stdin,
 * altrimenti la lettura termina con EOF
 * I caratteri di separazione vengono ignorati
 *
 * La funzione restituisce 0 nel caso in cui non ci siano errori,
 * 1 altrimenti */
int LeggiEspressione(FILE* FlussoIn, char* Espressione);

/* InterpretaFrammento interpreta il singolo elemento presente in Espressione
 * a partire da Inizio ed incrementa Inizio fino all'inizio del frammento successivo
 *
 * La funzione restituisce un puntatore ad un Elemento nel caso in cui l'interpretazione vada a buon fine,
 * NULL altrimenti */
Elemento* InterpretaFrammento(int* Inizio, char* Espressione);

/* Interpeta richiama LeggiEspressione(FlussoIn, Espressione) e successivamente
 * converte i caratteri contenuti in espressione in Elementi dell'espressione, 
 * rappresentata da una lista dinamica con testa in InizioExpr e coda in FineExpr
 *
 * La funzione restituisce 0 nel caso in cui non ci siano errori,
 * 1 altrimenti */
int Interpreta(FILE* FlussoIn, Elemento* InizioExpr, Elemento* FineExpr);

#endif