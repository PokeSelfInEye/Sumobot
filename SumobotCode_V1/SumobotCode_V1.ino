#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "
#include <Servo.h>
Servo liftservo;



#define LIFTSERVO   5
#define MOTORLEFT_1 6
#define MOTORLEFT_2 9
#define MOTORRIGHT_1 10
#define MOTORRIGHT_2 11

#define LINEFOUNDLEFT 0
#define LINEFOUNDRIGHT 1
#define ROBOTFOUND 2
#define SEARCH 3
//analogread 5 and 4 are the distance sensor
int distance;
int state = SEARCH;
int leftsens = analogRead(1);
int rightsens = analogRead(2);
int backsens = analogRead(3);

void forward() {
  analogWrite(MOTORLEFT_1, 255);
  digitalWrite(MOTORLEFT_2, LOW);
  analogWrite(MOTORRIGHT_1, 255);
  digitalWrite(MOTORRIGHT_2, LOW);
  delay(500);
}

void reverse() {
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 255);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 255);
  delay(500);
}

void left() {
  analogWrite(MOTORLEFT_1, 255);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 255);
  delay(500);
}

void right() {
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 255);
  analogWrite(MOTORRIGHT_1, 255);
  digitalWrite(MOTORRIGHT_2, LOW);
  delay(500);
}

void setup() {
  liftservo.attach(LIFTSERVO);
  pinMode(MOTORLEFT_1, OUTPUT);
  pinMode(MOTORLEFT_2, OUTPUT);
  pinMode(MOTORRIGHT_1, OUTPUT);
  pinMode(MOTORRIGHT_2, OUTPUT);
  Serial.begin(9600);
  //delay(5000); // startup delay
  //liftservo.write(0); //setting servo to proper position
  lox.begin();
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  Serial.print(distance);
  Serial.print("/t");6+98

  
  Serial.println(state);
  switch (state) {
    case LINEFOUNDLEFT:
      reverse;
      right;
      state = SEARCH;
      break;
    case LINEFOUNDRIGHT:
      reverse;
      left;
      state = SEARCH;
      break;
    case ROBOTFOUND:
      forward;
      if 
      break;
    case SEARCH:
      if (leftsens > 500) {
        state = LINEFOUNDLEFT;
      }
      if (rightsens > 500) {
        state = LINEFOUNDRIGHT;
      }
      if (distance < 1000) {
        state = ROBOTFOUND;
      }
  }

  /*  if (distance <= 50) {
      liftservo.write(170);
    }
    else {
      liftservo.write(0);
    }
  */
}
