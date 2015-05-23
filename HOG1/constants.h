#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define TICKS_PER_REVOLUTION 30
#define REVOLUTIONS_PER_INCH 8
#define INCH (TICKS_PER_REVOLUTION * REVOLUTIONS_PER_INCH)

#define PCONSTANT 30
#define PGAIN 1.7
#define PID_MAX 65000
#define PID_MIN 0

#define START 0
#define LINE 1
#define MOVE 2

#define DISTANCE_1_5     (INCH / 2)
#define HOVER_HEIGHT     (INCH / 4)
#define ROUTING_DEPTH    (INCH * 3 / 16)
#define Z_AXIS_TOLERANCE 20

#define X_PGAIN     PGAIN
#define X_PCONSTANT PCONSTANT
#define X_POWERMAX  PID_MAX
#define X_POWERMIN  PID_MIN

#define Y_PGAIN     PGAIN
#define Y_PCONSTANT PCONSTANT
#define Y_POWERMAX  PID_MAX
#define Y_POWERMIN  PID_MIN

#define Z_PGAIN     PGAIN
#define Z_PCONSTANT PCONSTANT
#define Z_POWERMAX  PID_MAX
#define Z_POWERMIN  PID_MIN

#define CALIBRATE_SPEED 100

#define TOLERANCE (INCH / 16)
#define isWithinTolerance(x, y, t) (abs((x) - (y)) <= t)
#define NEXT_COMMAND "Z"
#define AT_HOME "#"

#endif
