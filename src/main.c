#include "main.h"

// Threads
static thrd_t reader_thread;
static thrd_t analyzer_thread;
static thrd_t printer_thread;
static thrd_t watchdog_thread;
static thrd_t logger_thread;

// Queues
static Queue* q_rawData;
static Queue* q_analyzedData;

// Mutex
static mtx_t mtx_rawData;
static mtx_t mtx_analyzedData;

// Status flags
static atomic_bool terminate_signal;
static atomic_bool watchdog_reader;
static atomic_bool watchdog_analyzer;
static atomic_bool watchdog_printer;

int main(int argc, char* argv[]) {
	// Get 
	if (CPU_amount_init() == false) {
		fputs(CPU_UNDEFINED, stderr);
		return -1;
	}

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

	// Mutex init
	mtx_init(&mtx_rawData, mtx_plain);
	mtx_init(&mtx_analyzedData, mtx_plain);

	// Init threads
	if (thrd_create(&reader_thread, ReaderThreadFunc, NULL) != thrd_success ||
		thrd_create(&analyzer_thread, AnalyzerThreadFunc, NULL) != thrd_success ||
		thrd_create(&printer_thread, PrinterThreadFunc, NULL) != thrd_success ||
		//thrd_create(&watchdog_thread, WatchdogThreadFunc, NULL) != thrd_success ||
		//thrd_create(&logger_thread, LoggerThreadFunc, NULL) != thrd_success) {

		fputs(THREAD_CREATE_ERR, stderr);
		FreeAllResources();

		return -1;
	}

	// Wait untill threads are done
	thrd_join(reader_thread, NULL);
	thrd_join(analyzer_thread, NULL);
	thrd_join(printer_thread, NULL);
	//thrd_join(watchdog_thread, NULL);
	//thrd_join(logger_thread, NULL);

	FreeAllResources();

	// The end
	return 0;
}

void terminate_handler(int signum, siginfo_t* info, void* ptr) {
	fputs(SIGTERM_MSG, stderr);
	atomic_store(&terminate_signal, SIG_TERM_TRUE);
}

int ReaderThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		atomic_store(&watchdog_reader, WATCHDOG_FALSE);

		if (Queue_isFull(q_rawData) == true) {
			continue;
		}

		char raw_data[READER_FILE_LEN];
		if (reader_get_cpu_info(raw_data, READER_FILE_LEN) == true) {
			Queue_add(q_rawData, (void*)raw_data);
		}
	}

	return 0;
}
int AnalyzerThreadFunc(void* thread_data) {
	char* raw_data;
	InfoCPU* icpu = InfoCPU_new(Get_CPU());
	InfoCPU* icpu_old = InfoCPU_new(Get_CPU());

	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		atomic_store(&watchdog_analyzer, WATCHDOG_FALSE);

		if (Queue_isEmpty(q_rawData) == true) {
			continue;
		}
		if ((raw_data = (char *)Queue_pop(q_rawData)) == NULL) {
			continue;
		}

		char str_analyzedData[ANALYZER_STR_LEN];
		analyzer_parse_raw_data(icpu, raw_data);
		analyzer_calculate_cpu_usage(icpu_old, icpu, str_analyzedData, ANALYZER_STR_LEN);
		Queue_add(q_analyzedData, (void*)str_analyzedData);

		InfoCPU_copy(icpu_old, icpu);
	}

	InfoCPU_free(icpu);
	InfoCPU_free(icpu_old);

	return 0;
}
int PrinterThreadFunc(void* thread_data) {
	char* analyzedData;

	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		atomic_store(&watchdog_printer, WATCHDOG_FALSE);

		if (Queue_isEmpty(q_analyzedData) == true) {
			continue;
		}
		if ((analyzedData = (char*)Queue_pop(q_analyzedData)) == NULL) {
			continue;
		}
		
		fputs(analyzedData, stdout);
	}
	
	return 0;
}
int WatchdogThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		if (atomic_load(&watchdog_reader) == WATCHDOG_TRUE || 
			atomic_load(&watchdog_analyzer) == WATCHDOG_TRUE || 
			atomic_load(&watchdog_printer) == WATCHDOG_TRUE) {

			atomic_store(&terminate_signal, SIG_TERM_TRUE);
			fputs(WATCHDOG_MSG, stderr);

			thrd_exit(0);
		}

		atomic_store(&watchdog_reader, WATCHDOG_TRUE);
		atomic_store(&watchdog_analyzer, WATCHDOG_TRUE);
		atomic_store(&watchdog_printer, WATCHDOG_TRUE);

		thrd_sleep(&(struct timespec) {.tv_sec = 2}, NULL);
	}
	
	return 0;
}
int LoggerThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		
	}
	
	return 0;
}

void FreeAllResources(void) {
	Queue_free(q_rawData);
	Queue_free(q_analyzedData);

	mtx_destroy(&mtx_rawData);
	mtx_destroy(&mtx_analyzedData);
}
