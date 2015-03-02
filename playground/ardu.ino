void setup() {
   Serial.begin(9600);
}

void loop() {
   if (Serial.available() >= 4) {
      int number = 0;
      number += Serial.read() << 24;
      number += Serial.read() << 16;
      number += Serial.read() << 8;
      number += Serial.read();
      Serial.println(number, DEC);
      Serial.flush();
   }
}
