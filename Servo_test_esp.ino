#include <ESP32Servo.h>
#include <math.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

const int SERVO1_PIN = 12;
const int SERVO2_PIN = 13;
const int SERVO3_PIN = 14;
const int SERVO4_PIN = 15;
const int IO2_PIN = 2; 

const int BIG_MIN_POS = 130;
const int BIG_MAX_POS = 165;

unsigned long lastServo4 = 0;
bool servo4Position = false;

unsigned long lastIO2Toggle = 0;
bool io2State = LOW;

const float SWEEP_SPEED = 0.002; 

void setup() {
  
  pinMode(IO2_PIN, OUTPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);
  servo4.setPeriodHertz(50);

  servo1.attach(SERVO1_PIN, 500, 2500);
  servo2.attach(SERVO2_PIN, 500, 2500);
  servo3.attach(SERVO3_PIN, 500, 2500);

  servo4.attach(SERVO4_PIN, 1000, 2000);
}

void loop() {
  unsigned long currentMillis = millis();

  float centerPos = (BIG_MIN_POS + BIG_MAX_POS) / 2.0;
  float amplitude = (BIG_MAX_POS - BIG_MIN_POS) / 2.0;

  float timeFactor = currentMillis * SWEEP_SPEED;

  int pos1 = centerPos + (amplitude * sin(timeFactor));
  
  int pos2 = centerPos + (amplitude * sin(timeFactor + 2.094));
  
  int pos3 = centerPos + (amplitude * sin(timeFactor + 4.189));

  servo1.write(pos1);
  servo2.write(pos2);
  servo3.write(pos3);

  if (currentMillis - lastServo4 >= 1000) {
    lastServo4 = currentMillis;
    servo4Position = !servo4Position;

    if (servo4Position) {
      servo4.write(180);
    } else {
      servo4.write(0);
    }
  }

  if (currentMillis - lastIO2Toggle >= 1000) {
    lastIO2Toggle = currentMillis;
    io2State = !io2State;
    digitalWrite(IO2_PIN, io2State);
  }

  delay(15); 
}