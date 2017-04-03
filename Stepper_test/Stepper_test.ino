const int a=8;
const int b=9;
const int c=10;
const int d=11;
int fixed = 4;


void setup(){
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
}

void loop(){
  digitalWrite(a,HIGH);
  delay(fixed);
  digitalWrite(a,LOW);
  digitalWrite(b,HIGH);
  delay(fixed);
  digitalWrite(b,LOW);
  digitalWrite(c,HIGH);
  delay(fixed);
  digitalWrite(c,LOW);
  digitalWrite(d,HIGH);
  delay(fixed);
  digitalWrite(d,LOW);
}
