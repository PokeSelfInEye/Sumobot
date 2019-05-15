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
//analogread 5 and 4 are the distance sensor
int distance;
int state = SEARCH;
int direct;

long timeThis, timeLast;

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
  timeThis = millis();
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  Serial.print(distance);
  Serial.print("\t");
  Serial.print(state);
  Serial.print("\t");
  Serial.print(analogRead(0));
  Serial.print("\t");
  Serial.print(analogRead(1));
  Serial.print("\t");
  Serial.print(analogRead(2));
  Serial.print("\t");
  Serial.println(state);
  
  switch (state) {
    case FORWARD:


      timeLast = timeThis;
      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      if (distance <= 1000) {
        state = ROBOTFOUND;
      }
      if (timeThis - timeLast <= 100) {

        analogWrite(MOTORLEFT_1, 200);
        digitalWrite(MOTORLEFT_2, LOW);
        analogWrite(MOTORRIGHT_1, 200);
        digitalWrite(MOTORRIGHT_2, LOW);
      }
      else {
        digitalWrite(MOTORLEFT_1, LOW);
        digitalWrite(MOTORLEFT_2, LOW);
        digitalWrite(MOTORRIGHT_1, LOW);
        digitalWrite(MOTORRIGHT_2, LOW);
      }
      if (direct = SEARCH) {
        state = LEFT;
      }
      else {
        state = SEARCH;
      }


      break;


    case REVERSE:


      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      if (distance <= 1000) {
        state = ROBOTFOUND;
      }
      if (timeThis - timeLast <= 100) {

        digitalWrite(MOTORLEFT_1, LOW);
        analogWrite(MOTORLEFT_2, 200);
        digitalWrite(MOTORRIGHT_1, LOW);
        analogWrite(MOTORRIGHT_2, 200);
      }
      else {
        digitalWrite(MOTORLEFT_1, LOW);
        digitalWrite(MOTORLEFT_2, LOW);
        digitalWrite(MOTORRIGHT_1, LOW);
        digitalWrite(MOTORRIGHT_2, LOW);
      }
      if (direct = LEFT)  {
        state = RIGHT;
      }
      if (direct = RIGHT) {
        state = LEFT;
      }
      else {
        state = SEARCH;
      }


      break;


    case LEFT:


      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      if (distance <= 1000) {
        state = ROBOTFOUND;
      }
      if (timeThis - timeLast <= 250) {

        analogWrite(MOTORLEFT_1, 200);
        digitalWrite(MOTORLEFT_2, LOW);
        digitalWrite(MOTORRIGHT_1, LOW);
        analogWrite(MOTORRIGHT_2, 200);
      }
      else {
        digitalWrite(MOTORLEFT_1, LOW);
        digitalWrite(MOTORLEFT_2, LOW);
        digitalWrite(MOTORRIGHT_1, LOW);
        digitalWrite(MOTORRIGHT_2, LOW);
      }
      if (direct = SEARCH) {
        state = FORWARD;
      }
      else {
        state = SEARCH;
      }


      break;


    case RIGHT:



      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      if (distance <= 1000) {
        state = ROBOTFOUND;
      }
      if (timeThis - timeLast <= 250) {

        digitalWrite(MOTORLEFT_1, LOW);
        analogWrite(MOTORLEFT_2, 200);
        analogWrite(MOTORRIGHT_1, 200);
        digitalWrite(MOTORRIGHT_2, LOW);
      }
      else {
        digitalWrite(MOTORLEFT_1, LOW);
        digitalWrite(MOTORLEFT_2, LOW);
        digitalWrite(MOTORRIGHT_1, LOW);
        digitalWrite(MOTORRIGHT_2, LOW);
      }
      if (direct = SEARCH) {
        state = LEFT;
      }
      else {
        state = SEARCH;
      }


      break;


    case LINEFOUNDLEFT:


      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      direct = LEFT;
      state = REVERSE;
      break;
    case LINEFOUNDRIGHT:

      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      else {
        state = SEARCH;
      }
      break;
    case LINEFOUNDBACK:
      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      else {
        state = SEARCH;
      }


      break;


    case ROBOTFOUND:


      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      if (distance > 1000) {
        state = SEARCH;
      }


      break;


    case SEARCH:


      direct = SEARCH;
      if (analogRead(0) <= 500) {
        state = LINEFOUNDLEFT;
      }
      if (analogRead(1) <= 500) {
        state = LINEFOUNDRIGHT;
      }
      if (analogRead(2) <= 500) {
        state = LINEFOUNDBACK;
      }
      if (distance <= 1000) {
        state = ROBOTFOUND;
      }
      break;
  }
}
