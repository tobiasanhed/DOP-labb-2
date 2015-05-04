/* File: main.c
 * ------------
 * Enkelt huvudprogram f�r laboration #2 - Prioritetsk�.
 *
 * Niclas �strand, Data- och programstrukturer HT-2003
 * Senast modifierad 23/7 - 2003
 */

#include <stdio.h>
#include "genlib.h"
#include "pqueuetest.h"
#include "performance.h"

int main()
{
	BasicPQueueTest();
	MorePQueueTest();
	PQueueSortTest();
	PQueuePerformance();
	return 0;

}
