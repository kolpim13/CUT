#include "logger.h"

static char descriptor[FILE_DESCRIPTOR_LEN] = "log.txt";

const static char Reader_gpc[] = "Reader thread has got data from the proc\\stat file\r\n";
const static char Reader_srd[] = "Reader thread sent raw data to the analyzer thread\r\n";
const static char Analyzer_gwd[] = "Analyzer thread has got raw data from the reader thread\r\n";
const static char Analyzer_scd[] = "Analyzer thread converted data and sent it to the printer thread\r\n";
const static char Printer_gcd[] = "Printer thread has got data from the analyzer thread\r\n";
const static char Printer_pcd[] = "Printer thread wrote converted data to the stdout\r\n";

void logger_init(char* file_name) {
	if (file_name == NULL) {
		return;
	}
}
char* logger_format_string(Logger_enum mes, char* str) {
	char* str_begin = str;

	time_t t;
	time(&t);

	char* temp;
	switch (mes) {
	case Reader_got_proc_stat: { temp = Reader_gpc; break; }
	case Reader_sent_raw_data: { temp = Reader_srd; break; }
	case Analyzer_got_raw_data: { temp = Analyzer_gwd; break; }
	case Analyzer_sent_converted_data: { temp = Analyzer_scd; break; }
	case Printer_got_converted_data: { temp = Printer_gcd; break; }
	case Printer_printed_converted_data: { temp = Printer_pcd; break; }
	}

	sprintf(str, "%s: %s", ctime(&t), temp);

	return str_begin;
}
bool logger_log(char* str) {
	FILE* fp = fopen(descriptor, "a");
	if (fp == NULL) {
		return false;
	}

	fprintf(str);

	fclose(fp);
	return true;
}
