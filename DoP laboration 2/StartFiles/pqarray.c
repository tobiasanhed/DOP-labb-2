/*
 * File: pqueue.c
 * --------------
 * Den h�r filen implementerar en prioritetsk� med ett
 * f�lt med konstant storlek. Implementeringen g�r det l�tt
 * att s�tta in nya element med sv�rt att plocka ut det st�rsta.
 *
 * Niclas �strand, Data- och programstrukturer HT-2003
 * Senast modifierad: 23/7 - 2003
 */

#include "pqueue.h"
#include "genlib.h"

/* Constant: MAX_ELEMENTS
 * ----------------------
 * Den h�r konstanten anger antalet element det f�lt som
 * utg�r representationen av prioritetsk�n rymmer.
 */

#define MAX_ELEMENTS 1200000

struct pqueueCDT {
	int entries[MAX_ELEMENTS];
	int numEntries;
};

/* Exported endries */

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->numEntries = 0;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->numEntries == 0);
}

bool IsFull(pqueueADT pqueue)
{
	return (pqueue->numEntries == MAX_ELEMENTS);
}

/*
 * Implementation notes: Enqueue
 * -----------------------------
 * D� elementen sparas osorterat i f�ltet beh�ver endast nya
 * elementet placeras i slutet av f�ltet.
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	if (IsFull(pqueue))
		Error("Tried to enqueue into a priority queue which is full!");
	pqueue->entries[pqueue->numEntries++] = newValue;
}

/*
 * Implementation notes: DequeueMax
 * -------------------------------------------------
 * D� elementen sparas osorterat i f�ltet m�ste en s�kning g�ras
 * f�r att finna det st�rsta elementet. F�r att ta bort det st�rsta
 * elementet flyttas det sista elementet i f�ltet till den position
 * i vilken det st�rsta elementet �terfanns samtidigt som antalet
 * element i f�ltet minskas med 1. Det finns ingen anledning att skifta
 * samtliga element ett steg �t v�nster f�r att t�cka det gap som uppst�r
 * vid borttagning av ett element. Det v�rde som tas bort returneras fr�n
 * funktionen.
 */

int DequeueMax(pqueueADT pqueue)
{
	int maxIndex, value, i;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

	maxIndex = 0;
	for (i = 1; i < pqueue->numEntries; i++) {
		if (pqueue->entries[i] > pqueue->entries[maxIndex])
			maxIndex = i;
	}
	/* spara v�rdet som skall returneras */
	value = pqueue->entries[maxIndex];
	/* flytta det sista v�rdet i f�ltet hit */
	pqueue->entries[maxIndex] = pqueue->entries[--(pqueue->numEntries)];
	return (value);
}

int BytesUsed(pqueueADT pqueue)
{
	return (sizeof(*pqueue));
}
