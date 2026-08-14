#include <ESP32Servo.h>
#include <math.h>

Servo servo4;

const int SERVO4_PIN = 15;
const int IO2_PIN = 2; 

void setup() {
  
  pinMode(IO2_PIN, OUTPUT);

  servo4.setPeriodHertz(50);
  servo4.attach(SERVO4_PIN, 500, 2500);
  servo4.write(0);

  digitalWrite(IO2_PIN, HIGH);
  delay(700);
  servo4.write(180);
  delay(500);
  digitalWrite(IO2_PIN, LOW);
  servo4.write(0);
  delay(700);
}

void loop() {

  
  
  delay(15); 
}