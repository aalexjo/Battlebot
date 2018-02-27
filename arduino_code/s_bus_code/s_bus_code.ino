#include <FUTABA_SBUS.h>
#include <Servo.h> 
#include <SoftwareSerial.h>

SoftwareSerial mserial(10, 11); // RX, TX
FUTABA_SBUS sBus;
Servo myservo;

//pin setup
int INT_0 = 2; 
int INT_1 = 3; 
int enA = 4; 
int enB = 5;
int IN1 = 6; 
int IN2 = 7; 
int IN3 = 1; 
int IN4 = 0; 

enum motor_dir{
  FORWARD,
  BACKWARD,
};

int motor_vel = 0; 
int diff_vel = 0; 

void setup(){
  mserial.begin(115200);
  sBus.begin();
  myservo.attach(14); 
   //set Arduinopins
  pinMode(INT0, INPUT);     
  pinMode(INT1, INPUT); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  
}

void loop(){
  delay(100);
  sBus.FeedLine();
  if (sBus.toChannels == 1){
    sBus.UpdateServos();
    sBus.UpdateChannels();
    sBus.toChannels = 0;

    motor_vel = sBus.channels[1]; 
    motor_set_vel(motor_vel);
    diff_vel = sBus.channels[2]; 
    motor_set_dir(diff_vel); 
    
    
    mserial.print(sBus.channels[1]);
    mserial.print("\t");
    mserial.print(sBus.channels[2]);
    mserial.print("\t");
    mserial.print(sBus.channels[3]);
    mserial.print("\t");
    mserial.println(sBus.channels[4]);
    myservo.writeMicroseconds(sBus.channels[6]+800);
  }
}

void motor_set_vel(int motor_vel){
  if (motor_vel > 127){
    set_dir(FORWARD);
    motor_vel = map(motor_vel, 128, 255, 0, 255);
  }else{
    set_dir(BACKWARD);
    motor_vel = map(motor_vel, 0, 127, 0, 255); 
  }
  //Set speed on motors
  analogWrite(enA, motor_vel);
  analogWrite(enB, motor_vel);
}

void motor_set_dir(int diff_vel){
  if(diff_vel > 130){
    analogWrite(enA, 200); 
    //turn right
  }else if(diff_vel < 124){
    analogWrite(enB, 200); 
    //turn left
  }
 }

void set_dir(motor_dir dir){
  if (dir == FORWARD){
    //set directions sjekk opp i H-bro
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }else{
    //set other directions
    digitalWrite(IN1, LOW);
    digitalWrite(IN1, LOW);
  }
}





