void setup() {
  DDRK = 0xff;
  PORTK = 0;

  Serial.begin(9600);

  pinMode(47, INPUT_PULLUP);
  pinMode(49, INPUT_PULLUP);

}

void loop() {
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0xA1) {
      PORTK = B00000000;
      delay(800);
      PORTK = B10000000;
      delay(800);
      PORTK = B01000000;
      delay(800);
      PORTK = B00100000;
      delay(800);
      PORTK = B00010000;
      delay(800);
      PORTK = B00001000;
      delay(800);
      PORTK = B00000100;
      delay(800);
      PORTK = B00000010;
      delay(800);
    } else if (inByte == 0xB1) {
      PORTK = B10000001;
      delay(800);
      PORTK = B01000010;
      delay(800);
      PORTK = B00100100;
      delay(800);
      PORTK = B00011000;
      delay(800);
      PORTK = B00000000;
      delay(800);
    }
  }

  if (digitalRead(47) == LOW) {
    Serial.write(0xB1);
    delay(200);
  }
  if (digitalRead(49) == LOW) {
    Serial.write(0xB0);
    delay(200);
  }
}
