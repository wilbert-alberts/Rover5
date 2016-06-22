#ifndef HSI_H
#define HSI_H

////////////////////////////////////////////////////
// HSI
////////////////////////////////////////////////////

#define PIN_PWM_LEFT  (12)
#define PIN_PWM_RIGHT (13)

#define PIN_DIR_LEFT  (22)
#define PIN_DIR_RIGHT (23)

#define PIN_LINE_NE (A0)
#define PIN_LINE_EN (A1)
#define PIN_LINE_ES (A2)
#define PIN_LINE_SE (A3)
#define PIN_LINE_SW (A4)
#define PIN_LINE_WS (A5)
#define PIN_LINE_WN (A6)
#define PIN_LINE_NW (A7)

#define PIN_LINE_LIGHT (0) // TODO: select pin for LINE_LIGHT

#define PIN_COL_NE (A12) // TODO: check A12-A15 availability
#define PIN_COL_SE (A13)
#define PIN_COL_SW (A14)
#define PIN_COL_NW (A15)

#define PIN_COL_LIGHT_NE (8)
#define PIN_COL_LIGHT_SE (9)
#define PIN_COL_LIGHT_SW (10)
#define PIN_COL_LIGHT_NW (11)


#define PIN_ENC_LEFT_A   (18) 
#define PIN_ENC_LEFT_B   (19)
#define PIN_ENC_RIGHT_A  (2)
#define PIN_ENC_RIGHT_B  (3)

#define PIN_REQEXC  (0)  // TODO: select pin for Request Exchange        // Input, high whenever PI requests data
#define PIN_ACKEXC  (0)  // TODO: select pin for Acknowledge Exchange    // Output, high as soon as MEGA ready to send data

#endif


