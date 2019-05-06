#include "Adafruit_VL53L0X.h" //distance sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // "
#include <Servo.h>
#include <QTRSensors.h>

Servo liftservo;
QTRSensors qtr;

const uint8_t SensorCount = 3;
uint16_t sensorValues[SensorCount];

#define LIFTSERVO   5
#define MOTORLEFT_1 6
#define MOTORLEFT_2 9
#define MOTORRIGHT_1 10
#define MOTORRIGHT_2 11

#define LINEFOUNDLEFT 0
#define LINEFOUNDRIGHT 1
#define ROBOTFOUND 2
#define SEARCH 3
#define FORWARD 4
#define REVERSE 5
#define LEFT 6
#define RIGHT 7
//analogread 5 and 4 are the distance sensor
int distance;
int state = SEARCH;
int leftsens = analogRead(0);
int rightsens = analogRead(1);
int backsens = analogRead(2);

long timeThis, timeLast;


void forward() {
  analogWrite(MOTORLEFT_1, 255);
  digitalWrite(MOTORLEFT_2, LOW);
  analogWrite(MOTORRIGHT_1, 255);
  digitalWrite(MOTORRIGHT_2, LOW);
  Serial.print("forward");
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
  delay(100);
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
  delay(100);
  digitalWrite(MOTORLEFT_1, LOW);
  digitalWrite(MOTORLEFT_2, LOW);
  digitalWrite(MOTORRIGHT_1, LOW);
  digitalWrite(MOTORRIGHT_2, LOW);
}

void setup() {
  liftservo.attach(LIFTSERVO);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]) {
    A1, A2, A3
  }, SensorCount);
  pinMode(MOTORLEFT_1, OUTPUT);
  pinMode(MOTORLEFT_2, OUTPUT);
  pinMode(MOTORRIGHT_1, OUTPUT);
  pinMode(MOTORRIGHT_2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  //delay(5000); // startup delay
  //liftservo.write(0); //setting servo to proper position
  lox.begin();
}

void loop() {
  timeThis = millis();
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  distance = measure.RangeMilliMeter;
  Serial.print(distance);
  Serial.print("\t");


  Serial.println(state);
  switch (state) {
    case LINEFOUNDLEFT:
      state = REVERSE;
      right;
      state = SEARCH;
      break;
    case LINEFOUNDRIGHT:
      reverse;
      left;
      state = SEARCH;
      break;
    case ROBOTFOUND:
      forward();
      if (distance > 1000) {
        state = SEARCH;
      }
      if (distance <= 100) {
        //liftservo.write(170);
      }
      break;
    case SEARCH:
      if (leftsens > 500) {
        state = LINEFOUNDLEFT;
      }
      if (rightsens > 500) {
        state = LINEFOUNDRIGHT;
      }
      if (distance <= 1000) {
        state = ROBOTFOUND;
      }
      else {
        right;
        forward;
        left;
        forward;
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
