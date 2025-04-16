#define EMIT_PIN 11

void setup(){

}

void loop(){
  pinMode(EMIT_PIN, OUTPUT);
  digitalWrite(EMIT_PIN, LOW);
  delayMicroseconds(100);
  // // delay(1);
  pinMode(EMIT_PIN, INPUT);
  delay(6);
  // delayMicroseconds(5500);
}