#ifndef HSI_H
#define HSI_H

////////////////////////////////////////////////////
// HSI
////////////////////////////////////////////////////

#define PIN_PWM_RIGHT (6)
#define PIN_DIR_RIGHT (7)

#define PIN_PWM_LEFT  (4)
#define PIN_DIR_LEFT  (5)

#define PIN_ENC_LEFT_A   (18) 
#define PIN_ENC_LEFT_B   (19)
#define PIN_ENC_RIGHT_A  (16)
#define PIN_ENC_RIGHT_B  (17)

#define PIN_LINE_NE (A8)
#define PIN_LINE_EN (A9)
#define PIN_LINE_ES (A10)
#define PIN_LINE_SE (A11)
#define PIN_LINE_SW (A12)
#define PIN_LINE_WS (A13)
#define PIN_LINE_WN (A14)
#define PIN_LINE_NW (A15)

#define PIN_LINE_LIGHT (30) 

#define PIN_COL_NE (A0) // TODO: check A12-A15 availability
#define PIN_COL_SE (A1)
#define PIN_COL_SW (A2)
#define PIN_COL_NW (A3)

#define PIN_COL_LIGHT_NE (22)
#define PIN_COL_LIGHT_SE (24)
#define PIN_COL_LIGHT_SW (26)
#define PIN_COL_LIGHT_NW (28)



#define PIN_REQEXC    (48) // Input, high whenever PI requests transfer of frame
#define PIN_ACKEXC    (47) // Output, high as soon as MEGA ready for frame transfer
//#define PIN_RTS       (45) // PI indicates that it is ready to send a byte.
//#define PIN_RECEIVING (46) // AVR indicates that is in receiving mode.

#endif


