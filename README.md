# risolvi-espressioni
*Programma che calcola il valore di espressioni algebriche*

*Realizzato per un progetto assegnato durante il corso di Fondamenti di Informatica del Prof. Vincenzo Caglioti*
## Utilizzo
Invocando il programma senza opzioni sarà richiesto di inserire l'espressione algebrica da valutare
```sh
>> ./risolvi-espressioni

<< Inserisci l'espressione da valutare
<< [Termina con \n]
>> (1+2)/3
<< 1.000000
```
##### *>> indica l'input*

##### *<< indica l'output*
L'unica opzione che il programma sa interpretare è *-f nomefile*
```sh
>> cat espressione.txt

<< (2-3) * 4

>> ./risolvi-espressioni -f espressione.txt

<< -4.000000
```
