
/***
Code Ducumentation and usage:



Author: Abhayjeet Singh Juneja
Date: March 13th 2017
***/

#include<SoftwareSerial.h>

SoftwareSerial esp (3,4); //RX,TX

//status macros
#define LEFT -1
#define CENTER 0
#define RIGHT 1

//pin variables for front motor
const int mf_a=9;
const int mf_b=10;
const int mf_c=11;
const int mf_d=12;

//pin variables for back motor
const int mb_a=5;
const int mb_b=6;
const int mb_c=7;
const int mb_d=8;

//delay between motor phases - using 4 phase stepper motor
int del_ay = 4;

//divider global status
int newStatus = CENTER;
int currentStatus = CENTER;

//function prototypes
int initWifi();
void getResponse();
void motorLeftRotation();
void motorRightRotation();
void moveDividerLeftOneLane();
void moveDividerRightOneLane();
int statusChange();
void updateDivider();


void setup(){

  //set front motor pins to OUTPUT
  pinMode(mf_a,OUTPUT);
  pinMode(mf_b,OUTPUT);
  pinMode(mf_c,OUTPUT);
  pinMode(mf_d,OUTPUT);

  //set front motor pins to OUTPUT
  pinMode(mb_a,OUTPUT);
  pinMode(mb_b,OUTPUT);
  pinMode(mb_c,OUTPUT);
  pinMode(mb_d,OUTPUT);

  if(!initWifi()){
    Serial.println("Error at ESP initialization!")
  }
}

void loop(){
  talkToAPI();
  if(statusChange()){
    updateDivider();
  }
}

void ATResponse(){
  while(!(esp.available()>0));
  while(esp.available()>0){
    Serial.write(esp.read());
  }
}

void getResponse(){
  while(!(esp.available()>0));
  char st[7];
  int len=0;
  while(esp.available()>0){
    st[len++]=esp.read();
  }
  newStatus=st;
}


int initWifi(){
  esp.println("AT");
  ATResponse();
  delay(100);

  esp.println("AT+CWJAP=\"don\'t even\", \"Password\"");
  ATResponse();
  delay(100);
}

void talkToAPI(){
  esp.println("AT+CIPSTART=\"TCP\", \"https://traffic-capstone.run.aws-usw02-pr.ice.predix.io\", \"80\"");
  ATResponse();
  delay(100);

  esp.println("AT+CIPSEND=20");
  ATResponse();
  delay(100);

  esp.println("GET /dividerStatus")
  getResponse();
}

void motorLeftRotation(){

  //phase A High - both motors
  digitalWrite(mf_a,HIGH);
  digitalWrite(mb_a,HIGH);
  delay(del_ay);

  //Phase A Low, Phase B High
  digitalWrite(mf_a,LOW);
  digitalWrite(mb_a,LOW);
  digitalWrite(mf_b,HIGH);
  digitalWrite(mb_b,HIGH);
  delay(del_ay);

  //Phase B Low, Phase C High
  digitalWrite(mf_b,LOW);
  digitalWrite(mb_b,LOW);
  digitalWrite(mf_c,HIGH);
  digitalWrite(mb_c,HIGH);
  delay(del_ay);

  //Phase C Low, Phase D High
  digitalWrite(mf_c,LOW);
  digitalWrite(mb_c,LOW);
  digitalWrite(mf_d,HIGH);
  digitalWrite(mb_d,HIGH);
  delay(del_ay);

  //Phase D Low
  digitalWrite(mf_d,LOW);
  digitalWrite(mb_d,LOW);
}

void motorRightRotation(){

  //phase D High - both motors
  digitalWrite(mf_d,HIGH);
  digitalWrite(mb_d,HIGH);
  delay(del_ay);

  //Phase D Low, Phase C High
  digitalWrite(mf_d,LOW);
  digitalWrite(mb_d,LOW);
  digitalWrite(mf_c,HIGH);
  digitalWrite(mb_c,HIGH);
  delay(del_ay);

  //Phase C Low, Phase B High
  digitalWrite(mf_c,LOW);
  digitalWrite(mb_c,LOW);
  digitalWrite(mf_b,HIGH);
  digitalWrite(mb_b,HIGH);
  delay(del_ay);

  //Phase B Low, Phase A High
  digitalWrite(mf_b,LOW);
  digitalWrite(mb_b,LOW);
  digitalWrite(mf_a,HIGH);
  digitalWrite(mb_a,HIGH);
  delay(del_ay);

  //Phase A Low
  digitalWrite(mf_a,LOW);
  digitalWrite(mb_a,LOW);
}


void moveDividerLeftOneLane(){
  int i;
  //one full revolution
  for(i=0;i<32;i++){
    motorLeftRotation();
  }
}

void moveDividerRightOneLane(){
  int i;
  //one full revolution
  for(i=0;i<32;i++){
    motorRightRotation();
  }
}

void updateDivider(){

  if(currentStatus==CENTER){
    if(newStatus==LEFT){
      moveDividerLeftOneLane();
      currentStatus=LEFT;
    }
    if(newStatus==RIGHT){
      moveDividerRightOneLane();
      currentStatus=RIGHT;
    }
  }

  if(currentStatus==LEFT){
    if(newStatus==CENTER){
      moveDividerRightOneLane();
      currentStatus=CENTER;
    }
    if(newStatus==RIGHT){
      moveDividerRightOneLane();
      moveDividerRightOneLane();
      currentStatus=RIGHT;
    }
  }

  if(currentStatus==RIGHT){
    if(newStatus==CENTER){
      moveDividerLeftOneLane();
      currentStatus=CENTER;
    }
    if(newStatus==LEFT){
      moveDividerLeftOneLane();
      moveDividerLeftOneLane();
      currentStatus=LEFT;
    }
  }
}

int statusChange(){

  //call esp8266 function to update new status
  if(newStatus==currentStatus){
    return 0;
  }
  else{
    return 1;
  }
}
