# include <LiquidCrystal.h>
# include <Keypad.h>

LiquidCrystal lcd (13,12,11,10,9,8);
int in=A0; 
char a='0';
char b='0';
int count=0,i=0,k=0,rate=0;
int n=0;
char password[]={a,b};
char input [2];
unsigned long time2,time1;
unsigned long time;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] ={
           {'1','2','3','A'},
           {'4','5','6','B'},
           {'7','8','9','C'},
           {'*','0','#','D'}
};

byte rowPins [ROWS] = {7,6,5,4};
byte colPins [COLS] = {3,2,1,0};
Keypad mykeypad = Keypad(makeKeymap(keys), rowPins, colPins,4,4);

void setup() {
  lcd.begin (16,2);
  lcd.print ("Pulse Reader");
  
  delay (5000);
  lcd.clear ();
  lcd.print("Press Any Button");
  // put your setup code here, to run once:

}

void loop() {
 char key = mykeypad.getKey();
 n=0;
 if (key){
 while(n==0){

    while (n==0){
    lcd.clear();
    lcd.print ("Enter Gender");
      if (key='A'){
     lcd.print("Male");
     input[n]='A';
     n=n+1;
     }
    if (key='B'){
     lcd.clear();
     lcd.print("Female");
     input[n]='B';
     n=n+1;
    }
    else {
     lcd.clear();
     lcd.print("Try Again");
    }
    }
  while (n==1){
  lcd.clear();
  lcd.print ("Enter Age");
  if (key='1'){
    lcd.print("18-35 Yrs Old");
    input[n]='1';
    n=n+1;
  }
  if (key='2'){
    lcd.clear();
    lcd.print("36-55");
    input[n]='2';
    n=n+1;
  }
  }
  while (n==2){
  lcd.clear();
  lcd.print("Press *");
  if (key='*')
  {
    k=0;
    lcd.clear();
    lcd.print("Please wait.......");
    while(k<15)
    {
     if(digitalRead(in))
     {
      if(k==0)
      time1=millis();
      k++;
      while(digitalRead(in));
     }
    }
      time2=millis();
      rate=time2-time1;
      rate=rate/5;
      rate=75000/rate;
      lcd.clear();
      lcd.print("Heart Beat Rate:");
      lcd.setCursor(0,1);
      delay (30000);
      lcd.print(rate);
      lcd.print(" ");
      lcd.write(1);      
      k=0;
      rate=0;
    }

  }
  }
  }
}
