#include "main.h"

// Threads
thrd_t reader_thread;
thrd_t analyzer_thread;
thrd_t printer_thread;
thrd_t watchdog_thread;
thrd_t logger_thread;

// Queues
Queue* q_rawData;

// Status flags
static atomic_bool terminate_signal;

int main(int argc, char* argv[]) {
	// Get ID of main thread
	int pid = getpid();
	int parentID = getppid();
	printf("PID is %d, parent PID is %d\n", pid, parentID);

	// Set all flags
	atomic_init(&terminate_signal, SIG_TERM_FALSE);

	// Add termination after signal
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_sigaction = terminate_handler;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &action, NULL);

	// Init queues
	q_rawData = Queue_new(1000);

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

	// The end
	return 0;
}

void terminate_handler(int signum, siginfo_t* info, void* ptr) {
	write(STDERR_FILENO, SIGTERM_MSG, sizeof(SIGTERM_MSG));
	atomic_store(&terminate_signal, SIG_TERM_TRUE);
}

int ReaderThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {
		if (Queue_isFull(q_rawData) == true) {
			continue;
		}

		char raw_data[READER_FILE_LEN];
		if (reader_get_cpu_info(raw_data) == true) {
			Queue_add(q_rawData, (void*)raw_data);
		}
	}
	//thrd_exit(0);
	return 0;
}
int AnalyzerThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {

	}
	//thrd_exit(0);
	return 0;
}
int PrinterThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {

	}
	//thrd_exit(0);
	return 0;
}
int WatchdogThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {

	}
	//thrd_exit(0);
	return 0;
}
int LoggerThreadFunc(void* thread_data) {
	while (atomic_load(&terminate_signal) == SIG_TERM_FALSE) {

	}
	//thrd_exit(0);
	return 0;
}
