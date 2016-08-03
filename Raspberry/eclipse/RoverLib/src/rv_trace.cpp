/*
 * rv_trace.cpp
 *
 *  Created on: Jul 15, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */

/*
 * ---------------------------------------------------------------------------
 *               Includes
 * ---------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rv.h"
#include "rv_reg.h"
#include "rv_log.h"

/*
 * ---------------------------------------------------------------------------
 *               Static module data
 * ---------------------------------------------------------------------------
 */

/* Pointer to memory area holding trace records */
static REG_map* tr_buffer;

/* Index of where to write next trace rcord in buffer */
static int tr_bufferIndex;

/* Size of trace buffer, i.e. nr. records that can be
 * writte before buffer starts overwriting oldest records.
 */
static int tr_bufferSize;

/* Boolean indicating whether the buffer has overrun or not.
 * This one is used while writing buffers to file in order to
 * avoid writing empty records.
 */
static bool tr_bufferOverrun;

/*
 * ---------------------------------------------------------------------------
 *               Static function declarations.
 * ---------------------------------------------------------------------------
 */

static int tr_dumpBuffer(FILE* of, int idx);

/*
 * ---------------------------------------------------------------------------
 *               Module implementation
 * ---------------------------------------------------------------------------
 */

extern int TR_setup(int size) {
	int result = OK;
	LG_logEntry(__func__, "size: %d", size);

	/* Initialize administration */
	tr_bufferIndex = 0;
	tr_bufferSize = size;
	tr_bufferOverrun = false;

	/* Allocate memory for records */
	tr_buffer = (REG_map*)calloc(size, sizeof(REG_map));
	if (tr_buffer == NULL) {
		result = RV_UNABLE_TO_MALLOC;
	}

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int TR_traceRegmap() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	/* Retrieve current register map and store it
	 * in current trace record.
	 */
	REG_readAll(tr_buffer + tr_bufferIndex);

	/* Update administration, advance bufferIndex
	 * and administrate whether buffer has overrun.
	 */
	tr_bufferOverrun |= (1+tr_bufferIndex==tr_bufferSize);
	tr_bufferIndex = (1 + tr_bufferIndex) % tr_bufferSize;

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int TR_dumpBuffers(FILE* of) {
	int result = OK;
	int idx;
	LG_logEntry(__func__, "of: %p", of);

	/* Write header line with registernames as legend */
	for (int i = 0; (result == OK) && (i < REG_MAX); i++) {
		fprintf(of, "%s\t", REG_getRegistername(i));
	}
	fprintf(of,"\n");

	/* Determine index of oldest record. If no
	 * overrun occurred, index is zero, otherwise
	 * it is one higher than the current index.
	 */
	if (tr_bufferOverrun ) {
	    idx = (tr_bufferIndex + 1) % tr_bufferSize;
	}
	else {
	    idx=0;
	}

	/* Write all records to file */
	do
	{
		result = tr_dumpBuffer(of, idx);

		idx = (idx + 1) % tr_bufferSize;
	} while ((result == OK) && (idx != tr_bufferIndex));

	/* Reset administration in order to avoid
	 * writing trace records twice.
	 */
    tr_bufferIndex = 0;
    tr_bufferOverrun = false;

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

static int tr_dumpBuffer(FILE* of, int idx) {
	int result = OK;
    long v;

	/* Determine location of trace record to write to file
	 */
	REG_map* src = tr_buffer + idx;

	/* Write all registers be reading them as a long
	 * and writing them to file.
	 */
	for (int i = 0; (result == OK) && (i < REG_MAX); i++) {
		result = REG_readLong(src, i, &v);
		if (result == OK)
			fprintf(of, "%ld\t",  v);
	}
	/* Write EOL in order to ensure that next
	 * record ends up on next line.
	 */
	fprintf(of,"\n");

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int RV_dumpBuffersToFile(const char* name) {
	int result = OK;
	LG_logEntry(__func__, "name: %s", name);
	FILE* of;

	/* Open file in append made */
	of = fopen(name, "a+");
	if (of != NULL) {
	    /* Write records */
		TR_dumpBuffers(of);
		fclose(of);
	} else {
	    /* Report error */
		result = RV_UNABLE_TO_OPEN_FILE;
	}

	LG_logExit(__func__, result, NULL);
	return result;
}
