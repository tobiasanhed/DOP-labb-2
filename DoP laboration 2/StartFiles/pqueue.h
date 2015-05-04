/*
 * File: pqueue.h
 * --------------
 * Gr�nssnitt f�r abstrakt datatyp pqueueADT
 *
 * Niclas �strand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#ifndef _pqueue_h
#define _pqueue_h

#include "genlib.h"

/*
 * Type: pqueueADT
 * ---------------
 * Det h�r �r en abstrakt datatyp som representerar en
 * prioritetsk�. En prioritetsk� �r inte en vanligt FIFO-k�
 * utan en k� d�r elementen plockas ut i en prioritetsordning.
 * Elementen i prioritetsk�n �r heltal och representerar
 * prioriteten (st�rre heltal har h�gre prioritet).
 */

typedef struct pqueueCDT *pqueueADT;

/*
 * Function: NewPQueue
 * Usage: pqueue = NewPQueue();
 * ----------------------------
 * Skapar en tom prioritetsk�.
 */

pqueueADT NewPQueue(void);

/*
 * Function: FreePQueue
 * Usage: FreePQueue(pqueue);
 * -------------------------
 * Frig�r minnet associerat med en prioritetsk�.
 */

void FreePQueue(pqueueADT pqueue);

/*
 * Function: IsEmpty
 * Usage: if (IsEmpty(pqueue)) . . .
 * ---------------------------------
 * Returnerar TRUE om prioritetsk�n �r tom.
 */

bool IsEmpty(pqueueADT pqueue);

/*
 * Function: IsFull
 * Usage: if (IsFull(pqueue)) . . .
 * ---------------------------------
 * Returnerar TRUE om prioritetsk�n inte har plats f�r
 * ytterligare element. Klienter kan anv�nda den h�r funtionen
 * f�r att kontrollera om ytterligare ins�ttningar kommer medf�ra
 * att prioritetk�ns kapacitet �verstigs. Notera att somliga
 * implementeringar alltid returnerar FALSE.
 */

bool IsFull(pqueueADT pqueue);

/*
 * Function: Enqueue
 * Usage: Enqueue(pqueueADT pqueue, int newValue);
 * -----------------------------------------------
 * Ins�ttning av angivet v�rde (newValue) i prioritetsk�n. Om prioritetsk�n
 * �r full kommer funktionen att rapportera detta.
 */

void Enqueue(pqueueADT pqueue, int newValue);

/*
 * Function: DequeueMax
 * Usage: maxElement = DequeueMax(pqueueADT pqueue);
 * -------------------------------------------------
 * Tar bort, och returnerar, elementet med h�gst prioritet i k�n.
 * Om prioritetsk�n �r tom kommer funktionen att rapportera detta.
 */

int DequeueMax(pqueueADT pqueue);

/*
 * Function: BytesUsed
 * Usage: size = BytesUsed(pqueueADT pqueue);
 * ------------------------------------------
 * Givet en prioritetsk� s� ber�knar den h�r funktionen, och returnerar,
 * det totala antalet bytes k�n f�rbrukar.
 */

int BytesUsed(pqueueADT pqueue);

#endif
