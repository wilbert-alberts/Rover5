/*
 * rv_reg.cpp - Realizing operations on the register map
 *
 *
 * In order to allow efficient reading/writing of registers, a map
 * is calculated translating each register index into a pointer to
 * the memory location of the current REG_map. This map is initialized
 * by REG_setup.
 *
 *
 *  Created on: Jul 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */

/*
 * ---------------------------------------------------------------------------
 *               Includes
 * ---------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdint.h>
#include <semaphore.h>
#include <strings.h>

#include "rv.h"
#include "rv_log.h"
#include "rv_reg.h"

/*
 * ---------------------------------------------------------------------------
 *               Defines
 * ---------------------------------------------------------------------------
 */
#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) \
			r = c; \
	}

/*
 * REG_ADDREGISTER adds a register to the administration. It stores the
 * address of the data w.r.t. the current register map and it records
 * the name of the register in the map of registernames.
 */
#define REG_ADDREGISTER(R) 							\
  reg_address[REG_ ## R] = (uint8_t*) &reg_map.R; 	\
  reg_name[REG_ ## R] = #R;

/*
 * The LOG macros below allow type dependent logging of register
 * values to stdout. They are used by REG_logAll
 */
#define LOG_U32(S, R) \
{\
    uint32_t* v = (uint32_t*) (&(S->R));        \
    printf("%s: 0x%0x\n", reg_name[REG_ ## R], *v); \
}

#define LOG_32(S, R) \
{\
    int32_t* v = (int32_t*) (&(S->R));       \
    printf("%s: 0x%0x\n", reg_name[REG_ ## R], *v); \
}

#define LOG_U16(S, R) \
{\
    uint16_t* v = (uint16_t*) (&(S->R));        \
    printf("%s: 0x%0x\n", reg_name[REG_ ## R], *v); \
}

#define LOG_U8(S, R) \
{\
    uint8_t* v = (uint8_t*) (&(S->R));       \
    printf("%s: 0x%0x\n", reg_name[REG_ ## R], *v); \
}

/*
 * ---------------------------------------------------------------------------
 *               Static module data
 * ---------------------------------------------------------------------------
 */

static REG_map reg_map;   // The current register map
static sem_t reg_sem;     // Semaphore protecting access to current register map
static uint8_t* reg_address[REG_MAX];  // Map with register locations
static const char* reg_name[REG_MAX];  // Map with register names

/*
 * ---------------------------------------------------------------------------
 *               Module implementation
 * ---------------------------------------------------------------------------
 */

extern int REG_setup() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	/* Fill the registermap with zeros and initialize
	 * the protection semaphore
	 */
	bzero(&reg_map, sizeof(reg_map));
	SAFE_INVOKE(sem_init(&reg_sem, 0, 1), result, RV_SEM_INIT_FAILED)

	/* Initialize the name and location maps */
	if (result == OK) {
		REG_ADDREGISTER(HEADER)
		REG_ADDREGISTER(MICROS)
		REG_ADDREGISTER(MILLIS)
		REG_ADDREGISTER(LEFTDIR)
		REG_ADDREGISTER(LEFTDC)
		REG_ADDREGISTER(RIGHTDIR)
		REG_ADDREGISTER(RIGHTDC)
		REG_ADDREGISTER(LEFTPOS)
		REG_ADDREGISTER(RIGHTPOS)
		REG_ADDREGISTER(AMB_COL_NE)
		REG_ADDREGISTER(AMB_COL_SE)
		REG_ADDREGISTER(AMB_COL_SW)
		REG_ADDREGISTER(AMB_COL_NW)
		REG_ADDREGISTER(AMB_LINE_N)
		REG_ADDREGISTER(AMB_LINE_E)
		REG_ADDREGISTER(AMB_LINE_S)
		REG_ADDREGISTER(AMB_LINE_W)
		REG_ADDREGISTER(IR_COL_NE)
		REG_ADDREGISTER(IR_COL_SE)
		REG_ADDREGISTER(IR_COL_SW)
		REG_ADDREGISTER(IR_COL_NW)
		REG_ADDREGISTER(IR_LINE_N)
		REG_ADDREGISTER(IR_LINE_E)
		REG_ADDREGISTER(IR_LINE_S)
		REG_ADDREGISTER(IR_LINE_W)
		REG_ADDREGISTER(TRAILER)
	}

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 * Read/Write operations
 *
 * Each read and write operations tries to wait for the protection
 * semaphore before actually reading/writing a registers. Obviously
 * the semaphore is released before exiting the function.
 *
 * Each access consists of determining the memory location, casting
 * it to a destination suitable to hold the data type (8, 16 or 32 bit),
 * and then reading/writing into that location.
 */

extern int REG_write8(int id, uint8_t val) {
	int result = OK;
	LG_logEntry(__func__, "id: %d, val: %d", id, val);

	uint8_t* dst = (uint8_t*) (reg_address[id]);

	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int REG_write16(int id, uint16_t val) {
	int result = OK;
	LG_logEntry(__func__, "id: %d, val: %d", id, val);

	uint16_t* dst = (uint16_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int REG_write32(int id, int32_t val) {
	int result = OK;
	LG_logEntry(__func__, "id: %d, val: %d", id, val);

	int32_t* dst = (int32_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int REG_read8(int id, uint8_t* val) {
	int result = OK;
	LG_logEntry(__func__, "id: %d, val: %p", id, val);

	uint8_t* src = (uint8_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, "*val: %d", *val);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int REG_read16(int id, uint16_t* val) {
	int result = OK;
	LG_logEntry(__func__, "id: %d, val: %p", id, val);

	uint16_t* src = (uint16_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, "*val: %d", *val);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int REG_read32(int id, int32_t* val) {
	int result = OK;
	LG_logEntry(__func__, "id: %d, val: %p", id, val);

	int32_t* src = (int32_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, "*val: %d", *val);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 *
 * REG_readLong reads an arbitrary register and stores the result into a
 * long variable. Note that this operation contains a switch statement
 * making it unsuitable for regular usage inside a time critical section.
 *
 * As this operation works on a registermap represented by 'src', the
 * regular map with memory locations per register doesn't work here.
 * Instead, the map is used to determine an offset w.r.t. the start
 * address of the memory structure. Then, this offset is used in
 * combination with 'src' in order to determine the memory location
 * holding the value to be retrieved.
 */

extern int REG_readLong(REG_map* src, int id, long* val) {
	int result = OK;
	LG_logEntry(__func__, "id: %d, val: %p", id, val);

	int32_t* v32;
	uint16_t* v16;
	uint8_t* v8;
	uint8_t* s = (uint8_t*)src;
	int offset;

	switch (id) {
	case REG_HEADER:
	case REG_MICROS:
	case REG_MILLIS:
	case REG_TRAILER:
	case REG_LEFTPOS:
	case REG_RIGHTPOS:
		// 32 bit
		offset = reg_address[id] -  ((uint8_t*)&reg_map);
		v32 = (int32_t*) &s[offset];
		*val = (long) *v32;
		break;

	case REG_AMB_LINE_N:
	case REG_AMB_LINE_E:
	case REG_AMB_LINE_S:
	case REG_AMB_LINE_W:
	case REG_AMB_COL_NE:
	case REG_AMB_COL_SE:
	case REG_AMB_COL_SW:
	case REG_AMB_COL_NW:
	case REG_IR_LINE_N:
	case REG_IR_LINE_E:
	case REG_IR_LINE_S:
	case REG_IR_LINE_W:
	case REG_IR_COL_NE:
	case REG_IR_COL_SE:
	case REG_IR_COL_SW:
	case REG_IR_COL_NW:
		// 16bit
		offset = reg_address[id] -  ((uint8_t*)&reg_map);
		v16 = (uint16_t*) &s[offset];
		*val = (long) *v16;
		break;

	case REG_LEFTDIR:
	case REG_LEFTDC:
	case REG_RIGHTDIR:
	case REG_RIGHTDC:
		offset = reg_address[id] -  ((uint8_t*)&reg_map);
		v8 = (uint8_t*)&s[offset];
		*val = (long) *v8;
		break;
	default:
		break;
	}

	LG_logExit(__func__, result, "*val: %d", *val);
	return result;

}

/*
 * ---------------------------------------------------------------------------
 *
 * REG_readAll, copy the current registermap into dst
 */

extern int REG_readAll(REG_map* dst) {
	int result = OK;
	LG_logEntry(__func__, "dst: %p", dst);

	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = reg_map;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 *
 * REG_writeAll, copy 'src' into the current registermap
 */

extern int REG_writeAll(REG_map* src) {
	int result = OK;
	LG_logEntry(__func__, "src: %p", src);

	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	reg_map = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern const char* REG_getRegistername(int idx) {
	if ((idx >= 0) && (idx < REG_MAX)) {
		return reg_name[idx];
	}
	return NULL;
}

/*
 * ---------------------------------------------------------------------------
 */

extern void REG_logAll(REG_map* src) {
	printf("reg_map: \n");
	LOG_U32(src, HEADER);
	LOG_U32(src, MICROS);
	LOG_U32(src, MILLIS);
	LOG_U8(src, LEFTDIR);
	LOG_U8(src, LEFTDC);
	LOG_U8(src, RIGHTDIR);
	LOG_U8(src, RIGHTDC);
	LOG_32(src, LEFTPOS);
	LOG_32(src, RIGHTPOS);
	LOG_U16(src, AMB_LINE_N);
	LOG_U16(src, AMB_LINE_E);
	LOG_U16(src, AMB_LINE_S);
	LOG_U16(src, AMB_LINE_W);
	LOG_U16(src, AMB_COL_NE);
	LOG_U16(src, AMB_COL_SE);
	LOG_U16(src, AMB_COL_SW);
	LOG_U16(src, AMB_COL_NW);
	LOG_U16(src, IR_LINE_N);
	LOG_U16(src, IR_LINE_E);
	LOG_U16(src, IR_LINE_S);
	LOG_U16(src, IR_LINE_W);
	LOG_U16(src, IR_COL_NE);
	LOG_U16(src, IR_COL_SE);
	LOG_U16(src, IR_COL_SW);
	LOG_U16(src, IR_COL_NW);
	LOG_U32(src, TRAILER);
}

