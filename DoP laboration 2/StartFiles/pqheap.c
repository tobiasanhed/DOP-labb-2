/*
* File: pqlist.c
* ------------------
*
* Niclas Åstrand, Data- och programstrukturer HT-2003
* Senast modifierad: 23/7 - 2003
*/

#include "pqueue.h"
#include "genlib.h"

#define START_SIZE 100

/* Types */

typedef struct heapT {
	int *heapValues;
	int	nodesInHeap,
		maxNodes;
} heapT;

struct pqueueCDT {
	heapT *heap;
};

/* Exported entries */

static void extendArray(pqueueADT pqueue){
	
	int i;
	int *newArray;

	newArray = NewArray(pqueue->heap->maxNodes * 2, int);

	for (i = 1; i <= pqueue->heap->maxNodes; i++)
		newArray[i] = pqueue->heap->heapValues[i];

	FreeBlock(pqueue->heap->heapValues);
	pqueue->heap->heapValues = newArray;
	pqueue->heap->maxNodes *= 2;
	
}



pqueueADT NewPQueue(void)
{

	int i;
	pqueueADT pqueue;
	heapT *heap;

	heap = New(heapT*);
	pqueue = New(pqueueADT);
	pqueue->heap = heap;

	heap->nodesInHeap = 0;
	heap->maxNodes = START_SIZE;
	heap->heapValues = NewArray(START_SIZE, int);

	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	FreeBlock(pqueue->heap);
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->heap->nodesInHeap == 0);
}

bool IsFull(pqueueADT pqueue){
	return (FALSE);
}

/* Implementation notes: Enqueue
* -----------------------------
* För att finna rätt position för insättning måste en sökning utföras.
* Sökningar i enkellänkade listor kan vara 'knöligt'. I den här
* implementeringen används två 'paralella pekare, en följer ett steg
* bakom den andra, till dess att vi finner korrekt position för insättning
* av den nya noden. Notera specialfallet vid insättning vid listans
* huvud.
*/

void Enqueue(pqueueADT pqueue, int newValue)
{
	int temp,
		cpInHeap;

	//Kolla ifall heapen är full.
	if (pqueue->heap->nodesInHeap + 1 == pqueue->heap->maxNodes)
		extendArray(pqueue);

	//Sätt in värdet sist i heapen
	pqueue->heap->heapValues[pqueue->heap->nodesInHeap + 1] = newValue;
	//Håller koll vart man satt in värdet.
	cpInHeap = pqueue->heap->nodesInHeap + 1;

	//Kollar ifall elementet är större än sin förälder, isf skifta och fortsätt kolla.
	while (TRUE){
		//Om man är först i vektorn ska man inte kolla mer..
		if (cpInHeap == 1) break;
		if (pqueue->heap->heapValues[cpInHeap] > pqueue->heap->heapValues[cpInHeap / 2]){
			temp = pqueue->heap->heapValues[cpInHeap / 2];
			pqueue->heap->heapValues[cpInHeap / 2] = pqueue->heap->heapValues[cpInHeap];
			pqueue->heap->heapValues[cpInHeap] = temp;
			cpInHeap /= 2;
		}
		else
			break;
	}
	pqueue->heap->nodesInHeap++;
}

/* Implementation notes: DequeueMax
* --------------------------------
* Det största värdet sparas först i listan så att det är
* enkelt att ta bort. Notera att minne för noder frigörs
* vid borttagning ur kön.
*/

int DequeueMax(pqueueADT pqueue)
{
	int value,
		cpInHeap,
		temp;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");
	// första värdet lagras i variabel
	value = pqueue->heap->heapValues[1];

	//Flytta upp sista elementet i heapen.
	pqueue->heap->heapValues[1] = pqueue->heap->heapValues[pqueue->heap->nodesInHeap];
	pqueue->heap->heapValues[pqueue->heap->nodesInHeap] = -10000;

	//Möblera om heapen så att det största värdet återigen ligger längst upp i heapen 
	//och håll koll på vart du är.
	cpInHeap = 1;
	while (TRUE){

		//Man kan kan inte kolla efter vektorns slut..
		if (cpInHeap * 2 < pqueue->heap->nodesInHeap){
			//Om noden är mindre en något av sina barn, skifta och fortsätt sedan.
			if (pqueue->heap->heapValues[cpInHeap] < pqueue->heap->heapValues[2 * cpInHeap] ||
				pqueue->heap->heapValues[cpInHeap] < pqueue->heap->heapValues[(2 * cpInHeap) + 1]){
				//Är vänstra barnet större än det högra barnet, byt med vänstra barnet
				if (pqueue->heap->heapValues[2 * cpInHeap] > pqueue->heap->heapValues[(2 * cpInHeap) + 1]){
					temp = pqueue->heap->heapValues[2 * cpInHeap];
					pqueue->heap->heapValues[2 * cpInHeap] = pqueue->heap->heapValues[cpInHeap];
					pqueue->heap->heapValues[cpInHeap] = temp;
					cpInHeap *= 2;
				}
				else{
					//annars byt med högra
					temp = pqueue->heap->heapValues[(2 * cpInHeap) + 1];
					pqueue->heap->heapValues[(2 * cpInHeap) + 1] = pqueue->heap->heapValues[cpInHeap];
					pqueue->heap->heapValues[cpInHeap] = temp;
					cpInHeap = (cpInHeap * 2) + 1;
				}
			}
			else
				break;
		}
		else
			break;
	}
	
	// counter för antalet element minskas
	pqueue->heap->nodesInHeap--;
	// returnera gamla förstavärdet
	return (value);
}

/* Implementation notes: BytesUsed
* -------------------------------
* Minnes förbrukningen utgörs av minnet för en struct pqueueCDT +
* storleken på summan av samtliga noder i den länkade listan.
*/

int BytesUsed(pqueueADT pqueue)
{
	int total;

	total = sizeof(*pqueue);
	total += sizeof(pqueue->heap);

	return (total);
}
