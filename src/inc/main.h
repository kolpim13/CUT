#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <signal.h>
#include <unistd.h>

#include <threads.h>
#include <stdatomic.h>

#include "reader.h"
#include "analyzer.h"
#include "general.h"

// TERMINATION PROCESS
#define	SIG_TERM_FALSE	0
#define SIG_TERM_TRUE	1

#define WATCHDOG_FALSE	0
#define WATCHDOG_TRUE	1

#define SIGTERM_MSG			"SIGTERM signal occured\n"
#define WATCHDOG_MSG		"Watchdog signal occured\n"
#define CPU_UNDEFINED		"Impossible to define amount of cpu\n"
#deinfe THREAD_CREATE_ERR	"Error during threads creating\n"

void terminate_handler(int signum, siginfo_t* info, void* ptr);

//THREADS
#define THREAD_OK		0
#define THREAD_ERR		1

int ReaderThreadFunc(void* thread_data);
int AnalyzerThreadFunc(void* thread_data);
int PrinterThreadFunc(void* thread_data);
int WatchdogThreadFunc(void* thread_data);
int LoggerThreadFunc(void* thread_data);

void FreeAllResources(void);

#endif // !_MAIN_H
