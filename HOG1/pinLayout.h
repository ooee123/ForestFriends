#ifndef _PIN_LAYOUT_H_
#define _PIN_LAYOUT_H_

#define X_ENCODER_DDR  DDRE
#define X_ENCODER_PIN  PINE
#define X_ENCODER_PORT PORTE
#define X_ENCODER_PINA PE4
#define X_ENCODER_PINB PE5

#define Y_ENCODER_DDR  DDRE
#define Y_ENCODER_PIN  PINE
#define Y_ENCODER_PORT PORTE
#define Y_ENCODER_PINA PE6
#define Y_ENCODER_PINB PE7

#define Z_ENCODER_DDR  DDRD
#define Z_ENCODER_PIN  PIND
#define Z_ENCODER_PORT PORTD
#define Z_ENCODER_PINA PD0
#define Z_ENCODER_PINB PD1

#define turnOffEncoders() (EIMSK = 0)

#define X_LIMIT_DDR          DDRA
#define X_LIMIT_PORT         PORTA
#define X_LIMIT_PIN          PINA
#define X_ZERO_LIMIT_PIN_NUM PA7
#define X_MAX_LIMIT_PIN_NUM  PA4

#define Y_LIMIT_DDR          DDRA
#define Y_LIMIT_PORT         PORTA
#define Y_LIMIT_PIN          PINA
#define Y_ZERO_LIMIT_PIN_NUM PA6
#define Y_MAX_LIMIT_PIN_NUM  PA3

#define Z_LIMIT_DDR          DDRA
#define Z_LIMIT_PORT         PORTA
#define Z_LIMIT_PIN          PINA
#define Z_ZERO_LIMIT_PIN_NUM PA5
#define Z_MAX_LIMIT_PIN_NUM  PA2

#define CURRENT_DDR          DDRF
#define CURRENT_PORT         PORTF
#define CURRENT_PIN          PINF
#define X_CURRENT_PIN_NUM    PF0
#define Y_CURRENT_PIN_NUM    PF1
#define Z_CURRENT_PIN_NUM    PF2
#define NUM_CURRENT_SENSORS  3

#define SOLID_STATE_DDR      DDRA
#define SOLID_STATE_PORT     PORTA
#define SOLID_STATE_PIN_NUM  PA0

#define _getBit(X, Y)   (((X) >> (Y)) & 1)
#define _setBit(X, Y)   ((X) |= (1 << (Y)))
#define _clearBit(X, Y) ((X) &= ~(1 << (Y)))
#define setupLimitSwitch(DDR, PORT, NUM) (_clearBit((DDR), (NUM)), _setBit((PORT), (NUM)))
#define getXLimitSwitch() _getBit(X_LIMIT_PIN, X_ZERO_LIMIT_PIN_NUM)
#define getYLimitSwitch() _getBit(Y_LIMIT_PIN, Y_ZERO_LIMIT_PIN_NUM)
#define getZLimitSwitch() _getBit(Z_LIMIT_PIN, Z_ZERO_LIMIT_PIN_NUM)
#define getXMaxLimitSwitch() _getBit(X_LIMIT_PIN, X_MAX_LIMIT_PIN_NUM)
#define getYMaxLimitSwitch() _getBit(Y_LIMIT_PIN, Y_MAX_LIMIT_PIN_NUM)
#define getZMaxLimitSwitch() _getBit(Z_LIMIT_PIN, Z_MAX_LIMIT_PIN_NUM)

#endif
