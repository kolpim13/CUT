#include "reader.h"

bool reader_get_cpu_info(char* raw_data, size_t raw_data_len) {
	FILE* fp = fopen("/proc/stat", "r");
	size_t len = 0;
	char* str;
	if (fp == NULL) {
		return false;
	}

	for (;;) {
		str = raw_data + len;
		if ((str = fgets(str, raw_data_len - len, fp)) == NULL) {
			break;
		}
		if (strstr(str, "cpu") == NULL) {
			break;
		}
		len += strlen(str);
		if (len > raw_data_len) {
			len = 0;
			break;
		}
	}
	raw_data[len] = '\0';
	fclose(fp);
	
	return (len == 0) ? false : true;
}
