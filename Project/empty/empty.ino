void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  //string str = Serial.readStringUntil('\n');

  Serial.write("hello");
  delay(900);
  Serial.write("world");
  delay(900);
}
