//interrupt variables
volatile int pwm_value_0 = 0; 
volatile int timer_0 = 0; 
volatile int pwm_value_1 = 0; 
volatile int timer_1 = 0; 
volatile int motor_vel = 0; 
volatile int diff_vel = 0; 
 
int min_pulse_time = 500; //micros NB!
int max_pulse_time = 2500;


enum motor_dir{
  FORWARD,
  BACKWARD,
};


//pin setup
int INT_0 = 2; 
int INT_1 = 3; 
int enA = 4; 
int enB = 5;
int IN1 = 6; 
int IN2 = 7; 
int IN3 = 1; 
int IN4 = 0; 

void setup() {
  //set Arduinopins
  pinMode(INT0, INPUT);     
  pinMode(INT1, INPUT); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  //enable interrupt
  attachInterrupt(digitalPinToInterrupt(INT0), ISR0_velocity, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INT1), ISR1_diff, CHANGE);  

}

void loop() {
  //Full speed motor A 
  digitalWrite(2, HIGH); 
  digitalWrite(3, LOW);
}



void set_dir(motor_dir dir){
  if (dir == FORWARD){
    //set directions 
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }else{
    //set other directions
    digitalWrite(IN4, HIGH);
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
  if(motor_vel > 130){
    analogWrite(enA, 200); 
    //turn right
  }else if(motor_vel < 124){
    analogWrite(enB, 200); 
    //turn left
  }
 }


void ISR0_velocity(){
  if (digitalRead(INT0) == HIGH){
    timer_0 = micros();
  }
  else {
    if(timer_0!= 0){
      pwm_value_0 = micros() - timer_0; 
      timer_0 = 0; 
    }
  }

  motor_vel = (pwm_value_0 - min_pulse_time)/(max_pulse_time - min_pulse_time); //scaled from 0-255
  motor_set_vel(motor_vel); 
}



void ISR1_diff(){
  if (digitalRead(INT1) == HIGH){
    timer_1 = micros();
  }
  else {
    if(timer_1!= 0){
      pwm_value_1 = micros() - timer_1; 
      timer_1 = 0; 
    }
  }

  diff_vel = (pwm_value_1 - min_pulse_time)/(max_pulse_time - min_pulse_time);
  motor_set_dir(diff_vel);
}


