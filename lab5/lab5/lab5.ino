#include "configuration.h"

unsigned long milliseconds=0;
unsigned long interval=0;

/*motor control*/
int getDistance(int trigPin, int echoPin)
{
  long duration;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  duration = duration / 59;
  if ((duration < 2) || (duration > 300)) return false;
  return duration;
}
void go_Advance(void)  //Forward
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL,LOW);
  digitalWrite(dir1PinR,HIGH);
  digitalWrite(dir2PinR,LOW);
}
void go_Left(void)  //Turn left
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL,LOW);
  digitalWrite(dir1PinR,LOW);
  digitalWrite(dir2PinR,HIGH);
}
void go_Right(void)  //Turn right
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,HIGH);
  digitalWrite(dir1PinR,HIGH);
  digitalWrite(dir2PinR,LOW);
}
void go_Back(void)  //Reverse
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,HIGH);
  digitalWrite(dir1PinR,LOW);
  digitalWrite(dir2PinR,HIGH);
}
void stop_Stop()    //Stop
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,LOW);
  digitalWrite(dir1PinR,LOW);
  digitalWrite(dir2PinR,LOW);
}

/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL,speed_L); 
  analogWrite(speedPinR,speed_R);   
}

//Pins initialize
void init_GPIO()
{
  pinMode(dir1PinL, OUTPUT); 
  pinMode(dir2PinL, OUTPUT); 
  pinMode(speedPinL, OUTPUT);  
 
  pinMode(dir1PinR, OUTPUT);
  pinMode(dir2PinR, OUTPUT); 
  pinMode(speedPinR, OUTPUT); 
  stop_Stop();
}



void setup()
{
 
}

  void logarithm(){
    
   set_Motorspeed(255*0.6,255*0.5);
  go_Advance();
  delay(1200);
  
  set_Motorspeed(255*0.7,255*0.2);
  delay(1500);
  
  set_Motorspeed(255*0.65, 255*0.5);
  delay(3700);
  stop_Stop();
 
}


void loop(){
  if(getDistance(A5,A4)<30){
     go_Right();
    delay(500);
  }
 milliseconds=millis()-interval;
  if(milliseconds<=100){
    set_Motorspeed(255*0.6,255*0.5);
  go_Advance();
  }
  if(milliseconds<=1200&&milliseconds>=100){
     set_Motorspeed(255*0.7,255*0.2);
      go_Advance();
  }
  if(milliseconds<=2800&&milliseconds>1200){
   set_Motorspeed(255*0.65, 255*0.5);
    go_Advance();
  }

  if(milliseconds<=6500&&milliseconds>2800){
   set_Motorspeed(255*0.65, 255*0.5);
    go_Advance();
  }
  if(milliseconds<=7600&&milliseconds>6500){
   set_Motorspeed(255*0.6,255*0.6);
  go_Right();
  }
   if(milliseconds<=13500&&milliseconds>7600){
 go_Advance();
  }
  if(milliseconds<=14600&&milliseconds>13500){
 go_Right();
  }
    if(milliseconds<=16000&&milliseconds>14600){
 go_Advance();
  }
  
  
  if(milliseconds>16100){
    interval=millis();
  }
  
  
 
  
  
}
