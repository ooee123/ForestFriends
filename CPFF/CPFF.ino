#include "CPFF.h"


Axis xAxis, yAxis, zAxis;
int len = 0;
int cur = 0;

int initAxis(Axis *a, char pinA, char pinB, char ind) {
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(ind, INPUT);
  a->encoderPinA = pinA;
  a->encoderPinB = pinB;
  a->index = ind;
  a->prevA = digitalRead(pinA);
  a->prevB = digitalRead(pinB);
  a->pos = 0;
}

void pollEncoder(Axis *axis) {
  byte newA = digitalRead(axis->encoderPinA);
  byte newB = digitalRead(axis->encoderPinB);
  byte prevA = axis->prevA;
  byte prevB = axis->prevB;
  byte prevSum = (prevA << 1) + prevB;
  if (prevA != newA) {
    if (prevSum == 1 || prevSum == 2) {
      axis->pos += 1;
    } else {
      axis->pos -= 1;
    }
  } else {
    if (prevSum == 0 || prevSum == 3) {
      axis->pos += 1;
    } else {
      axis->pos -= 1;
    }
  }
  axis->prevA = newA;
  axis->prevB = newB;
  Serial.print("En:");
  Serial.println(axis->pos, DEC);
}

void getPaths() {
  short x = 0;
  short y = 0;
  short type = 0;
  if (Serial.available() >= 6) {
    x = Serial.read() << 8;
    x += Serial.read();
    
    y = Serial.read() << 8;
    y += Serial.read();
    
    type = Serial.read() << 8;
    type += Serial.read();
    
    Serial.print("(");
    Serial.print(x, DEC);
    Serial.print(", ");
    Serial.print(y, DEC);
    Serial.print(", ");
    Serial.print(type, DEC);
    Serial.println(")");
    Serial.flush();
    
    paths[len].x = x;
    paths[len].y = y;
    paths[len].type = (MovementType)type;
    len++;
  } 
}

void PID() {
  int deltaX;
  int deltaY;
  
  deltaX = paths[cur].x - xAxis.pos;
  deltaY = paths[cur].y - yAxis.pos;
  
}

void setup() {
  Serial.begin(9600);
  initAxis(&xAxis, 0, 0, 0);
  initAxis(&yAxis, 0, 0, 0);
  initAxis(&zAxis, 0, 0, 0);
}

void loop() {
}
