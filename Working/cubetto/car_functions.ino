//one wheel is composed of 32 slices
//16 black and 16 white.

const int whiteSlice = 1;
const int blackSlice = 0;

// change to 1 if you are using digital input with CNY70
const int digitalEncoder = 0; 


int l = 2;
int r = 2;

int readLeft(int l) {
  int rl = analogRead(leftEncoder);
  if (rl < bwVal && l == 1) {
    l = 0;
  } else if (rl < bwVal && l == 2) {
    l = 0;
  }
  if (rl >= bwVal && l == 0){
    l = 1;
  } else if (rl >= bwVal && l == 2) {
    l = 1;
  }
  return l;
}

int readRight(int r) {
  int rr = analogRead(rightEncoder);
  if (rr < bwVal && r == 1) {
    r = 0;
  } else if (rr < bwVal && r == 2) {
    r = 0;
  }
  if (rr >= bwVal && r == 0) {
    r = 1;
  } else if (rr >= bwVal && r == 2) {
    r = 1;
  }
  return r;
}


int readEncoder(int enc){
  if (digitalEncoder == 1){
    return digitalRead(enc);
  }
  int value = analogRead(enc);
//  Serial.println(value);
  // convert analog input to digital output
  return (value < 130) ? whiteSlice : blackSlice; 
}


//rotate wheels until the next sign
void initialize() {
//  int vl = digitalRead(leftEncoder);
  int vl = readEncoder(leftEncoder);

  //forward left
  analogWrite(leftForward, 88);
  digitalWrite(leftReverse, LOW);

//  while (digitalRead(leftEncoder) == vl);
  while (readEncoder(leftEncoder) == vl);

  stopLeft();

  delay(100);

//  int vr = digitalRead(rightEncoder);
  int vr = readEncoder(rightEncoder);

  //forward right
  analogWrite(rightForward, 88);
  digitalWrite(rightReverse, LOW);

//  while (digitalRead(rightEncoder) == vr);
  while (readEncoder(rightEncoder) == vr);

  stopRight();
}

/* MOTION FUNCTIONS */

//not used
void backward(int speed, int steps) {
  int vL, pvL = 0;
  int vR, pvR = 0;

  int counterL, counterR = 0;

  //right backward
  digitalWrite(rightForward, LOW);
  analogWrite(rightReverse, speed);

  //left backward
  digitalWrite(leftForward, LOW);
  analogWrite(leftReverse, speed);


  while (counterL <= steps && counterR <= steps) {
    if (counterL <= steps) {
//      vL = digitalRead(leftEncoder);
      vL = readEncoder(leftEncoder);
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
//      vR = digitalRead(rightEncoder);
      vR = readEncoder(rightEncoder);
      if (vR != pvR) counterR++;
      pvR = vR;
    }
  }

  stop();
}

void turningTest(int speed, int steps){
  int vR, pvR = -1;
  int aR, apvR = -1;
  int counterR = 0;

  
 //left forward
  analogWrite(leftForward, speed);
  digitalWrite(leftReverse, LOW);
  
  pvR = readEncoder(leftEncoder);
  apvR = analogRead(leftEncoder);
  Serial.print("inicial encoder: ");
  Serial.println(apvR);
 while (counterR <= steps) {
      vR = readEncoder(leftEncoder);
      aR = analogRead(leftEncoder);
      Serial.print("w:aR: ");
      Serial.print(aR);
      Serial.print(", w:vR: ");
      Serial.print(vR);
      Serial.print(", w:apvR: ");
      Serial.print(apvR);
      Serial.print(", w:pvR ");
      Serial.println(pvR);
      
      if (vR != pvR) {
        counterR++;
      }
      pvR = vR;
      apvR = aR;
 }
   stop();
   Serial.println("-------------");
}

