#include "general.h"

Queue* Queue_new(size_t length) {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->data = (void**)malloc(length * sizeof(void*));
	q->length = length;
	q->amount = 0;
	q->pos = 0;
	return q;
}
void Queue_free(Queue* q) {
	free(q->data);
	free(q);
}

bool Queue_add(Queue* q, void* elem) {
	if (Queue_isFull(q) == true) {
		return false;
	}

	q->data[q->pos] = elem;
	q->amount++;

	return true;
}
bool Queue_getLast(Queue* q, void* elem) {
	if (Queue_isEmpty(q) == true) {
		return false;
	}

	elem = q->data[q->pos];
	return true;
}
bool Queue_pop(Queue* q, void* elem) {
	if (Queue_isEmpty(q) == true) {
		return false;
	}

	elem = q->data[q->pos];
	q->amount--;
	q->pos++;
	if (q->pos >= q->length) {
		q->pos = 0;
	}
	return true;
}
bool Queue_isFull(Queue* q) {
	return (q->amount == q->length - 1) ? true : false;
}
bool Queue_isEmpty(Queue* q) {
	return (q->amount == 0) ? true : false;
}
//-----------------------------------------------

static size_t CPU_amount;
bool CPU_amount_init(void) {
	char str[128];
	FILE* fp = fopen("/proc/stat", "r");
	
	size_t index = 0;
	while (strstr(fgets(str, 128, fp), "cpu") != NULL) {
		index++;
	}

	CPU_amount = index;
	return (index == 0) ? false : true;
}
size_t Get_CPU(void) {
	return CPU_amount;
}
//-----------------------------------------------