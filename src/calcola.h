#ifndef CALCOLA_H
#define CALCOLA_H
#include "espressione.h"

/* Calcola è una funzione ricorsiva che riceve InizioExpr e FineExpr,
 * che sono rispettivamete testa e coda della lista dinamica rappresentante
 * l'espressione aritmetica, e ne restituisce il risultato */
double Calcola(Elemento* InizioExpr, Elemento* FineExpr);

/* ParSuperflue è una funzione che rileva le parentesi superflue all'Inizio
 * ed alla fine dell'espressione
 * esempio: in (4 + 5) le parentesi sono superflue e possono essere eliminate
 *
 * La funzione restituisce 1 se ci sono parentesi superflue all'inizio ed alla
 * fine dell'espressione, 0 altrimenti */
int ParSuperflue(Elemento* InizioExpr, Elemento* FineExpr);

#endif
