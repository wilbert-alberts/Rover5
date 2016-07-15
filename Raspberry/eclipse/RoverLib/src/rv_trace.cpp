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

static REG_map* tracebuffer;
static int tracebufferIndex;
static int tracebufferSize;

static int TR_dumpBuffer(FILE* of, int idx);

extern int TR_setup(int size) {
	int result = OK;
	RV_LogEntry(__func__, "size: %d", size);

	tracebufferIndex = 0;
	tracebufferSize = size;

	tracebuffer = (REG_map*)calloc(size, sizeof(REG_map));
	if (tracebuffer == NULL) {
		result = RV_UNABLE_TO_MALLOC;
	}

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int TR_traceRegmap() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	REG_readAll(tracebuffer + tracebufferIndex);
	tracebufferIndex = (1 + tracebufferIndex) % tracebufferSize;

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int TR_dumpBuffers(FILE* of) {
	int result = OK;
	RV_LogEntry(__func__, "of: %p", of);

	int idx = (tracebufferIndex + 1) % tracebufferSize;
	do
	{
		result = TR_dumpBuffer(of, idx);

		idx = (idx + 1) % tracebufferSize;
	} while ((result == OK) && (idx != tracebufferIndex));

	RV_LogExit(__func__, result, NULL);
	return result;
}

static int TR_dumpBuffer(FILE* of, int idx) {
	int result = OK;

	long v;
	REG_map* src = tracebuffer + idx;
	for (int i = 0; (result == OK) && (i < REG_MAX); i++) {
		result = REG_readLong(src, idx, &v);
		if (result == OK)
			fprintf(of, "%s\t%ld\n", REG_getRegistername(idx), v);
	}

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_dumpBuffersToFile(const char* name) {
	int result = OK;
	RV_LogEntry(__func__, "name: %s", name);
	FILE* of;

	of = fopen(name, "a+");
	if (of != NULL) {
		TR_dumpBuffers(of);
		fclose(of);
	} else {
		result = RV_UNABLE_TO_OPEN_FILE;
	}

	RV_LogExit(__func__, result, NULL);
	return result;
}
