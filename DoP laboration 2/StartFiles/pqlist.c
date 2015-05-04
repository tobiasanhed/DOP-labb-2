/*
 * File: pqlist.c
 * ------------------
 *
 * Niclas �strand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#include "pqueue.h"
#include "genlib.h"

/* Types */

typedef struct cellT {
	int value;
	struct cellT *next;
} cellT;

struct pqueueCDT {
	cellT *head;	
};

/* Exported entries */

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->head = NULL;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	cellT *next;

	while (pqueue->head != NULL) {
    	next = pqueue->head->next;
    	FreeBlock(pqueue->head);
    	pqueue->head = next;
    }
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->head == NULL);
}

bool IsFull(pqueueADT pqueue)
{
	return (FALSE);
}

/* Implementation notes: Enqueue
 * -----------------------------
 * F�r att finna r�tt position f�r ins�ttning m�ste en s�kning utf�ras.
 * S�kningar i enkell�nkade listor kan vara 'kn�ligt'. I den h�r
 * implementeringen anv�nds tv� 'paralella pekare, en f�ljer ett steg
 * bakom den andra, till dess att vi finner korrekt position f�r ins�ttning
 * av den nya noden. Notera specialfallet vid ins�ttning vid listans
 * huvud.
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	cellT *cur, *prev, *newCell;
 	
	newCell = New(cellT *);
 	newCell->value = newValue;
 	
 	for (prev = NULL, cur = pqueue->head; cur != NULL; prev = cur, cur = cur->next) {
 		if (newValue > cur->value) break;
 	}
 	newCell->next = cur;
 	if (prev)
 		prev->next = newCell;
 	else
 		pqueue->head = newCell;
}

/* Implementation notes: DequeueMax
 * --------------------------------
 * Det st�rsta v�rdet sparas f�rst i listan s� att det �r
 * enkelt att ta bort. Notera att minne f�r noder frig�rs
 * vid borttagning ur k�n.
 */

int DequeueMax(pqueueADT pqueue)
{
	cellT *toBeDeleted;
	int value;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

 	toBeDeleted = pqueue->head;
 	value = pqueue->head->value;
 	pqueue->head = pqueue->head->next; 	
	FreeBlock(toBeDeleted);
	return (value);
}

/* Implementation notes: BytesUsed
 * -------------------------------
 * Minnes f�rbrukningen utg�rs av minnet f�r en struct pqueueCDT +
 * storleken p� summan av samtliga noder i den l�nkade listan.
 */

int BytesUsed(pqueueADT pqueue)
{
	int total;
	cellT *cur;

	total = sizeof(*pqueue);
	for (cur = pqueue->head; cur != NULL; cur = cur->next)
 		total += sizeof(*cur);

	return (total);
}
