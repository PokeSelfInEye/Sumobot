#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "

#define LIFTSERVO   5
#define MOTORLEFT_1 6
#define MOTORLEFT_2 5
#define MOTORRIGHT_1 10
#define MOTORRIGHT_2 9
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
#define SPEED 150
#define TURNSPEED 50
int distance;
int state = SEARCH;
int direct;

long timeThis, timeLast;

void forward() {
  Serial.print("forward");
  analogWrite(MOTORLEFT_1, SPEED);
  digitalWrite(MOTORLEFT_2, LOW);
  analogWrite(MOTORRIGHT_1, SPEED);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void reverse() {
  Serial.print("reverse");
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, SPEED);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, SPEED);
}

void left() {
  Serial.print("left");
  analogWrite(MOTORLEFT_1, TURNSPEED);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  analogWrite(MOTORRIGHT_2, TURNSPEED);
}

void right() {
  Serial.print("right");
  digitalWrite(MOTORLEFT_1, LOW);
  analogWrite(MOTORLEFT_2, 70);
  analogWrite(MOTORRIGHT_1, 70);
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
  //delay(5000);
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


      if (analogRead(0) < 200) {
        reverse();
      }
      if (analogRead(1) < 200) {
        reverse();
      }
      if (distance <= 400) {
        forward();
      }
      if ((analogRead(1) > 200) && (analogRead(0) > 200)  &&  (distance > 400))  {
        if (timeThis - timeLast < 1000) {
          left();
        }
        if ((timeThis - timeLast > 1000) && (timeThis - timeLast < 2000)) {
          right();
        }
        if ( timeThis - timeLast > 2000) {
          timeLast = timeThis;
        }
        else {
          cease;
          state = SEARCH;
        }
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
      if (timeThis - timeLast <= 500) {

        forward();

      }
      else {
        cease();
        state = SEARCH;
      }
      if (direct == SEARCH) {
        timeLast = timeThis;
        state = LEFT;
      }
      else if ((analogRead(1) > 500) && (analogRead(0) > 500) &&  (distance > 1000))  {
        // Serial.print(direct);
        // Serial.print("GOING TO SEARCH");
        timeLast = timeThis;
        state = SEARCH;
      }


      break;


    case REVERSE:


      Serial.print("IN REVERSE");
      if (timeThis - timeLast <= 1000) {
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
      else if ((direct != LEFT) && (direct != RIGHT)) {
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
      else if (distance > 1000) {
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
      else if (distance > 1000)  {
        cease();
        timeLast = timeThis;
        state = SEARCH;
      }


      break;
  }
}