// Red block
//void forward(int speed, int steps) {
//  int vL, pvL = 0;
//  int vR, pvR = 0;
//
//  int counterL, counterR = 0;
//
//  //right forward
//  analogWrite(rightForward, speed);
//  digitalWrite(rightReverse, LOW);
//
//  //left forward
//  analogWrite(leftForward, speed);
//  digitalWrite(leftReverse, LOW);
//
//
//  while (counterL <= steps && counterR <= steps) {
//    if (counterL <= steps) {
////      vL = digitalRead(leftEncoder);
//      vL = readEncoder(leftEncoder);
//      if (vL != pvL) counterL++;
//      pvL = vL;
//    }
//    if (counterR <= steps) {
////      vR = digitalRead(rightEncoder);
//      vR = readEncoder(rightEncoder);
//      if (vR != pvR) counterR++;
//      pvR = vR;
//    }
//  }
//
//  stop();
//}
void forward(int speed, int steps) {
  int vL = 0; 
  int pvL = 0;
  int vR = 0;
  int pvR = 0;

  int counterL = 0;
  int counterR = 0;
  
  pvL = readLeft(l);
  pvR = readRight(r);

  //right forward
  analogWrite(rightReverse, LOW);
  digitalWrite(rightForward, speed);
  
  //left forward
  analogWrite(leftReverse, LOW);
  digitalWrite(leftForward, speed);
  

  while (counterL <= steps || counterR <= steps) {
    if (counterL <= steps) {
      vL = readLeft(pvL);
      if (vL != pvL) {
        counterL++;
        pvL = vL;
      }
    }
    if (counterR <= steps) {
      vR = readRight(pvR);
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

//// Blue block
//void left(int speed, int steps) {
//  int vL, pvL = 0;
//  int vR, pvR = 0;
//
//  int counterL, counterR = 0;
//
//  //right forward
//  analogWrite(rightForward, speed);
//  digitalWrite(rightReverse, LOW);
//
//  //left backward
//  digitalWrite(leftForward, LOW);
//  analogWrite(leftReverse, speed);
//
//  //count rotation
//  while (counterL <= steps && counterR <= steps) {
//    if (counterL <= steps) {
////      vL = digitalRead(leftEncoder);
//      vL = readEncoder(leftEncoder);
//      if (vL != pvL) counterL++;
//      pvL = vL;
//    }
//    if (counterR <= steps) {
////      vR = digitalRead(rightEncoder);
//      vR = readEncoder(rightEncoder);
//      if (vR != pvR) counterR++;
//      pvR = vR;
//    }
//  }
//
//  stop();
//}
//
//// Yellow block
//void right(int speed, int steps) {
//
//  int vL, pvL = 0;
//  int vR, pvR = 0;
//
//  int counterL = 0, counterR = 0;
//
//  //right reverse
//  digitalWrite(rightForward, LOW);
//  analogWrite(rightReverse, speed);
//
//  //left forward
//  digitalWrite(leftReverse, LOW);
//  analogWrite(leftForward, speed);
//
//  //count rotation
//  while (counterL <= steps && counterR <= steps) {
//    if (counterL <= steps) {
////      vL = digitalRead(leftEncoder);
//      vL = readEncoder(leftEncoder);
//      if (vL != pvL) counterL++;
//      pvL = vL;
//    }
//    if (counterR <= steps) {
////      vR = digitalRead(rightEncoder);
//      vR = readEncoder(rightEncoder);
//      if (vR != pvR) counterR++;
//      pvR = vR;
//    }
//  }
//
//  stop();
//}



void left(int speed, int steps) {
  int vL = 0; 
  int pvL = 0;
  int vR = 0;
  int pvR = 0;

  int counterL = 0;
  int counterR = 0;

  pvL = readLeft(l);
  pvR = readRight(r);

  //right forward
  analogWrite(rightReverse, LOW);
  digitalWrite(rightForward, speed);
  
  //left backward
  digitalWrite(leftForward, LOW);
  analogWrite(leftReverse, speed);

  //count rotation
  while (counterL <= steps) { // && counterR <= steps) {
    if (counterL <= steps) {
      vL = readLeft(pvL);
      if (vL != pvL) {
        counterL++;
        pvL = vL;
      }
    }
    if (counterR <= steps) {
      vR = readRight(pvR);
      if (vR != pvR) {
        counterR++;
        pvR = vR;
      }
    }

    Serial.print("L L: ");
    Serial.println(counterL);
    Serial.println(vL);
    Serial.print("L R: ");
    Serial.println(counterR);
    Serial.println(vR);
    
  }

  stop();
}

void right(int speed, int steps) {
  int vL = 0; 
  int pvL = 0;
  int vR = 0;
  int pvR = 0;

  int counterL = 0;
  int counterR = 0;

  pvL = readLeft(l);
  pvR = readRight(r);

  //right reverse
  digitalWrite(rightForward, LOW);
  analogWrite(rightReverse, speed);

  //left forward
  digitalWrite(leftReverse, LOW);
  analogWrite(leftForward, speed);

  //count rotation
  while (counterR <= steps) { // && counterL <= steps) {
    if (counterL <= steps) {
      vL = readLeft(pvL);
      if (vL != pvL) {
        counterL++;
        pvL = vL;
      }
    }
    if (counterR <= steps) {
      vR = readRight(pvR);
      if (vR != pvR) {
        counterR++;
        pvR = vR;
      }
    }

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
  stopLeft();
  stopRight();
}

void hardStop() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, HIGH);

  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, HIGH);
  instruction = 'O';
}
