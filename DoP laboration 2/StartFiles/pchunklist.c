/*
* File: pqlist.c
* ------------------
*
* Niclas Åstrand, Data- och programstrukturer HT-2003
* Senast modifierad: 23/7 - 2003
*/

#include "pqueue.h"
#include "genlib.h"

#define MAX_ELEMS_PER_BLOCK 20

/* Types */

typedef struct chunkT {
	int values[MAX_ELEMS_PER_BLOCK];
	int elementsInValues;
	int minValue;
	int maxValue;
	struct chunkT *next;
} chunkT;

struct pqueueCDT {
	chunkT *head;
};

/* Exported entries */

static void insertElement(chunkT *chunk, int newValue){
	int i, j;

	for (i = 0; i <= chunk->elementsInValues; i++){
		if (newValue > chunk->values[i]){
			for(j = chunk->elementsInValues; j > i; j--) {
				chunk->values[j] = chunk->values[j-1];
			}
			chunk->values[i] = newValue;
			chunk->elementsInValues++;
			chunk->maxValue = chunk->values[0];
			chunk->minValue = chunk->values[chunk->elementsInValues - 1];
			return;
		}
	}
	if (chunk->elementsInValues == 0) {
		chunk->values[0] = newValue;
		chunk->elementsInValues++;
		chunk->maxValue = chunk->values[0];
		chunk->minValue = chunk->values[0];
	}
	

}

static void splitChunk(chunkT *current){
	int i;
	chunkT *newChunk;

	newChunk = New(chunkT*);
	newChunk->elementsInValues=0;
	newChunk->maxValue = 0;
	newChunk->minValue = 0;

	for (i = MAX_ELEMS_PER_BLOCK/2; i < MAX_ELEMS_PER_BLOCK; i++){
		newChunk->values[i - MAX_ELEMS_PER_BLOCK/2] = current->values[i];
		current->elementsInValues--;
		newChunk->elementsInValues++;
	}
	
	newChunk->maxValue = newChunk->values[0];
	newChunk->minValue = newChunk->values[newChunk->elementsInValues - 1];
	current->minValue = current->values[current->elementsInValues - 1];
	if(current->next == NULL)
		newChunk->next = NULL;
	else
		newChunk->next = current->next;
	current->next = newChunk;
}

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;
	chunkT *chunk;

	chunk = New(chunkT*);
	pqueue = New(pqueueADT);
	pqueue->head = chunk;

	chunk->elementsInValues = 0;
	chunk->maxValue = 0;
	chunk->minValue = 0;
	chunk->next = NULL;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	chunkT *next;

	while (pqueue->head != NULL) {
		next = pqueue->head->next;
		FreeBlock(pqueue->head);
		pqueue->head = next;
	}
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->head->elementsInValues == 0);
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
	chunkT *cur, *prev;

	cur = pqueue->head;

	while (newValue < cur->minValue && cur->next != NULL){
		prev = cur;
		cur = cur->next;
	}

	if (cur->elementsInValues >= MAX_ELEMS_PER_BLOCK)
		splitChunk(cur);

	if (newValue > cur->minValue || cur->next == NULL)
		insertElement(cur, newValue);
	else
		insertElement(cur->next, newValue);



	
}

/* Implementation notes: DequeueMax
* --------------------------------
* Det största värdet sparas först i listan så att det är
* enkelt att ta bort. Notera att minne för noder frigörs
* vid borttagning ur kön.
*/

int DequeueMax(pqueueADT pqueue)
{
	int value, i;
	chunkT *toBeDeleted;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

	value = pqueue->head->values[0];

	for (i = 0; i < pqueue->head->elementsInValues; i++){
		pqueue->head->values[i] = pqueue->head->values[i + 1];
	}

	pqueue->head->elementsInValues--;
	if (pqueue->head->elementsInValues == 0)	{
		toBeDeleted = pqueue->head;
		if (pqueue->head->next != NULL){
			pqueue->head = pqueue->head->next;
			FreeBlock(toBeDeleted);
		}
	}
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
	chunkT *cur;

	total = sizeof(*pqueue);
	for (cur = pqueue->head; cur != NULL; cur = cur->next)
		total += sizeof(*cur);

	return (total);
}
