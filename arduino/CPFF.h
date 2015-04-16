typedef enum {
  START,
  LINE,
  MOVE
} MovementType;

typedef struct {
  char encoderPinA;
  char encoderPinB;
  char index;
  char prevA;
  char prevB;
  int pos;
} Axis;

typedef struct {
  int x;
  int y;
  MovementType type;
} Path;
