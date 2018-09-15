/*
  http://primo.io

  This sketch is part of the Primo Prototype Documentation.
  http://docs.primo.io

  Tested on the Arduino UNO.
  Load this into Cubetto, the little cube robot.
*/

// Uncomment line below to enable debug serial messages
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif

#define FORWARD 1
#define BACKWARD 0

//serial protocol
#define STOP 'O'
#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'
#define BACKWARD 'B'
#define INIT 'I'
#define STATUS 'S'
#define WRONG 'W'

// Setting I/O pins
//left motor - M2
const int leftEnable = 5;
const int leftForward = 6;
const int leftReverse = 7;

//right motor - M1
const int rightEnable = 2;
const int rightForward = 3;
const int rightReverse = 4;

//encoders
const int leftEncoder = A1;
const int rightEncoder = A0;

//CNY70
const int bwVal = 150;

char instruction = 'O';

#ifdef DEBUG
int d_valueL = -1;
int d_previousValueL = -1;
int d_valueR = -1;
int d_previousValueR = -1;
int d_counterR = 0;
int d_counterL = 0;
int d_i_ts = 0;

void init_debug_counters() {
  d_i_ts = millis();
  d_valueL = -1;
  d_previousValueL = -1;
  d_valueR = -1;
  d_previousValueR = -1;
  d_counterR = 0;
  d_counterL = 0;
  d_previousValueL = readEncoder(leftEncoder);
  d_previousValueR = readEncoder(rightEncoder);
}

void count_wheel_turns() {
  d_valueR = readEncoder(rightEncoder);
  d_valueL = readEncoder(leftEncoder);
  if (d_valueL != d_previousValueL) {
    d_counterL++;
    d_previousValueL = d_valueL;
  }
  if (d_valueR != d_previousValueR) {
    d_counterR++;
    d_previousValueR = d_valueR;
  }
}

void print_count_wheel_turns() {
  int ts = millis() - d_i_ts;
  Serial.print("Elapsed time: ");
  Serial.print(ts);
  Serial.print(", counterL : ");
  Serial.print(d_counterL);
  Serial.print(", counterR : ");
  Serial.print(d_counterR);
  Serial.print(" -> ");
  print_inline_encoders();
}

#endif

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

  delay(500);

  //initialize aligns the wheels
  initialize();
  if (Serial.available() > 0) {
    Serial.println("Hi");
  }
}

void loop() {
  //read rom the xbee
  if (Serial.available() > 0) {
    instruction = Serial.read();
  }

  //decode instruction
  switch (instruction) {
    case STATUS:
    case 's':
      print_inline_encoders();
      break;
    case FORWARD: // Red block
    case 'f':
      forward(128, 16); //128,16 ~= 12cm
      break;
    case LEFT: // Blue block
    case 'l':
      turn(128, 10, LEFT); //128,9
      break;
    case RIGHT: // Yellow block
    case 'r':
      turn(128, 10, RIGHT); //138,7
      break;
    case INIT:
    case 'i':
      initialize();
      break;
    case STOP:
    case 'o':
      stop();
      break;
  }
  instruction = 'O';
}
