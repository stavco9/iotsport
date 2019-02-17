void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //string str = Serial.readStringUntil('\n');

  Serial.println(5432);

  Serial.println(463);

  delay(100);
}
