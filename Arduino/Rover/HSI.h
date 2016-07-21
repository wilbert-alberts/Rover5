#ifndef HSI_H
#define HSI_H

////////////////////////////////////////////////////
// HSI
////////////////////////////////////////////////////

#define PIN_PWM_LEFT (6)
#define PIN_DIR_LEFT (7)

#define PIN_PWM_RIGHT  (4)
#define PIN_DIR_RIGHT  (5)

#define PIN_ENC_RIGHT_A   (18) 
#define PIN_ENC_RIGHT_B   (19)
#define PIN_ENC_LEFT_A  (3)
#define PIN_ENC_LEFT_B  (2)

#define PIN_LINE_N (A11)
#define PIN_LINE_E (A10)
#define PIN_LINE_S (A9)
#define PIN_LINE_W (A8)

#define PIN_LINE_LIGHT (30) 

#define PIN_COL_NE (A2) 
#define PIN_COL_SE (A1)
#define PIN_COL_SW (A0)
#define PIN_COL_NW (A3)

#define PIN_COL_LIGHT_NE (26)
#define PIN_COL_LIGHT_SE (24)
#define PIN_COL_LIGHT_SW (22)
#define PIN_COL_LIGHT_NW (28)



#define PIN_REQEXC    (48) // Input, high whenever PI requests transfer of frame
#define PIN_ACKEXC    (47) // Output, high as soon as MEGA ready for frame transfer
//#define PIN_RTS       (45) // PI indicates that it is ready to send a byte.
//#define PIN_RECEIVING (46) // AVR indicates that is in receiving mode.

#endif


