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
  analogWrite(MOTORLEFT_1, 150);
  digitalWrite(MOTORLEFT_2, LOW);
  analogWrite(MOTORRIGHT_1, 150);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void reverse() {
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 150);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 150);
}

void left() {
  analogWrite(MOTORLEFT_1, 150);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, 150);
}

void right() {
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 150);
  analogWrite(MOTORRIGHT_1, 150);
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

  switch (state) {


    case SEARCH:


      if (analogRead(0) <= 500) {
        timeLast = timeThis;
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        timeLast = timeThis;
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        timeLast = timeThis;
        state = LINEFOUNDBACK;
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


      if (analogRead(0) <= 500) {
        timeLast = timeThis;
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        timeLast = timeThis;
        state = LINEFOUNDRIGHT;
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
        timeLast = timeThis;
        state = SEARCH;
      }


      break;


    case REVERSE:


      if (analogRead(2) <= 500) {
        timeLast = timeThis;
        state = LINEFOUNDBACK;
      }
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
        state = ROBOTFOUND;
      }
      if (timeThis - timeLast <= 750) {
        left();
      }
      else {
        cease();
      }
      if (direct == SEARCH) {
        timeLast = timeThis;
        state = FORWARD;
      }
      else {
        timeLast = timeThis;
        state = SEARCH;
      }


      break;


      case RIGHT:


      if (distance <= 1000) {
        timeLast = timeThis;
        state = ROBOTFOUND;
      }
      if (timeThis - timeLast <= 750) {
        right();

      }
      else {
        cease();
      }
      if (direct == SEARCH) {
        timeLast = timeThis;
        state = LEFT;
      }
      else {
        timeLast = timeThis;
        state = SEARCH;
      }


      break;


       case LINEFOUNDLEFT:


      direct = LEFT;
      timeLast = timeThis;
      state = REVERSE;


      break;


    case LINEFOUNDRIGHT:


      direct = RIGHT;
      timeLast = timeThis;
      state = REVERSE;


      break;


      case LINEFOUNDBACK:


        direct = REVERSE;
        timeLast = timeThis;
        state = SEARCH;


      break;
  }
}
