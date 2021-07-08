#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x23,16,2);

#define man1 A1
#define man2 A0
#define man3 13
#define man4 12
#define aut 11
#define sen 10
#define pres 9

#define sensor1 8
#define sensor2 7
#define sensor3 6
#define sensor4 5

#define dtb 3
#define dta 4
#define sw 2

#define out1 A2
#define out2 A3
#define out3 A6
#define out4 A7

unsigned int t1 = 1000;   //čas izpihovanja
unsigned int t2 = 10000;  //čas med izpihovanji avtomatsko
unsigned int t3 = 500;    //čas med izpihovanji ročno
unsigned int ontime;

int pos;

boolean another = 1;

void pressure(){
  while(digitalRead(pres)){
    lcd.clear();
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("OPOZORILO:");
    lcd.setCursor(0,1);
    lcd.print("PREMAJHEN PRITISK");
  }
  lcd.setBacklight(LOW);
  lcd.clear();
}

bool test_aut(){
  if(!digitalRead(aut)){
    return 1;
  }
  return 0;
}

bool test_sen(){
  if(!digitalRead(sen)){
    return 1;
  }
  return 0;
}

bool test_man(){
  if(!(digitalRead(man1) || digitalRead(man2) || digitalRead(man3) || digitalRead(man4))){
    return 1;
  }
  return 0;
}

void cisti1(){
  pressure();
  digitalWrite(out1, HIGH);
  delay(t1);
  digitalWrite(out1, LOW);
  delay(t3);
}

void cisti2(){
  pressure();
  digitalWrite(out2, HIGH);
  delay(t1);
  digitalWrite(out2, LOW);
  delay(t3);
}

void cisti3(){
  pressure();
  digitalWrite(out3, HIGH);
  delay(t1);
  digitalWrite(out3, LOW);
  delay(t3);
}

void cisti4(){
  pressure();
  digitalWrite(out4, HIGH);
  delay(t1);
  digitalWrite(out4, LOW);
  delay(t3);
}

void setup() {
  pinMode(man1, INPUT);
  pinMode(man2, INPUT);
  pinMode(man3, INPUT);
  pinMode(man4, INPUT);
  pinMode(aut, INPUT);
  pinMode(sen, INPUT);
  pinMode(pres, INPUT);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);

  pinMode(sw, INPUT);
  pinMode(dta, INPUT_PULLUP);
  pinMode(dtb, INPUT_PULLUP);

  lcd.init();
  lcd.setBacklight(LOW);
  lcd.clear();
}

void loop() {
  while(!digitalRead(man1) && !digitalRead(man2) && !digitalRead(man3) && !digitalRead(man4) && !digitalRead(aut) && !digitalRead(sen)){
    //nastavitev časov - meni
    if(digitalRead(sw)){
      lcd.setBacklight(HIGH);
      delay(100);
      ontime = millis();
      pos = 1;
      while(millis() < ontime + 30000){
        if(pos == 1){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("CAS IZPIHOVANJA");
          lcd.setCursor(0,1);
          lcd.print(t1);
          lcd.print("ms");
        }
        if(pos == 2){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("CAS MED IZPIHOVANJI");
          lcd.setCursor(0,1);
          lcd.print(t2);
          lcd.print("ms");
        }
        if(pos == 3){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("CAS PAVZE");
          lcd.setCursor(0,1);
          lcd.print(t3);
          lcd.print("s");
        }
        if(digitalRead(sw)){
          if(pos == 3){
            pos = 0;
          }
          ontime == millis();
          pos++;
          delay(100);
        }
        if(!digitalRead(dta)){
          if(pos == 1){
            t1 += 10;
          }
          if(pos == 2){
            t2 += 10;
          }
          if(pos == 3){
            t3 += 10;
          }
          ontime == millis();
        }
        if(!digitalRead(dtb)){
          if(pos == 1){
            t1 -= 10;
          }
          if(pos == 2){
            t2 -= 10;
          }
          if(pos == 3){
            t3 -= 10;
          }
          ontime == millis();
        }
    }
    lcd.clear();
    lcd.setBacklight(LOW);
    }
  }

  lcd.setBacklight(LOW);

  while(another && (digitalRead(man1) || digitalRead(man2) || digitalRead(man3) || digitalRead(man4))){
    //ročno proženje
    if(man1) cisti1();
    if(test_man()) break;
    if(man2) cisti2();
    if(test_man()) break;
    if(man3) cisti3();
    if(test_man()) break;
    if(man4) cisti4();
    another = 0;
  }
  another = 1;
  
  while(digitalRead(aut)){
    //avtomatsko proženje
    cisti1();
    if(test_aut()) break;
    cisti2();
    if(test_aut()) break;
    cisti3();
    if(test_aut()) break;
    cisti4();
    if(test_aut()) break;
    delay(t2);
  }
  
  while(digitalRead(sen)){
    //senzorsko proženje
    if(digitalRead(sensor1)) cisti1();
    if(test_sen()) break;
    if(digitalRead(sensor2)) cisti2();
    if(test_sen()) break;
    if(digitalRead(sensor3)) cisti3();
    if(test_sen()) break;
    if(digitalRead(sensor4)) cisti4();
  }

}
