#include <PString.h>

//one wheel is composed of 32 slices
//16 black and 16 white.

const int whiteSlice = 1;
const int blackSlice = 0;

int readEncoder(int enc) {
  int value = analogRead(enc);
  return (value < bwVal) ? blackSlice : whiteSlice;
}


//rotate wheels until the next black sign
void initialize() {
  //forward left
  analogWrite(leftForward, 88);
  digitalWrite(leftReverse, LOW);

  while (readEncoder(leftEncoder) == blackSlice);

  stopLeft();

  //forward right
  analogWrite(rightForward, 88);
  digitalWrite(rightReverse, LOW);

  while (readEncoder(rightEncoder) == blackSlice);

  stopRight();
}

/* MOTION FUNCTIONS */

// Red block
void forward(int speed, int steps) {
  int vL, pvL = -1;
  int vR, pvR = -1;

  int counterL, counterR = 0;

  pvL = readEncoder(leftEncoder);
  pvR = readEncoder(rightEncoder);

  //left forward
  analogWrite(leftForward, speed);
  digitalWrite(leftReverse, LOW);

  //TODO Fix me
  // trying to handle: right motor is faster than left
  delay(100);
  
   //right forward
  analogWrite(rightForward, speed);
  digitalWrite(rightReverse, LOW);
  
  while (counterL <= steps || counterR <= steps) {
    if (counterL <= steps) {
      vL = readEncoder(leftEncoder);
      if (vL != pvL) {
        counterL++;
        pvL = vL;
      }
    }
    if (counterR <= steps) {
      vR = readEncoder(rightEncoder);
      if (vR != pvR) {
        counterR++;
        pvR = vR;
      }
    }
  }

  stop();
}

// Blue block
void left(int speed, int steps) {
  int vL, pvL = -1;
  int counterL, counterR = 0;

  pvL = readEncoder(leftEncoder);

  //left backward
  digitalWrite(leftForward, LOW);
  analogWrite(leftReverse, speed);

  //right forward
  analogWrite(rightForward, speed);
  digitalWrite(rightReverse, LOW);

  //count rotation
  while (counterL <= steps) {
    vL = readEncoder(leftEncoder);
    if (vL != pvL) {
      counterL++;
      pvL = vL;
    }
  }
  stop();
}

// Yellow block
void right(int speed, int steps) {
  int vR, pvR = -1;
  int counterR = 0;
  
  pvR = readEncoder(rightEncoder);

  //right reverse
  digitalWrite(rightForward, LOW);
  analogWrite(rightReverse, speed);

  //left forward
  digitalWrite(leftReverse, LOW);
  analogWrite(leftForward, speed);

  //count rotation
  while (counterR <= steps) {
    vR = readEncoder(rightEncoder);
    if (vR != pvR) {
      counterR++;
      pvR = vR;
    }
  }
  stop();
}

/* STOP FUNCTIONS */
void stopLeft() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, LOW);
}

void stopRight() {
  digitalWrite(rightForward, LOW);
  digitalWrite(rightReverse, LOW);
}

void stop() {
  stopLeft();
  stopRight();
}

void hardStop() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, HIGH);
}
