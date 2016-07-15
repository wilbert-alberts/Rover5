#include <stdio.h>
#include <stdint.h>
#include <semaphore.h>
#include <strings.h>

#include "rv.h"
#include "rv_log.h"
#include "rv_reg.h"

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) \
			r = c; \
	}

#define REG_ADDREGISTER(R) 							\
  reg_address[REG_ ## R] = (uint8_t*) &reg_map.R; 	\
  reg_name[REG_ ## R] = #R;

static REG_map reg_map;
static sem_t reg_sem;
static uint8_t* reg_address[REG_MAX];
static const char* reg_name[REG_MAX];

extern int REG_setup() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	bzero(&reg_map, sizeof(reg_map));
	SAFE_INVOKE(sem_init(&reg_sem, 0, 1), result, RV_SEM_INIT_FAILED)

	if (result == OK) {
		REG_ADDREGISTER(HEADER)
		REG_ADDREGISTER(MICROS)
		REG_ADDREGISTER(MILLIS)
		REG_ADDREGISTER(LEFTDIR)
		REG_ADDREGISTER(LEFTDC)
		REG_ADDREGISTER(RIGHTDIR)
		REG_ADDREGISTER(RIGHTDC)
		REG_ADDREGISTER(COLLISION)
		REG_ADDREGISTER(LINE)
		REG_ADDREGISTER(LEFTPOS)
		REG_ADDREGISTER(RIGHTPOS)
		REG_ADDREGISTER(AMBOFFSET)
		REG_ADDREGISTER(AMB_COL_NE)
		REG_ADDREGISTER(AMB_COL_SE)
		REG_ADDREGISTER(AMB_COL_SW)
		REG_ADDREGISTER(AMB_COL_NW)
		REG_ADDREGISTER(AMB_LINE_NE)
		REG_ADDREGISTER(AMB_LINE_EN)
		REG_ADDREGISTER(AMB_LINE_ES)
		REG_ADDREGISTER(AMB_LINE_SE)
		REG_ADDREGISTER(AMB_LINE_SW)
		REG_ADDREGISTER(AMB_LINE_WS)
		REG_ADDREGISTER(AMB_LINE_WN)
		REG_ADDREGISTER(AMB_LINE_NW)
		REG_ADDREGISTER(IR_COL_NE)
		REG_ADDREGISTER(IR_COL_SE)
		REG_ADDREGISTER(IR_COL_SW)
		REG_ADDREGISTER(IR_COL_NW)
		REG_ADDREGISTER(IR_LINE_NE)
		REG_ADDREGISTER(IR_LINE_EN)
		REG_ADDREGISTER(IR_LINE_ES)
		REG_ADDREGISTER(IR_LINE_SE)
		REG_ADDREGISTER(IR_LINE_SW)
		REG_ADDREGISTER(IR_LINE_WS)
		REG_ADDREGISTER(IR_LINE_WN)
		REG_ADDREGISTER(IR_LINE_NW)
		REG_ADDREGISTER(TRAILER)
	}

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_write8(int id, uint8_t val) {
	int result = OK;
	RV_LogEntry(__func__, "id: %d, val: %d", id, val);

	uint8_t* dst = (uint8_t*) (reg_address[id]);

	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_write16(int id, uint16_t val) {
	int result = OK;
	RV_LogEntry(__func__, "id: %d, val: %d", id, val);

	uint16_t* dst = (uint16_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_write32(int id, int32_t val) {
	int result = OK;
	RV_LogEntry(__func__, "id: %d, val: %d", id, val);

	int32_t* dst = (int32_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_read8(int id, uint8_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "id: %d, val: %p", id, val);

	uint8_t* src = (uint8_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, "*val: %d", *val);
	return result;
}

extern int REG_read16(int id, uint16_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "id: %d, val: %p", id, val);

	uint16_t* src = (uint16_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, "*val: %d", *val);
	return result;
}

extern int REG_read32(int id, int32_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "id: %d, val: %p", id, val);

	int32_t* src = (int32_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, "*val: %d", *val);
	return result;
}

extern int REG_readLong(REG_map* src, int id, long* val) {
	int result = OK;
	RV_LogEntry(__func__, "id: %d, val: %p", id, val);

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

	case REG_AMBOFFSET:
	case REG_AMB_LINE_NE:
	case REG_AMB_LINE_EN:
	case REG_AMB_LINE_ES:
	case REG_AMB_LINE_SE:
	case REG_AMB_LINE_SW:
	case REG_AMB_LINE_WS:
	case REG_AMB_LINE_WN:
	case REG_AMB_LINE_NW:
	case REG_AMB_COL_NE:
	case REG_AMB_COL_SE:
	case REG_AMB_COL_SW:
	case REG_AMB_COL_NW:
	case REG_IR_LINE_NE:
	case REG_IR_LINE_EN:
	case REG_IR_LINE_ES:
	case REG_IR_LINE_SE:
	case REG_IR_LINE_SW:
	case REG_IR_LINE_WS:
	case REG_IR_LINE_WN:
	case REG_IR_LINE_NW:
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
	case REG_COLLISION:
	case REG_LINE:
		offset = reg_address[id] -  ((uint8_t*)&reg_map);
		v8 = (uint8_t*)&s[offset];
		*val = (long) *v8;
		break;
	default:
		break;
	}

	RV_LogExit(__func__, result, "*val: %d", *val);
	return result;

}

extern int REG_readAll(REG_map* dst) {
	int result = OK;
	RV_LogEntry(__func__, "dst: %p", dst);

	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = reg_map;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_writeAll(REG_map* src) {
	int result = OK;
	RV_LogEntry(__func__, "src: %p", src);

	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	reg_map = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern const char* REG_getRegistername(int idx) {
	if ((idx >= 0) && (idx < REG_MAX)) {
		return reg_name[idx];
	}
	return NULL;
}

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

extern void REG_logAll(REG_map* src) {
	printf("reg_map: \n");
	LOG_U32(src, HEADER);
	LOG_U32(src, MICROS);
	LOG_U32(src, MILLIS);
	LOG_U8(src, LEFTDIR);
	LOG_U8(src, LEFTDC);
	LOG_U8(src, RIGHTDIR);
	LOG_U8(src, RIGHTDC);
	LOG_U8(src, COLLISION);
	LOG_U8(src, LINE);
	LOG_32(src, LEFTPOS);
	LOG_32(src, RIGHTPOS);
	LOG_U16(src, AMBOFFSET);
	LOG_U16(src, AMB_LINE_NE);
	LOG_U16(src, AMB_LINE_EN);
	LOG_U16(src, AMB_LINE_ES);
	LOG_U16(src, AMB_LINE_SE);
	LOG_U16(src, AMB_LINE_SW);
	LOG_U16(src, AMB_LINE_WS);
	LOG_U16(src, AMB_LINE_WN);
	LOG_U16(src, AMB_LINE_NW);
	LOG_U16(src, AMB_COL_NE);
	LOG_U16(src, AMB_COL_SE);
	LOG_U16(src, AMB_COL_SW);
	LOG_U16(src, AMB_COL_NW);
	LOG_U16(src, IR_LINE_NE);
	LOG_U16(src, IR_LINE_EN);
	LOG_U16(src, IR_LINE_ES);
	LOG_U16(src, IR_LINE_SE);
	LOG_U16(src, IR_LINE_SW);
	LOG_U16(src, IR_LINE_WS);
	LOG_U16(src, IR_LINE_WN);
	LOG_U16(src, IR_LINE_NW);
	LOG_U16(src, IR_COL_NE);
	LOG_U16(src, IR_COL_SE);
	LOG_U16(src, IR_COL_SW);
	LOG_U16(src, IR_COL_NW);
	LOG_U32(src, TRAILER);
}

