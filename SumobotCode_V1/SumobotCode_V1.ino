#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "






#define brake_servo   5
#define linesens_back 7
#define linesens_left 8
#define linesens_right 2
#define lift_servo 3
#define motor_left_1 6
#define motor_left_2 9
#define motor_right_1 10
#define motor_right_2 11
//analogread 5 and 4 are the distance sensor
int distance;

void setup() {
  Serial.begin(9600);
  delay(5000); // startup delay
  analogWrite(brake_servo, 0); //setting servo to proper position
  analogWrite(lift_servo, 0);  //             "
  lox.begin();
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  if (distance == 0) {
    analogWrite(motor_left_1, 255);
    digitalWrite(motor_left_2, LOW);
    analogWrite(motor_right_1, 255);
    digitalWrite(motor_right_2, LOW);
  }
}
