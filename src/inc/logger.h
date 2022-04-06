#ifndef _LOGGER_H
#define _LOGGER_H

#include <string.h>
#include <stdbool.h>
#include <time.h>

#define FILE_DESCRIPTOR_LEN			(size_t)128
#define LOGGER_MES_LEN				(size_t)512

typedef enum {
	Reader_got_proc_stat,
	Reader_got_not_proc_stat,
	Reader_sent_raw_data,
	Analyzer_got_raw_data,
	Analyzer_sent_converted_data,
	Printer_got_converted_data,
	Printer_printed_converted_data,
}Logger_enum;

void logger_init(char* file_name);
char* logger_format_string(Logger_enum mes, char* str);
bool logger_log(char* str);

#endif // !_LOGGER_H
