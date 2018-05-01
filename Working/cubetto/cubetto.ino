/*
http://primo.io

This sketch is part of the Primo Prototype Documentation.
http://docs.primo.io

Tested on the Arduino UNO.
Load this into Cubetto, the little cube robot.
*/

#define FORWARD 1
#define BACKWARD 0

//serial protocol
#define STOP 'O'
#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'
#define BACKWARD 'B'
#define INIT 'I'
#define WRONG 'W'

//left motor - M2
const int leftEnable = 5;
const int leftForward = 6;
const int leftReverse = 7;

//right motor - M1
const int rightEnable = 2;
const int rightForward = 3;
const int rightReverse = 4;

//encoders
const int leftEncoder = A0;
const int rightEncoder = A1;

//CNY70
const int bwVal = 150;

char instruction = 'O';

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  

  Serial.begin(9600);

  //initialize left
  pinMode(leftEnable, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  pinMode(leftForward, OUTPUT);

  //initialize right
  pinMode(rightEnable, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);

  //enable motors
  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);

  delay(1000);
  //initialize aligns the wheels
  initialize();
 
}

void loop() {

  //read rom the xbee
  if (Serial.available() > 0) {
    instruction = Serial.read();
  }

  //decode instruction
  switch (instruction) {    

    case 'T':
    case 't':
//      initialize();
//      turningTest(50,2);
      break;
    case BACKWARD:
    case 'b':
      backward(128,16);   

    case FORWARD:
    case 'f':
      forward(128, 16);
      break;

    case LEFT:
    case 'l':
       left(150, 5);
      break;

    case RIGHT:
    case 'r':
      right(130, 7);
      break;

    case INIT:
    case 'i':
      initialize();
      break;
  
    case STOP:
    case 's':
      stop();
      break;

    default:
      stop();
      break;
  }
  instruction = 'O';
}






