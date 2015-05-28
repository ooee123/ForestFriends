#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

typedef enum Direction {
   DECREASING, INCREASING, ZERO
} Direction;

typedef enum State
{
   HOME, NORMAL
} State;

#define TICKS_PER_REVOLUTION 32
#define REVOLUTIONS_PER_INCH 8
#define INCH (TICKS_PER_REVOLUTION * REVOLUTIONS_PER_INCH)

#define START 0
#define LINE 1
#define MOVE 2
#define HOME_THREE_QUARTER_BOARD  3
#define HOME_ONE_POINT_FIVE_BOARD 4

#define DISTANCE_1_5     (INCH / 2)
#define HOVER_HEIGHT     (INCH / 4)
#define ROUTING_DEPTH    (INCH * 3 / 16)
#define Z_AXIS_TOLERANCE 20

#define X_PGAIN     0.05
#define X_PCONSTANT 0
#define X_POWERMIN  0
#define X_POWERMAX  200

#define Y_PGAIN     0.05
#define Y_PCONSTANT 0
#define Y_POWERMIN  0
#define Y_POWERMAX  200

#define Z_PGAIN     0.05
#define Z_PCONSTANT 0
#define Z_POWERMIN  0
#define Z_POWERMAX  50

#define X_CALIBRATE_SPEED -200
#define Y_CALIBRATE_SPEED -100
#define Z_CALIBRATE_SPEED -20

#define ADC_MAX 31

#define TOLERANCE (INCH / 16)
#define isWithinTolerance(x, y, t) (abs((x) - (y)) <= t)
#define NEXT_COMMAND "@"
#define AT_HOME "#"

#endif
