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
	RV_LogEntry(__func__, "val: %d", val);

	uint8_t* dst = (uint8_t*) (reg_address[id]);

	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_write16(int id, uint16_t val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %d", val);

	uint16_t* dst = (uint16_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_write32(int id, int32_t val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %d", val);

	int32_t* dst = (int32_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*dst = val;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int REG_read8(int id, uint8_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %p", val);

	uint8_t* src = (uint8_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, "*val: %d", *val);
	return result;
}

extern int REG_read16(int id, uint16_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %p", val);

	uint16_t* src = (uint16_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, "*val: %d", *val);
	return result;
}

extern int REG_read32(int id, int32_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %p", val);

	int32_t* src = (int32_t*) (reg_address[id]);
	SAFE_INVOKE(sem_wait(&reg_sem), result, RV_SEM_WAIT_FAILED)
	*val = *src;
	SAFE_INVOKE(sem_post(&reg_sem), result, RV_SEM_POST_FAILED)

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

extern void REG_logAll(REG_map* src)
{
  printf("reg_map: \n");
  printf("%s: 0x%0x\n", reg_name[REG_HEADER], src->HEADER);
  printf("%s: 0x%0x\n", reg_name[REG_MICROS], src->MICROS);
  printf("%s: 0x%0x\n", reg_name[REG_MILLIS], src->MILLIS);
  printf("%s: 0x%0x\n", reg_name[REG_LEFTDIR], src->LEFTDIR);
  printf("%s: 0x%0x\n", reg_name[REG_LEFTDC], src->LEFTDC);
  printf("%s: 0x%0x\n", reg_name[REG_RIGHTDIR], src->RIGHTDIR);
  printf("%s: 0x%0x\n", reg_name[REG_RIGHTDC], src->RIGHTDC);
  printf("%s: 0x%0x\n", reg_name[REG_COLLISION], src->COLLISION);
  printf("%s: 0x%0x\n", reg_name[REG_LINE], src->LINE);
  printf("%s: 0x%0x\n", reg_name[REG_LEFTPOS], src->LEFTPOS);
  printf("%s: 0x%0x\n", reg_name[REG_RIGHTPOS], src->RIGHTPOS);
  printf("%s: 0x%0x\n", reg_name[REG_AMBOFFSET], src->AMBOFFSET);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_COL_NE], src->AMB_COL_NE);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_COL_SE], src->AMB_COL_SE);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_COL_SW], src->AMB_COL_SW);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_COL_NW], src->AMB_COL_NW);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_NE], src->AMB_LINE_NE);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_EN], src->AMB_LINE_EN);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_ES], src->AMB_LINE_ES);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_SE], src->AMB_LINE_SE);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_SW], src->AMB_LINE_SW);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_WS], src->AMB_LINE_WS);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_WN], src->AMB_LINE_WN);
  printf("%s: 0x%0x\n", reg_name[REG_AMB_LINE_NW], src->AMB_LINE_NW);
  printf("%s: 0x%0x\n", reg_name[REG_IR_COL_NE], src->IR_COL_NE);
  printf("%s: 0x%0x\n", reg_name[REG_IR_COL_SE], src->IR_COL_SE);
  printf("%s: 0x%0x\n", reg_name[REG_IR_COL_SW], src->IR_COL_SW);
  printf("%s: 0x%0x\n", reg_name[REG_IR_COL_NW], src->IR_COL_NW);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_NE], src->IR_LINE_NE);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_EN], src->IR_LINE_EN);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_ES], src->IR_LINE_ES);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_SE], src->IR_LINE_SE);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_SW], src->IR_LINE_SW);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_WS], src->IR_LINE_WS);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_WN], src->IR_LINE_WN);
  printf("%s: 0x%0x\n", reg_name[REG_IR_LINE_NW], src->IR_LINE_NW);
  printf("%s: 0x%0x\n", reg_name[REG_TRAILER], src->TRAILER);
}


