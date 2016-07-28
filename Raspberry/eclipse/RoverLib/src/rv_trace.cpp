/*
 * rv_trace.cpp
 *
 *  Created on: Jul 15, 2016
 *      Author: walberts
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rv.h"
#include "rv_reg.h"
#include "rv_log.h"

static REG_map* tr_buffer;
static int tr_bufferIndex;
static int tr_bufferSize;

static int TR_dumpBuffer(FILE* of, int idx);

extern int TR_setup(int size) {
	int result = OK;
	LG_logEntry(__func__, "size: %d", size);

	tr_bufferIndex = 0;
	tr_bufferSize = size;

	tr_buffer = (REG_map*)calloc(size, sizeof(REG_map));
	if (tr_buffer == NULL) {
		result = RV_UNABLE_TO_MALLOC;
	}

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int TR_traceRegmap() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	REG_readAll(tr_buffer + tr_bufferIndex);
	tr_bufferIndex = (1 + tr_bufferIndex) % tr_bufferSize;

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int TR_dumpBuffers(FILE* of) {
	int result = OK;
	LG_logEntry(__func__, "of: %p", of);

	for (int i = 0; (result == OK) && (i < REG_MAX); i++) {
		fprintf(of, "%s\t", REG_getRegistername(i));
	}
	fprintf(of,"\n");

	int idx = (tr_bufferIndex + 1) % tr_bufferSize;
	do
	{
		result = TR_dumpBuffer(of, idx);

		idx = (idx + 1) % tr_bufferSize;
	} while ((result == OK) && (idx != tr_bufferIndex));

	LG_logExit(__func__, result, NULL);
	return result;
}

static int TR_dumpBuffer(FILE* of, int idx) {
	int result = OK;

	long v;
	REG_map* src = tr_buffer + idx;
	for (int i = 0; (result == OK) && (i < REG_MAX); i++) {
		result = REG_readLong(src, idx, &v);
		if (result == OK)
			fprintf(of, "%ld",  v);
	}
	fprintf(of,"\n");

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_dumpBuffersToFile(const char* name) {
	int result = OK;
	LG_logEntry(__func__, "name: %s", name);
	FILE* of;

	of = fopen(name, "a+");
	if (of != NULL) {
		TR_dumpBuffers(of);
		fclose(of);
	} else {
		result = RV_UNABLE_TO_OPEN_FILE;
	}

	LG_logExit(__func__, result, NULL);
	return result;
}
