/*
* File: pqlist.c
* ------------------
*
* Niclas �strand, Data- och programstrukturer HT-2003
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
	int totalElements;
};

/* Exported entries */

static void insertElement(chunkT *chunk, int newValue){
	int i, j;
	// f�r varje element i chunken
	for (i = 0; i <= chunk->elementsInValues; i++){
		// om nya v�rdet �r st�rre �n v�rdet vi tittar p�
		if (newValue >= chunk->values[i]){
			for(j = chunk->elementsInValues; j > i; j--) {
				chunk->values[j] = chunk->values[j-1];
			}
			chunk->values[i] = newValue;
			chunk->elementsInValues++;
			chunk->maxValue = chunk->values[0];
			chunk->minValue = chunk->values[chunk->elementsInValues - 1];
			return;
		}
		if(i == chunk->elementsInValues)
			chunk->values[i] = newValue;
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
		current->values[i] = -100000;
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
	for(i=MAX_ELEMS_PER_BLOCK / 2; i < MAX_ELEMS_PER_BLOCK; i++){
		newChunk->values[i] = -100000;
	}
}

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;
	chunkT *chunk;
	int i;

	chunk = New(chunkT*);
	pqueue = New(pqueueADT);
	pqueue->head = chunk;
	pqueue->totalElements = 0;

	chunk->elementsInValues = 0;
	chunk->maxValue = 0;
	chunk->minValue = 0;
	chunk->next = NULL;
	for(i=0; i < MAX_ELEMS_PER_BLOCK; i++){
		chunk->values[i] = -100000;
	}
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
* F�r att finna r�tt position f�r ins�ttning m�ste en s�kning utf�ras.
* S�kningar i enkell�nkade listor kan vara 'kn�ligt'. I den h�r
* implementeringen anv�nds tv� 'paralella pekare, en f�ljer ett steg
* bakom den andra, till dess att vi finner korrekt position f�r ins�ttning
* av den nya noden. Notera specialfallet vid ins�ttning vid listans
* huvud.
*/

void Enqueue(pqueueADT pqueue, int newValue)
{
	chunkT *cur, *prev;
	// s�tt cursor p� f�rsta chunken
	cur = pqueue->head;
	// s�tt cursor p� r�tt chunk om den inte redan �r d�r (kollar �ven s� att det finns en n�sta-chunk)
	while (newValue < cur->minValue && cur->next != NULL){
		prev = cur;
		cur = cur->next;
	}
	// �r chunken full
	if (cur->elementsInValues >= MAX_ELEMS_PER_BLOCK) {
		// splitta
		splitChunk(cur);
		// s�tt in newValue p� r�tt chunk
		if(newValue >= cur->minValue)
			insertElement(cur, newValue);
		else
			insertElement(cur->next, newValue);
	}
	else {
		insertElement(cur, newValue);
	}
	pqueue->totalElements++;


	
}

/* Implementation notes: DequeueMax
* --------------------------------
* Det st�rsta v�rdet sparas f�rst i listan s� att det �r
* enkelt att ta bort. Notera att minne f�r noder frig�rs
* vid borttagning ur k�n.
*/

int DequeueMax(pqueueADT pqueue)
{
	int value, i;
	chunkT *toBeDeleted;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");
	// f�rsta v�rdet lagras i variabel
	value = pqueue->head->values[0];
	// counter f�r antalet element minskas
	pqueue->head->elementsInValues--;
	// flytta alla element som finns kvar ett steg fram�t
	for (i = 0; i < pqueue->head->elementsInValues; i++){
		pqueue->head->values[i] = pqueue->head->values[i + 1];
	}
	// �r chunken tom
	if (pqueue->head->elementsInValues == 0)	{
		// och det finns en chunk efter head chunken
		if (pqueue->head->next != NULL){
			// ta bort head chunken, byt ut till head->next
			toBeDeleted = pqueue->head;
			pqueue->head = pqueue->head->next;
			FreeBlock(toBeDeleted);
		}
		else {
			pqueue->head->maxValue = 0;
			pqueue->head->minValue = 0;
			for(i=0; i < MAX_ELEMS_PER_BLOCK; i++){
				pqueue->head->values[i] = -100000;
			}
		}
	}
	// returnera gamla f�rstav�rdet
	pqueue->totalElements--;
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
	chunkT *cur;

	total = sizeof(*pqueue);
	for (cur = pqueue->head; cur != NULL; cur = cur->next)
		total += sizeof(*cur);

	return (total);
}
