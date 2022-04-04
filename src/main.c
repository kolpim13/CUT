#include "main.h"

// Threads
thrd_t reader_thread;
thrd_t analyzer_thread;
thrd_t printer_thread;
thrd_t watchdog_thread;
thrd_t logger_thread;

// Queues
Queue* q_rawData;
Queue* q_analyzedData;

// Status flags
static atomic_bool terminate_signal;
static atomic_bool watchdog_reader;
static atomic_bool watchdog_analyzer;
static atomic_bool watchdog_printer;

int main(int argc, char* argv[]) {
	// Get ID of main thread
	int pid = getpid();
	int parentID = getppid();
	printf("PID is %d, parent PID is %d\n", pid, parentID);

	// Set all flags
	atomic_init(&terminate_signal, SIG_TERM_FALSE);
	atomic_init(&watchdog_reader, WATCHDOG_FALSE);
	atomic_init(&watchdog_analyzer, WATCHDOG_FALSE);
	atomic_init(&watchdog_printer, WATCHDOG_FALSE);

	// Add termination after signal
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_sigaction = terminate_handler;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &action, NULL);

	// Init queues
	q_rawData = Queue_new(1000);
	q_analyzedData = Queue_new(1000);

	// Init threads
	thrd_create(&reader_thread, ReaderThreadFunc, NULL);
	thrd_create(&analyzer_thread, ReaderThreadFunc, NULL);
	thrd_create(&printer_thread, ReaderThreadFunc, NULL);
	thrd_create(&watchdog_thread, ReaderThreadFunc, NULL);
	thrd_create(&logger_thread, ReaderThreadFunc, NULL);

	// Wait untill threads are done
	thrd_join(reader_thread, NULL);
	thrd_join(analyzer_thread, NULL);
	thrd_join(printer_thread, NULL);
	thrd_join(watchdog_thread, NULL);
	thrd_join(logger_thread, NULL);

	// Free all allocated resources
	Queue_free(q_rawData);
	Queue_free(q_analyzedData);

	// The end
	return 0;
}

void terminate_handler(int signum, siginfo_t* info, void* ptr) {
	write(STDERR_FILENO, SIGTERM_MSG, sizeof(SIGTERM_MSG));
	atomic_store(&terminate_signal, SIG_TERM_TRUE);
}

int ReaderThreadFunc(void* thread_data) {
	atomic_store(&watchdog_reader, WATCHDOG_FALSE);

	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		if (Queue_isFull(q_rawData) == true) {
			continue;
		}

		char raw_data[READER_FILE_LEN];
		if (reader_get_cpu_info(raw_data) == true) {
			Queue_add(q_rawData, (void*)raw_data);
		}
	}

	return 0;
}
int AnalyzerThreadFunc(void* thread_data) {
	char* raw_data;
	InfoCPU icpu = InfoCPU_new(Get_CPU());
	InfoCPU icpu_old = InfoCPU_new(Get_CPU());

	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		atomic_store(&watchdog_analyzer, WATCHDOG_FALSE);

		if (Queue_isEmpty(q_rawData) == true) {
			continue;
		}

		if ((raw_data = Queue_pop(q_rawData)) == NULL) {
			continue;
		}

		char str_analyzedData[128];
		analyzer_parse_raw_data(icpu, raw_data);
		analyzer_calculate_cpu_usage(icpu_old, icpu, str_analyzedData, 128);
		Queue_add(q_analyzedData, (void*)str_analyzedData);
	}

	InfoCPU_free(icpu);
	InfoCPU_free(icpu_old);

	return 0;
}
int PrinterThreadFunc(void* thread_data) {
	atomic_store(&watchdog_printer, WATCHDOG_FALSE);

	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {

	}
	
	return 0;
}
int WatchdogThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		if (atomic_load(&watchdog_reader) == WATCHDOG_TRUE || 
			atomic_load(&watchdog_analyzer) == WATCHDOG_TRUE || 
			atomic_load(&watchdog_printer) == WATCHDOG_TRUE) {
			atomic_store(&terminate_signal, SIG_TERM_TRUE);
			write(STDERR_FILENO, WATCHDOG_MSG, sizeof(WATCHDOG_MSG));
		}

		atomic_store(&watchdog_reader, WATCHDOG_TRUE);
		atomic_store(&watchdog_analyzer, WATCHDOG_TRUE);
		atomic_store(&watchdog_printer, WATCHDOG_TRUE);

		sleep(2);
	}
	
	return 0;
}
int LoggerThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		
	}
	
	return 0;
}
