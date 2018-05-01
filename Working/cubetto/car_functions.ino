//one wheel is composed of 32 slices
//16 black and 16 white.

const int whiteSlice = 1;
const int blackSlice = 0;

int readEncoder(int enc) {
  int value = analogRead(enc);
  return (value < bwVal) ? blackSlice : whiteSlice;
}


//rotate wheels until the next sign
void initialize() {
  //  int vl = readEncoder(leftEncoder);

  //forward left
  analogWrite(leftForward, 88);
  digitalWrite(leftReverse, LOW);

  while (readEncoder(leftEncoder) == blackSlice);

  stopLeft();

  delay(100);

  int vr = readEncoder(rightEncoder);

  //forward right
  analogWrite(rightForward, 88);
  digitalWrite(rightReverse, LOW);

  while (readEncoder(rightEncoder) == blackSlice);

  stopRight();
}

/* MOTION FUNCTIONS */

//not used
void backward(int speed, int steps) {
  int vL, pvL = -1;
  int vR, pvR = -1;

  int counterL, counterR = 0;

  pvL = readEncoder(leftEncoder);
  pvR = readEncoder(rightEncoder);

  //right backward
  digitalWrite(rightForward, LOW);
  analogWrite(rightReverse, speed);

  //left backward
  digitalWrite(leftForward, LOW);
  analogWrite(leftReverse, speed);


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
    // debug
    Serial.print("R L: ");
    Serial.println(counterL);
    Serial.println(vL);
    Serial.print("R R: ");
    Serial.println(counterR);
    Serial.println(vR);
  }



  stop();
}

// Red block
void forward(int speed, int steps) {
  int vL, pvL = -1;
  int vR, pvR = -1;

  int counterL, counterR = 0;

  pvL = readEncoder(leftEncoder);
  pvR = readEncoder(rightEncoder);

  //right forward
  analogWrite(rightForward, speed);
  digitalWrite(rightReverse, LOW);

  //left forward
  analogWrite(leftForward, speed);
  digitalWrite(leftReverse, LOW);


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
    Serial.print("F L: ");
    Serial.println(counterL);
    Serial.println(vL);
    Serial.print("F R: ");
    Serial.println(counterR);
    Serial.println(vR);
  }

  stop();
}

// Blue block
void left(int speed, int steps) {
  int vL, pvL = -1;
  int vR, pvR = -1;

  int counterL, counterR = 0;

  pvL = readEncoder(leftEncoder);
  pvR = readEncoder(rightEncoder);

  //right forward
  analogWrite(rightForward, speed);
  digitalWrite(rightReverse, LOW);

  //left backward
  digitalWrite(leftForward, LOW);
  analogWrite(leftReverse, speed);

  //count rotation
  while (counterL <= steps) { //&& counterR <= steps) {
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
    //debug
    Serial.print("L L: ");
    Serial.println(counterL);
    Serial.println(vL);
    Serial.print("L R: ");
    Serial.println(counterR);
    Serial.println(vR);
  }

  stop();
}

// Yellow block
void right(int speed, int steps) {
  int vL, pvL = -1;
  int vR, pvR = -1;

  int counterL = 0, counterR = 0;

  pvL = readEncoder(leftEncoder);
  pvR = readEncoder(rightEncoder);

  //right reverse
  digitalWrite(rightForward, LOW);
  analogWrite(rightReverse, speed);

  //left forward
  digitalWrite(leftReverse, LOW);
  analogWrite(leftForward, speed);

  //count rotation
  while (counterR <= steps) { // && counterL <= steps) {
    if (counterR <= steps) {
      vR = readEncoder(rightEncoder);
      if (vR != pvR) {
        counterR++;
        pvR = vR;
      }
    }
    if (counterL <= steps) {
      vL = readEncoder(leftEncoder);
      if (vL != pvL) {
        counterL++;
        pvL = vL;
      }
    }
    //debug
    Serial.print("R L: ");
    Serial.println(counterL);
    Serial.println(vL);
    Serial.print("R R: ");
    Serial.println(counterR);
    Serial.println(vR);

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
  digitalWrite(leftEnable, LOW);
  digitalWrite(rightEnable, LOW);
  stopLeft();
  stopRight();
  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);
}

void hardStop() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, HIGH);

  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, HIGH);
  instruction = 'O';
}
