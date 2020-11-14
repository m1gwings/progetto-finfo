/* Definisce i tipi di dati utilizzati per rappresentare le espressioni */
#ifndef ESPRESSIONE_H
#define ESPRESSIONE_H

/* TipoElemento descrive il tipo di elemento che può comporre un'espressione:
 *  - Op  -> Operazione
 *  - Par -> Parentesi
 *  - Val -> Un qualsiasi valore double */
typedef enum {
    Op,
    Par,
    Val
} TipoElemento;

/* Operazione descrive il tipo di operazione che può comparire in un'espressione:
 *  - Somma -> Somma
 *  - Diff -> Differenza
 *  - Molt -> Moltiplicazione
 *  - Div -> Divisione */
typedef enum {
    Somma,
    Diff,
    Molt,
    Div
} Operazione;

/* Parentesi descrive il tipo di parentesi che può comparire in un'espressione */
typedef enum {
    Aperta,
    Chiusa
} Parentesi;

/* Elemento è il generico elemento che compone l'espressione
 * Op ha significato quando Tipo == Op
 * Par ha significato quando Tipo == Par
 * Val ha significato quando Tipo == Val
 * Succ è un puntatore all'elemento successivo nell'espressione
 * Prec è un puntatore all'elemento precedente nell'espressione */
typedef struct Elemento {
    TipoElemento Tipo;
    Operazione Op;
    Parentesi Par;
    double Val;

    struct Elemento* Succ;
    struct Elemento* Prec;
} Elemento;

#endif
