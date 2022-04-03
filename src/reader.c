#include "reader.h"

bool reader_get_cpu_info(char* raw_data, size_t raw_data_len) {
	FILE* fp;
	size_t len = 0;
	char* str;
	if (fp = fopen("/proc/stat", "r") == NULL) {
		return false;
	}

	for (;;) {
		str = raw_data + len;
		if (str = fgets(temp, READER_FILE_LEN - len, fp) == NULL) {
			break;
		}
		if (strstr(str, "cpu") == NULL) {
			break;
		}
		len = str - raw_data;
	}
	raw_data[len] = '\0';
	fclose(fp);
	
	return (len == 0) ? false : true;
}
