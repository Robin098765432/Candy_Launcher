#include <ESP32Servo.h>
#include <math.h>

Servo servo4;

const int SERVO4_PIN = 15;
const int IO2_PIN = 2; 
const int PWM_VALUE = 255;

void setup() {
  
  pinMode(IO2_PIN, OUTPUT);

  servo4.setPeriodHertz(50);
  servo4.attach(SERVO4_PIN, 500, 2500);
  servo4.write(0);
}

void loop() {
  analogWrite(IO2_PIN, PWM_VALUE);
  delay(300);
  servo4.write(180);
  delay(800);
  servo4.write(0);
  analogWrite(IO2_PIN, 0);
  delay(3000);
}