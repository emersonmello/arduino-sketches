void route(int i) {

  int led = i + 30;

  //forward
  if (vals[i] > (forwardVal - gap) && vals[i] < (forwardVal + gap)) {
   // Serial1.print('F');
    Serial.println('F');
    digitalWrite(led, LOW);
    delay(1500);
  }

  //left
  if (vals[i] > (leftVal - gap) && vals[i] < (leftVal + gap)) {
 //   Serial1.print('L');
    Serial.println('L');
    digitalWrite(led, LOW);
    delay(1000);
  }


  //right
  if (vals[i] > (rightVal - gap) && vals[i] < (rightVal + gap)) {
  //  Serial1.print('R');
    Serial.println('R');
    digitalWrite(led, LOW);
    delay(1000);
  }

  //function
  if (vals[i] > (functionVal - gap) && vals[i] < (functionVal + gap)) {
    digitalWrite(led, LOW);
    //route function line
    for ( int j = 12; j < 16; j++) {
      if (vals[j] < 1023) {
          route(j);
      }else {
        break;
      }
    }
    
    //turn function leds back on
    for (int j = 12; j < 16; j++) {
      if (vals[j] < 1023) {
        digitalWrite(j + 30, HIGH);
      }
    }
    delay(500);
  }
}







