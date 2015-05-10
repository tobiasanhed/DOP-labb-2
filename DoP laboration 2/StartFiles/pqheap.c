/*
* File: pqlist.c
* ------------------
*
* Niclas �strand, Data- och programstrukturer HT-2003
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
* F�r att finna r�tt position f�r ins�ttning m�ste en s�kning utf�ras.
* S�kningar i enkell�nkade listor kan vara 'kn�ligt'. I den h�r
* implementeringen anv�nds tv� 'paralella pekare, en f�ljer ett steg
* bakom den andra, till dess att vi finner korrekt position f�r ins�ttning
* av den nya noden. Notera specialfallet vid ins�ttning vid listans
* huvud.
*/

void Enqueue(pqueueADT pqueue, int newValue)
{
	int temp,
		cpInHeap;

	//Kolla ifall heapen �r full.
	if (pqueue->heap->nodesInHeap + 1 == pqueue->heap->maxNodes)
		extendArray(pqueue);

	//S�tt in v�rdet sist i heapen
	pqueue->heap->heapValues[pqueue->heap->nodesInHeap + 1] = newValue;
	//H�ller koll vart man satt in v�rdet.
	cpInHeap = pqueue->heap->nodesInHeap + 1;

	//Kollar ifall elementet �r st�rre �n sin f�r�lder, isf skifta och forts�tt kolla.
	while (TRUE){
		//Om man �r f�rst i vektorn ska man inte kolla mer..
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
* Det st�rsta v�rdet sparas f�rst i listan s� att det �r
* enkelt att ta bort. Notera att minne f�r noder frig�rs
* vid borttagning ur k�n.
*/

int DequeueMax(pqueueADT pqueue)
{
	int value,
		cpInHeap,
		temp;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");
	// f�rsta v�rdet lagras i variabel
	value = pqueue->heap->heapValues[1];

	//Flytta upp sista elementet i heapen.
	pqueue->heap->heapValues[1] = pqueue->heap->heapValues[pqueue->heap->nodesInHeap];
	pqueue->heap->heapValues[pqueue->heap->nodesInHeap] = -10000;

	//M�blera om heapen s� att det st�rsta v�rdet �terigen ligger l�ngst upp i heapen 
	//och h�ll koll p� vart du �r.
	cpInHeap = 1;
	while (TRUE){

		//Man kan kan inte kolla efter vektorns slut..
		if (cpInHeap * 2 < pqueue->heap->nodesInHeap){
			//Om noden �r mindre en n�got av sina barn, skifta och forts�tt sedan.
			if (pqueue->heap->heapValues[cpInHeap] < pqueue->heap->heapValues[2 * cpInHeap] ||
				pqueue->heap->heapValues[cpInHeap] < pqueue->heap->heapValues[(2 * cpInHeap) + 1]){
				//�r v�nstra barnet st�rre �n det h�gra barnet, byt med v�nstra barnet
				if (pqueue->heap->heapValues[2 * cpInHeap] > pqueue->heap->heapValues[(2 * cpInHeap) + 1]){
					temp = pqueue->heap->heapValues[2 * cpInHeap];
					pqueue->heap->heapValues[2 * cpInHeap] = pqueue->heap->heapValues[cpInHeap];
					pqueue->heap->heapValues[cpInHeap] = temp;
					cpInHeap *= 2;
				}
				else{
					//annars byt med h�gra
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
	
	// counter f�r antalet element minskas
	pqueue->heap->nodesInHeap--;
	// returnera gamla f�rstav�rdet
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

	total = sizeof(*pqueue);
	total += sizeof(pqueue->heap);

	return (total);
}
