#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "

#define MOTORLEFT_1 6
#define MOTORLEFT_2 5
#define MOTORRIGHT_1 10
#define MOTORRIGHT_2 9
#define LEFTSENS 7
#define RIGHTSENS 4
#define SEARCH 0
#define FOUND 1
#define LINE 2
#define SPEED 150
#define TURNSPEED 50
#define LEFT 0
#define RIGHT 1

int distance;
int state = SEARCH;
int direct;

long timeThis, timeLast;

void forward() {
  Serial.print("forward");
  analogWrite(MOTORLEFT_1, 100);
  digitalWrite(MOTORLEFT_2, LOW);
  analogWrite(MOTORRIGHT_1, 255);
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
  analogWrite(MOTORLEFT_2, TURNSPEED);
  analogWrite(MOTORRIGHT_1, TURNSPEED);
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
  Serial.print("Setup Start");
  pinMode(MOTORLEFT_1, OUTPUT);
  pinMode(MOTORLEFT_2, OUTPUT);
  pinMode(MOTORRIGHT_1, OUTPUT);
  pinMode(MOTORRIGHT_2, OUTPUT);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  Serial.print("Setup Done");
  delay(5000);
}

void loop() {
  timeThis = millis();
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  Serial.println(distance);
  switch (state) {


    case SEARCH:


      if (analogRead(0) < 200) {
        timeLast = timeThis;
        state = LINE;
      }
      if (analogRead(1) < 200) {
        timeLast = timeThis;
        state = LINE;
      }
      if (distance < 700) {
        timeLast = timeThis;
        state = FOUND;
      }
      else if ((analogRead(1) > 200) && (analogRead(0) > 200) &&  (distance > 700)) {
        if (timeThis - timeLast < 1000) {
          left();
        }
        if ( timeThis - timeLast > 1000) {
          timeLast = timeThis;
        }
      }
      break;


    case LINE:
      if (timeThis - timeLast < 300) {
        reverse();
      }
      if (timeThis - timeLast > 300) {
        timeLast = timeThis;
        state = SEARCH;
      }
      break;


    case FOUND:
      if (timeThis - timeLast < 200) {
        right();
      }
      if ((timeThis - timeLast < 1200) && (timeThis - timeLast > 200)) {
        forward();
      }
      if ((timeThis - timeLast > 1200) && (distance < 700)) {
        timeLast = timeThis;
      }
      if ((timeThis - timeLast > 1200) && (distance > 700)) {
        timeLast = timeThis;
        state = SEARCH;
      }
      break;
  }
}

