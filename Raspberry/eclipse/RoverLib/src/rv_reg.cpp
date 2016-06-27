#include <stdio.h>
#include <stdint.h>
#include <semaphore.h>
#include <strings.h>

#include "rv.h"
#include "rv_log.h"
#include "rv_reg.h"

static REG_map reg_map;
static sem_t reg_sem;
static uint8_t* reg_address[REG_MAX];

void REG_setup() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	bzero(&reg_map, sizeof(reg_map));
	sem_init(&reg_sem, 0, 1);

	reg_address[REG_MICROS] = (uint8_t*) &reg_map.MICROS;
	reg_address[REG_MILLIS] = (uint8_t*) &reg_map.MILLIS;
	reg_address[REG_LEFTDIR] = (uint8_t*) &reg_map.LEFTDIR;
	reg_address[REG_LEFTDC] = (uint8_t*) &reg_map.LEFTDC;
	reg_address[REG_RIGHTDIR] = (uint8_t*) &reg_map.RIGHTDIR;
	reg_address[REG_RIGHTDC] = (uint8_t*) &reg_map.RIGHTDC;
	reg_address[REG_COLLISION] = (uint8_t*) &reg_map.COLLISION;
	reg_address[REG_LINE] = (uint8_t*) &reg_map.LINE;
	reg_address[REG_LEFTPOS] = (uint8_t*) &reg_map.LEFTPOS;
	reg_address[REG_RIGHTPOS] = (uint8_t*) &reg_map.RIGHTPOS;
	reg_address[REG_AMBOFFSET] = (uint8_t*) &reg_map.AMBOFFSET;
	reg_address[REG_AMB_COL_NE] = (uint8_t*) &reg_map.AMB_COL_NE;
	reg_address[REG_AMB_COL_SE] = (uint8_t*) &reg_map.AMB_COL_SE;
	reg_address[REG_AMB_COL_SW] = (uint8_t*) &reg_map.AMB_COL_SW;
	reg_address[REG_AMB_COL_NW] = (uint8_t*) &reg_map.AMB_COL_NW;
	reg_address[REG_AMB_LINE_NE] = (uint8_t*) &reg_map.AMB_LINE_NE;
	reg_address[REG_AMB_LINE_EN] = (uint8_t*) &reg_map.AMB_LINE_EN;
	reg_address[REG_AMB_LINE_ES] = (uint8_t*) &reg_map.AMB_LINE_ES;
	reg_address[REG_AMB_LINE_SE] = (uint8_t*) &reg_map.AMB_LINE_SE;
	reg_address[REG_AMB_LINE_SW] = (uint8_t*) &reg_map.AMB_LINE_SW;
	reg_address[REG_AMB_LINE_WS] = (uint8_t*) &reg_map.AMB_LINE_WS;
	reg_address[REG_AMB_LINE_WN] = (uint8_t*) &reg_map.AMB_LINE_WN;
	reg_address[REG_AMB_LINE_NW] = (uint8_t*) &reg_map.AMB_LINE_NW;
	reg_address[REG_IR_COL_NE] = (uint8_t*) &reg_map.IR_COL_NE;
	reg_address[REG_IR_COL_SE] = (uint8_t*) &reg_map.IR_COL_SE;
	reg_address[REG_IR_COL_SW] = (uint8_t*) &reg_map.IR_COL_SW;
	reg_address[REG_IR_COL_NW] = (uint8_t*) &reg_map.IR_COL_NW;
	reg_address[REG_IR_LINE_NE] = (uint8_t*) &reg_map.IR_LINE_NE;
	reg_address[REG_IR_LINE_EN] = (uint8_t*) &reg_map.IR_LINE_EN;
	reg_address[REG_IR_LINE_ES] = (uint8_t*) &reg_map.IR_LINE_ES;
	reg_address[REG_IR_LINE_SE] = (uint8_t*) &reg_map.IR_LINE_SE;
	reg_address[REG_IR_LINE_SW] = (uint8_t*) &reg_map.IR_LINE_SW;
	reg_address[REG_IR_LINE_WS] = (uint8_t*) &reg_map.IR_LINE_WS;
	reg_address[REG_IR_LINE_WN] = (uint8_t*) &reg_map.IR_LINE_WN;
	reg_address[REG_IR_LINE_NW] = (uint8_t*) &reg_map.IR_LINE_NW;


	RV_LogExit(__func__, result, NULL);
}

extern void REG_write8(int id, uint8_t val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %d", val);

	uint8_t* dst = (uint8_t*) (reg_address[id]);

	sem_wait(&reg_sem);
	*dst = val;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, NULL);
}

extern void REG_write16(int id, uint16_t val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %d", val);

	uint16_t* dst = (uint16_t*) (reg_address[id]);
	sem_wait(&reg_sem);
	*dst = val;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, NULL);
}

extern void REG_write32(int id, int32_t val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %d", val);

	int32_t* dst = (int32_t*) (reg_address[id]);
	sem_wait(&reg_sem);
	*dst = val;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, NULL);
}

extern void REG_read8(int id, uint8_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %p", val);

	uint8_t* src = (uint8_t*) (reg_address[id]);
	sem_wait(&reg_sem);
	*val = *src;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, "*val: %d", *val);
}

extern void REG_read16(int id, uint16_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %p", val);

	uint16_t* src = (uint16_t*) (reg_address[id]);
	sem_wait(&reg_sem);
	*val = *src;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, "*val: %d", *val);
}

extern void REG_read32(int id, int32_t* val) {
	int result = OK;
	RV_LogEntry(__func__, "val: %p", val);

	int32_t* src = (int32_t*) (reg_address[id]);
	sem_wait(&reg_sem);
	*val = *src;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, "*val: %d", *val);
}

extern void REG_readAll(REG_map* dst) {
	int result = OK;
	RV_LogEntry(__func__, "dst: %p", dst);

	sem_wait(&reg_sem);
	*dst = reg_map;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, NULL);
}

extern void REG_writeAll(REG_map* src) {
	int result = OK;
	RV_LogEntry(__func__, "src: %p", src);

	sem_wait(&reg_sem);
	reg_map = *src;
	sem_post(&reg_sem);

	RV_LogExit(__func__, result, NULL);
}

