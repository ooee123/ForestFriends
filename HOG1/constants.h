#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define TICKS_PER_REVOLUTION 30
#define REVOLUTIONS_PER_INCH 8
#define INCH TICKS_PER_REVOLUTION * REVOLUTIONS_PER_INCH

#define PCONSTANT 0
#define PGAIN 4.0

#define START 0
#define LINE 1
#define MOVE 2

#define JIG_HEIGHT INCH * 1
#define ROUTING_DEPTH INCH / 4
#define Z_AXIS_TOLERANCE 20

#define CALIBRATE_SPEED 100

#define TOLERANCE 60
#define isWithinTolerance(x, y) (abs((x) - (y)) <= TOLERANCE)
#define NEXT_COMMAND "Z"
#define AT_HOME "#"

#endif
