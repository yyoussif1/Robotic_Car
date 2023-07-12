#include <SPI.h> 
#include <MFRC522.h>
#define SS_PIN 10 
#define RST_PIN A0 
#define in1 2 
#define in2 4
#define in3 6
#define in4 7
#define enL 3
#define enR 5
#define trig 9
#define echo 8
#define buzzer A1
#define G_LED A2
#define R_LED A3
MFRC522 rfid(SS_PIN, RST_PIN);
int distance=0,t=0,vSpeed=150; 
char state =0;
boolean unlocked =false;

void setup() {
pinMode(in2,OUTPUT);
Serial.begin(9600);
SPI.begin();
rfid.PCD_Init();
pinMode(in1,OUTPUT);
pinMode(in3,OUTPUT); 
pinMode(in4,OUTPUT); 
pinMode(enL,OUTPUT); 
pinMode(enR,OUTPUT); 
pinMode(trig,OUTPUT); 
pinMode(buzzer,OUTPUT); 
pinMode(echo,INPUT);
pinMode(R_LED,OUTPUT);
pinMode(G_LED,OUTPUT);
}

void loop() {
if(unlocked==true){ 
  digitalWrite(trig,LOW);
  delayMicroseconds(5);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
   t=pulseIn(echo,HIGH);
   distance=(t/2)*0.035;
if(distance<30){
   stop();
   delay(500);
 backward();
 delay(500);
 stop();
 state='0';
}
if(Serial.available()){ 
state=Serial.read();
switch(state){
  case '0':
vSpeed=0;
    break;
  case '1':
vSpeed=100;
    break;
  case '2':
vSpeed=180;
break;
case '3':
vSpeed=200;
  break;
case '4':
vSpeed=255;
  break;
case 'F':
  forward();
  delay(35);
  stop();
  state='0';
  break;
case 'G':
  forwardL();
  delay(35);
  stop();
  state='0';
  break;
case 'I':
  forwardR();
  delay(35);
  stop();
  state='5';
  break;
case 'B':
  backward();
  delay(35);
  stop();
  state='0';
  break;
case 'H':
  backwardL();
  delay(35);
  stop();
  state='0';
    break;
case 'J':
  backwardR();
  delay(35);
  stop();
  state='0';
  break;
case 'R':
  right();
  delay(35);
  stop();
  state='0';
  break;
case 'L':
  left();
  delay(34);
  stop();
  state='0';
  break;
case 'S':
  stop();
  state='0';
  break;
case 'X':
  unlocked=false;
  state='0';
  break;
case 'V': digitalWrite(buzzer,HIGH);
  state='0';
  break;
case 'v':
  digitalWrite(buzzer,LOW); state='0';
  break;
      default:
        stop();
        state='0';
        break;
}
} }
}
void backward(){
analogWrite(enL,vSpeed); 
digitalWrite(in1,HIGH); 
digitalWrite(in2,LOW); 
digitalWrite(in3,HIGH); 
digitalWrite(in4,LOW);
analogWrite(enR,vSpeed); 
}
void backwardL(){
analogWrite(enR,vSpeed);
analogWrite(enL,vSpeed); 
digitalWrite(in1,HIGH); 
digitalWrite(in2,LOW); 
digitalWrite(in3,LOW); 
digitalWrite(in4,LOW);
}
void backwardR(){
analogWrite(enR,vSpeed); 
analogWrite(enL,vSpeed); 
digitalWrite(in1,LOW); 
digitalWrite(in2,LOW); 
digitalWrite(in3,HIGH); 
digitalWrite(in4,LOW);
}
void forward(){
analogWrite(enR,vSpeed); 
analogWrite(enL,vSpeed); 
digitalWrite(in1,LOW); 
digitalWrite(in2,HIGH); 
digitalWrite(in3,LOW); 
digitalWrite(in4,HIGH);
}
void forwardL(){
analogWrite(enR,vSpeed); 
analogWrite(enL,vSpeed);
digitalWrite(in1,LOW); 
digitalWrite(in2,HIGH); 
digitalWrite(in3,LOW); 
digitalWrite(in4,LOW);
}
void forwardR(){
analogWrite(enR,vSpeed); 
analogWrite(enL,vSpeed); 
digitalWrite(in1,LOW); 
digitalWrite(in2,LOW); 
digitalWrite(in3,LOW);
digitalWrite(in4,HIGH); 
}
void left(){
analogWrite(enR,vSpeed); 
analogWrite(enL,vSpeed); 
digitalWrite(in1,HIGH); 
digitalWrite(in2,LOW); 
digitalWrite(in3,LOW); 
digitalWrite(in4,HIGH);
}
void right(){
analogWrite(enR,vSpeed); 
analogWrite(enL,vSpeed); 
digitalWrite(in1,LOW); 
digitalWrite(in2,HIGH); 
digitalWrite(in3,HIGH); 
digitalWrite(in4,LOW);
}
void stop(){
analogWrite(enR,0); 
analogWrite(enL,0); 
digitalWrite(in1,LOW); 
digitalWrite(in2,LOW); 
digitalWrite(in3,LOW); 
digitalWrite(in4,LOW);
}

bool card_detected(){
if (!rfid.PICC_IsNewCardPresent()) return false;
if (!rfid.PICC_ReadCardSerial()) return false;
return true;
}

String card_id(){
  String id;
  for(byte i=0 ;i<4;i++)
  { id+=String(rfid.uid.uidByte[i],HEX);
   if(i<3)id+=' ';
  }
id.toUpperCase();
 rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
   return id;
}