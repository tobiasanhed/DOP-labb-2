/*
 * File: pqueuetest.h
 * ------------------
 * Den h�r modulen exporterar ett antal funktiner som testar
 * olika operatiner p� en pqueueADT. Funktionerna �r
 * anv�ndbara f�r att testa om en ny implementering �r korrekt.
 *
 * Niclas �strand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#ifndef _pqueuetest_h
#define _pqueuetest_h

#include "pqueue.h"

/*
 * Function: BasicPQueueTest
 * Usage: BasicQueueTest();
 * ------------------------
 * Testar enkla Enqueue och DequeueMax.
 * Resultatet skrivs ut till konsolen.
 */

void BasicPQueueTest(void);

/*
 * Function: MoreQueueTest
 * Usage: MoreQueueTest();
 * -----------------------
 * Testar ytterligare Enqueue, DequeueMax, en del 'boundry cases'
 * testas �ven.
 * Resultatet skrivs ut till konsolen.
 */

void MorePQueueTest();

/*
 * Function: PQueueSortTest
 * Usage: PQueueSortTest();
 * ------------------------
 * Testar anv�ndning av prioritetsk�n genom att implementera sorterings-
 * algoritmen HeapSort. Funktionen s�tter in en m�ngd v�rden i k�n
 * f�r att sedan anv�nda DequeueMax f�r att plocka ut elementen
 * och placera dem i ett f�lt i sorterad ordning. Utf�r f�rst operationen
 * med slumpade v�rden. Operationen utf�rs sedantv� g�nger till genom
 * att s�tta in v�rden i sorterad ordning (b�de fram- och bakl�nges) f�r
 * att avg�ra hur operationen p�verkas av data som redan �r sorterad.
 */

void PQueueSortTest();

/*
 * Function: Heapsort
 * Usage: Heapsort(pq, arr, n);
 * ----------------------------
 * Anv�nder prioritetsk�n f�r att implementera algoritmen HeapSort.
 * Algoritmen s�tter in alla element i array i prioritetsk�n f�r
 * att sedan plocka ut dem med DequeueMax och placera tillbaka dem
 * i array i stigande ordning. Beroende p� den underliggande
 * representationen av k�n s� kan detta vara en relativt snabb
 * sorteringsalgoritm. Funktionen exporteras eftersom den anv�nds
 * av performance.c s�v�l som den h�r modulen.
 */

void HeapSort(pqueueADT pq, int array[], int nElements);

#endif
