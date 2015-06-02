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
#define INTERRUPTS_PER_TICK  4
#define REVOLUTIONS_PER_INCH 8
#define INCH (TICKS_PER_REVOLUTION * REVOLUTIONS_PER_INCH * INTERRUPTS_PER_TICK)

#define TOLERANCE (INCH / 16)
#define Z_AXIS_TOLERANCE TOLERANCE

#define START 0
#define LINE 1
#define MOVE 2
#define HOME_THREE_QUARTER_BOARD  3
#define HOME_ONE_POINT_FIVE_BOARD 4

// Let DISTANCE_1_5 be the distance from the router bit to the surface of a 1.5 in board
#define DISTANCE_1_5     (INCH / 4)
#define HOVER_HEIGHT     (INCH / 4)
#define ROUTING_DEPTH    (INCH * 3 / 16)
//#define DISTANCE_1_5     0
//#define HOVER_HEIGHT     0
//#define ROUTING_DEPTH    0

#define X_PGAIN     5.5
//#define X_PGAIN     1
#define X_PCONSTANT 20
#define X_POWERMIN  280
#define X_POWERMAX  700
//#define X_POWERMIN  0
//#define X_POWERMAX  0
#define X_IGAIN     0.003
#define X_IMIN      0
#define X_IMAX      100000

#define Y_PGAIN     3.5
//#define Y_PGAIN     1
#define Y_PCONSTANT 5
#define Y_POWERMIN  100
#define Y_POWERMAX  400
//#define Y_POWERMIN  0
//#define Y_POWERMAX  0
#define Y_IGAIN     0.002
#define Y_IMIN      0
#define Y_IMAX      100000

#define Z_PGAIN     4
//#define Z_PGAIN     1
#define Z_PCONSTANT 0
#define Z_POWERMIN  160
#define Z_POWERMAX  230
//#define Z_POWERMIN  0
//#define Z_POWERMAX  0
#define Z_IGAIN     0.001
#define Z_IMIN      0
#define Z_IMAX      100000

#define X_CALIBRATE_SPEED -100
#define Y_CALIBRATE_SPEED -155
#define Z_CALIBRATE_SPEED -190

#define ADC_MAX 31

//#define isWithinTolerance(x, y, t) (abs((x) - (y)) <= t)
#define isWithinTolerance(x, y, t) (abs((x) - (y)) <= TOLERANCE)
//#define isWithinTolerance(x, y) (abs((x) - (y)) <= TOLERANCE)
#define NEXT_COMMAND "@"
#define AT_HOME "#"

#endif
