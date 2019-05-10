#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "
#include <Servo.h>
Servo liftservo;

#define LIFTSERVO   5
#define MOTORLEFT_1 6
#define MOTORLEFT_2 9
#define MOTORRIGHT_1 10
#define MOTORRIGHT_2 11
#define LEFTSENS 7
#define RIGHTSENS 4
#define BACKSENS 8
#define LINEFOUNDLEFT 0
#define LINEFOUNDRIGHT 1
#define ROBOTFOUND 2
#define SEARCH 3
#define LINEFOUNDBACK 4
//analogread 5 and 4 are the distance sensor
int distance;
int state = SEARCH;

long timeThis, timeLast;


void forward() {
  analogWrite(MOTORLEFT_1, 255);
  digitalWrite(MOTORLEFT_2, LOW);
  analogWrite(MOTORRIGHT_1, 255);
  digitalWrite(MOTORRIGHT_2, LOW);
  delay(100);
  digitalWrite(MOTORLEFT_1, LOW);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void reverse() {
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 255);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 255);
  delay(100);
  digitalWrite(MOTORLEFT_1, LOW);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void left() {
  analogWrite(MOTORLEFT_1, 255);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 255);
  delay(250);
  digitalWrite(MOTORLEFT_1, LOW);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void right() {
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 255);
  analogWrite(MOTORRIGHT_1, 255);
  digitalWrite(MOTORRIGHT_2, LOW);
  delay(250);
  digitalWrite(MOTORLEFT_1, LOW);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void setup() {
  Serial.begin(9600);
  liftservo.attach(LIFTSERVO);
  pinMode(MOTORLEFT_1, OUTPUT);
  pinMode(MOTORLEFT_2, OUTPUT);
  pinMode(MOTORRIGHT_1, OUTPUT);
  pinMode(MOTORRIGHT_2, OUTPUT);
  pinMode(LEFTSENS, INPUT);
  pinMode(RIGHTSENS, INPUT);
  pinMode(BACKSENS, INPUT);
  Serial.print("Setup Done");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  Serial.print(distance);
  Serial.print("\t");
  Serial.println(state);
  switch (state) {
    case LINEFOUNDLEFT:
      reverse();
      right();
      if (digitalRead(LEFTSENS) == HIGH) {
        state = LINEFOUNDLEFT;
      }
      if (digitalRead(RIGHTSENS) == HIGH) {
        state = LINEFOUNDRIGHT;
      }
      if (digitalRead(BACKSENS) == HIGH) {
        state = LINEFOUNDBACK;
      }
      else {
        state = SEARCH;
      }
      break;
    case LINEFOUNDRIGHT:
      reverse();
      left();
      if (digitalRead(LEFTSENS) == HIGH) {
        state = LINEFOUNDLEFT;
      }
      if (digitalRead(RIGHTSENS) == HIGH) {
        state = LINEFOUNDRIGHT;
      }
      if (digitalRead(BACKSENS) == HIGH) {
        state = LINEFOUNDBACK;
      }
      else {
        state = SEARCH;
      }
      break;
    case LINEFOUNDBACK:
      forward();
      if (digitalRead(LEFTSENS) == HIGH) {
        state = LINEFOUNDLEFT;
      }
      if (digitalRead(RIGHTSENS) == HIGH) {
        state = LINEFOUNDRIGHT;
      }
      if (digitalRead(BACKSENS) == HIGH) {
        state = LINEFOUNDBACK;
      }
      else {
        state = SEARCH;
      }
      break;
    case ROBOTFOUND:
      forward();
      if (digitalRead(LEFTSENS) == HIGH) {
        state = LINEFOUNDLEFT;
      }
      if (digitalRead(RIGHTSENS) == HIGH) {
        state = LINEFOUNDRIGHT;
      }
      if (digitalRead(BACKSENS) == HIGH) {
        state = LINEFOUNDBACK;
      }
      if (distance > 1000) {
        state = SEARCH;
      }
      if (distance <= 100) {
        //liftservo.write(170);
      }
      break;
    case SEARCH:
      if (digitalRead(LEFTSENS) == HIGH) {
        state = LINEFOUNDLEFT;
      }
      if (digitalRead(RIGHTSENS) == HIGH) {
        state = LINEFOUNDRIGHT;
      }
      if (digitalRead(BACKSENS) == HIGH) {
        state = LINEFOUNDBACK;
      }
      if (distance <= 1000) {
        state = ROBOTFOUND;
      }
      else {
        right();
        forward();
        left();
        forward();
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
