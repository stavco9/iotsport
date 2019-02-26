const int green1 = 10;
const int green2 = 11;
const int yellow = 12;
const int red = 13;

int c;

void setup() {
  // put your setup code here, to run once:
  pinMode(green1, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  c=0;

  // simulation
  // wifi...
  for(int i=0;i<3;i++) {
  digitalWrite(yellow, HIGH);
  delay(500);
  digitalWrite(yellow, LOW);
  delay(500);
  }
  //before
  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
  digitalWrite(green1, HIGH);
  delay(1000);
  digitalWrite(green1, LOW);
}

void loop() {
  // mid
  if((c+20)%40==0) {
  digitalWrite(green1, HIGH);
  digitalWrite(green2, LOW);
  }
  if(c%40==0) {
  digitalWrite(green1, LOW);
  digitalWrite(green2, HIGH);
  }

  // before
  /*digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
  digitalWrite(green1, HIGH);
  delay(1000);
  digitalWrite(green1, LOW);*/

  if(c==2000) {
  // after
  for(int i=0;i<3;i++) {
  digitalWrite(green1, HIGH);
  digitalWrite(green2, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(red, HIGH);
  delay(100);
  digitalWrite(green1, LOW);
  digitalWrite(green2, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  delay(100);
  }
  delay(5000);
  c=0;
  }

  // waiting for wifi
  //....while wifi not connected
  /*for(int i=0;i<3;i++) {
  digitalWrite(yellow, HIGH);
  delay(500);
  digitalWrite(yellow, LOW);
  delay(500);
  }*/
  
  c++;
  delay(10);
}
