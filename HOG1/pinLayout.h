#ifndef _PIN_LAYOUT_H_
#define _PIN_LAYOUT_H_

#define X_ENCODER_DDR  DDRE
#define X_ENCODER_PIN  PINE
#define X_ENCODER_PORT PORTE
#define X_ENCODER_PINA PE4
#define X_ENCODER_PINB PE5
#define X_ENCODER_A_INT_VECT INT4_vect
#define X_ENCODER_B_INT_VECT INT5_vect

#define Y_ENCODER_DDR  DDRE
#define Y_ENCODER_PIN  PINE
#define Y_ENCODER_PORT PORTE
#define Y_ENCODER_PINA PE6
#define Y_ENCODER_PINB PE7
#define Y_ENCODER_A_INT_VECT INT6_vect
#define Y_ENCODER_B_INT_VECT INT7_vect

#define Z_ENCODER_DDR  DDRE
#define Z_ENCODER_PIN  PINE
#define Z_ENCODER_PORT PORTE
#define Z_ENCODER_PINA PE1
#define Z_ENCODER_PINB PE0

#define X_LIMIT_PIN    PINA
#define X_LIMIT_PIN_NUM PA7

#define Y_LIMIT_PIN    PINA
#define Y_LIMIT_PIN_NUM PA6

#define _bitValue(X, Y) (((X) >> (Y)) & 1)
#define getXLimitSwitch() _bitValue(X_LIMIT_PIN, X_LIMIT_PIN_NUM)
#define getYLimitSwitch() _bitValue(Y_LIMIT_PIN, Y_LIMIT_PIN_NUM)

#define CALIBRATE_SPEED 10

#endif
