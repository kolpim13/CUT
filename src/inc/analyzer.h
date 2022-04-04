#ifndef _ANALYZER_H
#define _ANALYZER_H

#include <stdbool.h>

#include "general.h"

#define CPU_TYPE		(unsigned long long int)

typedef struct _InfoCPU {
	CPU_TYPE* user;
	CPU_TYPE* nice;
	CPU_TYPE* system;
	CPU_TYPE* idle;
	CPU_TYPE* iowait;
	CPU_TYPE* irq;
	CPU_TYPE* softirq;
	CPU_TYPE* steal;
	CPU_TYPE* guest;
	CPU_TYPE* guest_nice;
}InfoCPU;

InfoCPU* InfoCPU_new(size_t cpu_amount);
void InfoCPU_free(InfoCPU* icpu);

bool analyzer_parse_raw_data(InfoCPU* icpu, char* raw_data);
char* analyzer_calculate_cpu_usage(InfoCPU* icpu_prev, InfoCPU* icpu, char* str_out, size_t len_max);

#endif // !_ANALYZER_H
