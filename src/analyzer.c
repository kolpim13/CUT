#include "analyzer.h"

InfoCPU* InfoCPU_new(size_t cpu_amount) {
	InfoCPU* icpu = (InfoCPU*)malloc(sizeof(InfoCPU));
	icpu->user = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->nice = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->system = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->idle = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->iowait = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->irq = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->softirq = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->steal = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->guest = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);
	icpu->guest_nice = (CPU_TYPE*)malloc(sizeof(CPU_TYPE) * cpu_amount);

	// Init all with zero value
	for (size_t i = 0; i < cpu_amount; i++) {
		icpu->user[i] = 0;
		icpu->nice[i] = 0;
		icpu->system[i] = 0;
		icpu->idle[i] = 0;
		icpu->iowait[i] = 0;
		icpu->irq[i] = 0;
		icpu->softirq[i] = 0;
		icpu->steal[i] = 0;
		icpu->guest[i] = 0;
		icpu->guest_nice[i] = 0;
	}

	return icpu;
}
void InfoCPU_free(InfoCPU* icpu) {
	free(icpu->user);
	free(icpu->nice);
	free(icpu->system);
	free(icpu->idle);
	free(icpu->iowait);
	free(icpu->irq);
	free(icpu->softirq);
	free(icpu->steal);
	free(icpu->guest);
	free(icpu->guest_nice);
	free(icpu);
}

void analyzer_parse_raw_data(InfoCPU* icpu, char* raw_data) {
	char* token;
	const char* delim = "\n";
	size_t index = 0;

	token = strtok(raw_data, delim);
	while (token != NULL) {
		_analyzer_parse_raw_data_single(icpu, token, index);
		token = strtok(NULL, delim);
		index++;
	}
}
static void _analyzer_parse_raw_data_single(InfoCPU* icpu, char* data_single_cpu, size_t index) {
	char str[16];

	sscanf(data_single_cpu, "%s %I64u %I64u %I64u %I64u %I64u %I64u %I64u %I64u %I64u %I64u", str,
		&icpu->user[index], &icpu->nice[index], &icpu->system[index], &icpu->idle[index], &icpu->iowait[index],
		&icpu->irq[index], &icpu->softirq[index], &icpu->steal[index], &icpu->guest[index], &icpu->guest_nice[index]);
}

bool analyzer_calculate_cpu_usage(InfoCPU* icpu_prev, InfoCPU* icpu, char* str_out, size_t len_max) {
	size_t cpu_amount = Get_CPU();

	CPU_TYPE idle[cpu_amount];
	CPU_TYPE idle_prev[cpu_amount];
	CPU_TYPE non_idle[cpu_amount];
	CPU_TYPE non_idle_prev[cpu_amount];
	CPU_TYPE total[cpu_amount];
	CPU_TYPE total_prev[cpu_amount];
	CPU_TYPE totald[cpu_amount];
	CPU_TYPE idled[cpu_amount];
	float usage;

	char* str_out_begin = str_out;
	size_t len = 0;
	for (size_t i = 0; i < cpu_amount; i++) {
		idle[i] = icpu->idle[i] + icpu->iowait[i];
		idle_prev[i] = icpu_prev->idle[i] + icpu_prev->iowait[i];

		non_idle[i] = icpu->user[i] + icpu->nice[i] + icpu->system[i] + icpu->irq[i] + icpu->softirq[i] + icpu->steal[i];
		non_idle_prev[i] = icpu_prev->user[i] + icpu_prev->nice[i] + icpu_prev->system[i] + icpu_prev->irq[i] + icpu_prev->softirq[i] + icpu_prev->steal[i];

		total[i] = idle[i] + non_idle[i];
		total_prev[i] = idle_prev[i] + non_idle_prev[i];

		totald[i] = total[i] - total_prev[i];
		idled[i] = idle[i] - idle_prev[i];

		usage = (totald[i] - idled[i]) / totald[i];

		// Add into string
		int t = sprintf(str_out + len, len_max - len, "%.2f", usage);
		if (t == -1) {
			return false;
		}
		len += t;
	}

	return true;
}

