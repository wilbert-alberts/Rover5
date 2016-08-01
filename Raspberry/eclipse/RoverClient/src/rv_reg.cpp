#include <stdio.h>
#include <stdint.h>
#include <semaphore.h>
#include <strings.h>

#include "rv_reg.h"


#define REG_ADDREGISTER(R) 							\
  reg_address[REG_ ## R] = (uint8_t*) &reg_map.R; 	\
  reg_name[REG_ ## R] = #R;

static REG_map reg_map;
static uint8_t* reg_address[REG_MAX];
static const char* reg_name[REG_MAX];

extern int REG_setup() {
	int result = 0;

	bzero(&reg_map, sizeof(reg_map));

	if (result == 0) {
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


	return result;
}

extern int REG_write8(int id, uint8_t val) {
	int result = 0;

	uint8_t* dst = (uint8_t*) (reg_address[id]);
	*dst = val;

	return result;
}

extern int REG_write16(int id, uint16_t val) {
	int result = 0;

	uint16_t* dst = (uint16_t*) (reg_address[id]);
	*dst = val;

	return result;
}

extern int REG_write32(int id, int32_t val) {
	int result = 0;

	int32_t* dst = (int32_t*) (reg_address[id]);
	*dst = val;

	return result;
}

extern int REG_read8(int id, uint8_t* val) {
	int result = 0;

	uint8_t* src = (uint8_t*) (reg_address[id]);
	*val = *src;

	return result;
}

extern int REG_read16(int id, uint16_t* val) {
	int result = 0;

	uint16_t* src = (uint16_t*) (reg_address[id]);
	*val = *src;

	return result;
}

extern int REG_read32(int id, int32_t* val) {
	int result = 0;

	int32_t* src = (int32_t*) (reg_address[id]);
	*val = *src;

	return result;
}

extern int REG_readLong(REG_map* src, int id, long* val) {
	int result = 0;

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


	return result;

}

extern int REG_fillHeaderTrailer(REG_map* m)
{
	uint8_t* header = (uint8_t*) (&m->HEADER);
	uint8_t* trailer = (uint8_t*) (&m->TRAILER);

	header[0] = 0x00;
	header[1] = 0xA5;
	header[2] = 0xFF;
	header[3] = 0x5A;

	trailer[0] = 0xFF;
	trailer[1] = 0xA5;
	trailer[2] = 0x00;
	trailer[3] = 0x5A;

	return 0;
}

extern int REG_checkHeaderTrailer(REG_map* m)
{
	uint8_t* header = (uint8_t*) (&m->HEADER);
	uint8_t* trailer = (uint8_t*) (&m->TRAILER);

	if (header[0] != 0x00)
		return -1;
	if (header[1] != 0xA5)
		return -1;
	if (header[2] != 0xFF)
		return -1;
	if (header[3] != 0x5A)
		return -1;
	if (trailer[0] != 0xFF)
		return -1;
	if (trailer[1] != 0xA5)
		return -1;
	if (trailer[2] != 0x00)
		return -1;
	if (trailer[3] != 0x5A)
		return -1;
	return 0;
}


extern int REG_readAll(REG_map* dst) {
	int result = 0;

	*dst = reg_map;
	return result;
}

extern int REG_writeAll(REG_map* src) {
	int result = 0;

	reg_map = *src;

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
	printf("%s: %d\n", reg_name[REG_ ## R], *v); \
}

#define LOG_32(S, R) \
{\
	int32_t* v = (int32_t*) (&(S->R));       \
	printf("%s: %d\n", reg_name[REG_ ## R], *v); \
}

#define LOG_U16(S, R) \
{\
	uint16_t* v = (uint16_t*) (&(S->R));        \
	printf("%s: %d\n", reg_name[REG_ ## R], *v); \
}

#define LOG_U8(S, R) \
{\
	uint8_t* v = (uint8_t*) (&(S->R));       \
	printf("%s: %d\n", reg_name[REG_ ## R], *v); \
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

