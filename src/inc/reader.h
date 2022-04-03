#ifndef _READER_H
#define _READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unitstd.h>

#include "general.h"

#define READER_FILE_LEN			((size_t)512)

void reader_new(void);
bool reader_get_cpu_info(char* raw_data);

#endif // !_READER_H
