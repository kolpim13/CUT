#include "general.h"

Queue* Queue_new(size_t length) {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->data = (void**)malloc(length * sizeof(void*));
	q->length = length;
	q->amount = 0;
	q->pos = 0;
	mtx_init(&q->mutex, mtx_plain);
	return q;
}
void Queue_free(Queue* q) {
	free(q->data);
	mtx_destroy(q->mutex);
	free(q);
}

bool Queue_add(Queue* q, void* elem) {
	mtx_lock(&q->mutex);

	if (Queue_isFull(q) == true) {
		return false;
	}

	size_t pos = q->amount + q->pos;
	if (pos > q->length) {
		pos -= q->length;
	}

	q->data[pos] = elem;
	q->amount++;

	mtx_unlock(&q->mutex);

	return true;
}
void* Queue_getLast(Queue* q) {
	mtx_lock(&q->mutex);
	if (Queue_isEmpty(q) == true) {
		return NULL;
	}
	mtx_unlock(&q->mutex);

	return q->data[q->pos];
}
void* Queue_pop(Queue* q) {
	mtx_lock(&q->mutex);

	if (Queue_isEmpty(q) == true) {
		return NULL;
	}

	size_t temp = q->pos;
	q->amount--;
	q->pos++;
	if (q->pos >= q->length) {
		q->pos = 0;
	}

	mtx_unlock(&q->mutex);

	return q->data[temp];
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
	fclose(fp);

	CPU_amount = index;
	return (index == 0) ? false : true;
}
size_t Get_CPU(void) {
	return CPU_amount;
}
//-----------------------------------------------
