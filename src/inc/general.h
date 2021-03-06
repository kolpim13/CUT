#ifndef _GENERAL_H
#define _GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <threads.h>

// QUEUE
typedef struct _Queue {
	void** data;
	size_t length;
	size_t amount;
	size_t pos;
	mtx_t mutex;
}Queue;

Queue* Queue_new(size_t length);
void Queue_free(Queue* q);

bool Queue_add(Queue* q, void* elem);
void* Queue_getLast(Queue* q);
void* Queue_pop(Queue* q);
bool Queue_isFull(Queue* q);
bool Queue_isEmpty(Queue* q);
//-----------------------------------------------

bool CPU_amount_init(void);
size_t Get_CPU(void);
//-----------------------------------------------

#endif // !_GENERAL_H
