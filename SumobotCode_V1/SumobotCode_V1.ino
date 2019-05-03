#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "
#include <Servo.h>
Servo liftservo;



#define lift_servo   5
#define motor_left_1 6
#define motor_left_2 9
#define motor_right_1 10
#define motor_right_2 11
//analogread 5 and 4 are the distance sensor
int distance;

void setup() {
  liftservo.attach(lift_servo);
  pinMode(motor_left_1, OUTPUT);
  pinMode(motor_left_2, OUTPUT);
  pinMode(motor_right_1, OUTPUT);
  pinMode(motor_right_2, OUTPUT);
  Serial.begin(9600);
  //delay(5000); // startup delay
  //liftservo.write(0); //setting servo to proper position
  lox.begin();
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  Serial.println(distance);
  if (distance > 50) {
    analogWrite(motor_left_1, 255);
    digitalWrite(motor_left_2, LOW);
    analogWrite(motor_right_1, 255);
    digitalWrite(motor_right_2, LOW);
  }
  else {
    digitalWrite(motor_left_1, LOW);
    digitalWrite(motor_left_2, LOW);
    digitalWrite(motor_right_1, LOW);
    digitalWrite(motor_right_2, LOW);
  }
/*  if (distance <= 50) {
    liftservo.write(170);
  }
  else {
    liftservo.write(0);
  }
  */
}
