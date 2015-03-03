typedef enum {
  START,
  LINE,
  MOVE
} MovementType;

typedef struct {
  int x;
  int y;
  MovementType type;
} Path;

Path paths[512];
int len = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  short x = 0;
  short y = 0;
  short type = 0;
  if (Serial.available() >= 6) {
    /*
    x = Serial.read() << 24;
    x += Serial.read() << 16;
    x += Serial.read() << 8;
    x += Serial.read();
    
    y = Serial.read() << 24;
    y += Serial.read() << 16;
    y += Serial.read() << 8;
    y += Serial.read();
    
    type = Serial.read() << 24;
    type += Serial.read() << 16;
    type += Serial.read() << 8;
    type += Serial.read();
    */
    
    x += Serial.read() << 8;
    x += Serial.read();
    
    y += Serial.read() << 8;
    y += Serial.read();
    
    type += Serial.read() << 8;
    type += Serial.read();
    
    /*
    x = Serial.parseInt();
    y = Serial.parseInt();
    type = Serial.parseInt();
    */
    
    Serial.print("(");
    Serial.print(x, DEC);
    Serial.print(", ");
    Serial.print(y, DEC);
    Serial.print(", ");
    Serial.print(type, DEC);
    Serial.println(")");
    Serial.flush();
    /*
    paths[len].x = x;
    paths[len].y = y;
    paths[len].type = (MovementType)type;
    */
    
  }
}
