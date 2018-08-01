void setup() {
   Serial.begin(9600); // Start serial communication at 9600 bps
   DDRD |= B00100000;
   DDRB |= B00001010;
   delay (100);
 }

 void loop() {
  uint8_t r,g,b;
  if(Serial.available()>2){
    r = Serial.read();
    g = Serial.read();
    b = Serial.read();
    Serial.flush();
    analogWrite(5, r);
    analogWrite(9, g);
    analogWrite(11,b);
    /*Serial.write(r);
    Serial.write(g);
    Serial.write(b);*/
  }
  delay(10);
 }
