#ifndef CALCOLA_H
#define CALCOLA_H
#include "espressione.h"

/* Calcola è una funzione ricorsiva che riceve InizioExpr e FineExpr,
 * che sono rispettivamete testa e coda della lista dinamica rappresentante
 * l'espressione aritmetica, e ne restituisce il risultato */
double Calcola(Elemento* InizioExpr, Elemento* FineExpr);

#endif