// int trigPin=32;
// int echoPin=33;
// int pingTravelTime;
// void setup() {
//   // put your setup code here, to run once:
// pinMode(trigPin,OUTPUT);
// pinMode(echoPin,INPUT);
// Serial.begin(9600);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// digitalWrite(trigPin,LOW);
// delayMicroseconds(10);
// digitalWrite(trigPin,HIGH);
// delayMicroseconds(10);
// digitalWrite(trigPin,LOW);
// pingTravelTime=pulseIn(echoPin,HIGH);
// delay(1000);
// Serial.println(pingTravelTime);
// }




#include <ESP32Servo.h>
int servopin = 18;
int servopos;
Servo myservo;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(servopin);
  // to attach a object to the arduino we use attach
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("What is the angle you want to tilt?");
  while (Serial.available() == 0) {
  }
  servopos = Serial.parseInt();
  // servopos=0;
  // myservo.write(servopos);
  // delay(500);
  // servopos=180;
  myservo.write(servopos);
  delay(500);
  // uss object se kam karne ke liye use write
}
