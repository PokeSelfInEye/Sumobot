#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "

#define LIFTSERVO   5
#define MOTORLEFT_1 6
#define MOTORLEFT_2 9
#define MOTORRIGHT_1 10
#define MOTORRIGHT_2 5
#define LEFTSENS 7
#define RIGHTSENS 4
#define BACKSENS 8
#define LINEFOUNDLEFT 0
#define LINEFOUNDRIGHT 1
#define ROBOTFOUND 2
#define SEARCH 3
#define LINEFOUNDBACK 4
#define FORWARD 5
#define REVERSE 6
#define LEFT 7
#define RIGHT 8
int distance;
int state = SEARCH;
int direct;

long timeThis, timeLast;

void forward() {
  Serial.print("forward");
  analogWrite(MOTORLEFT_1, 45);
  digitalWrite(MOTORLEFT_2, LOW);
  analogWrite(MOTORRIGHT_1, 45);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void reverse() {
  Serial.print("reverse");
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 45);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 45);
}

void left() {
  Serial.print("left");
  analogWrite(MOTORLEFT_1, 45);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 45);
}

void right() {
  Serial.print("right");
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 45);
  analogWrite(MOTORRIGHT_1, 45);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void cease() {
  digitalWrite(MOTORLEFT_1, LOW);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(MOTORLEFT_1, OUTPUT);
  pinMode(MOTORLEFT_2, OUTPUT);
  pinMode(MOTORRIGHT_1, OUTPUT);
  pinMode(MOTORRIGHT_2, OUTPUT);
  Serial.print("Setup Done");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  timeThis = millis();
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  /*Serial.print(analogRead(0));
    Serial.print("\t");*/
  Serial.print(analogRead(1));
  Serial.print("\t");
  /*Serial.print(analogRead(2));
    Serial.print("\t");*/
  Serial.println(state);

  switch (state) {


    case SEARCH:


      if (analogRead(0) < 500) {
        direct = LEFT;
        timeLast = timeThis;
        state = REVERSE;
      }
      if (analogRead(1) < 500) {
        direct = RIGHT;
        Serial.print("less than 500");
        timeLast = timeThis;
        state = REVERSE;
      }
      if (distance <= 1000) {
        timeLast = timeThis;
        direct = ROBOTFOUND;
        state = FORWARD;
      }
      else {
        timeLast = timeThis;
        direct = SEARCH;
        state = FORWARD;
      }


      break;


    case FORWARD:


      if (analogRead(0) < 500) {
        direct = LEFT;
        timeLast = timeThis;
        state = REVERSE;
      }
      if (analogRead(1) < 500) {
        direct = RIGHT;
        timeLast = timeThis;
        state = REVERSE;
      }
      if ((distance > 1000) && (direct != SEARCH)) {
        timeLast = timeThis;
        state = SEARCH;
      }
      if (timeThis - timeLast <= 500) {

        forward();

      }
      else {
        cease();
      }
      if (direct == SEARCH) {
        timeLast = timeThis;
        state = LEFT;
      }
      else {
        // Serial.print(direct);
        // Serial.print("GOING TO SEARCH");
        timeLast = timeThis;
        state = SEARCH;
      }


      break;


    case REVERSE:


      Serial.print("IN REVERSE");
      if (timeThis - timeLast <= 500) {
        reverse();
      }
      else {
        cease();
      }
      if (direct == LEFT)  {
        timeLast = timeThis;
        state = RIGHT;
      }
      if (direct == RIGHT) {
        timeLast = timeThis;
        state = LEFT;
      }
      else {
        timeLast = timeThis;
        state = SEARCH;
      }


      break;


    case LEFT:


      if (distance <= 1000) {
        timeLast = timeThis;
        direct = ROBOTFOUND;
        state = FORWARD;
      }
      if (timeThis - timeLast <= 500) {
        left();
      }
      else {
        cease();
        timeLast = timeThis;
        state = SEARCH;
      }


      break;


    case RIGHT:


      if (distance <= 1000) {
        timeLast = timeThis;
        direct = ROBOTFOUND;
        state = FORWARD;
      }
      if (timeThis - timeLast <= 500) {
        right();

      }
      else {
        cease();
        timeLast = timeThis;
        state = SEARCH;
      }


      break;
  }
}
