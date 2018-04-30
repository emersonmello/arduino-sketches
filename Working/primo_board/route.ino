void route(int i) {
  int led = i + 30;

  switch (whichBlock(vals[i])){
    case 'F':
      Serial.println('F');
      digitalWrite(led, LOW);
      delay(1500);
      break;
    case 'L':
      Serial.println('L');
      digitalWrite(led, LOW);
      delay(1500);
      break;
    case 'R':
      Serial.println('R');
      digitalWrite(led, LOW);
      delay(1500);
      break;
    case 'P': // function
      digitalWrite(led, LOW);
      //route function line
      for ( int j = 12; j < 16; j++) {
        switch (whichBlock(vals[j])){
          case 'F':
          case 'L':
          case 'R':
            route(j);
        }
      }
      //turn function leds back on
      for (int j = 12; j < 16; j++) {
        if (vals[j] < emptyEdge) { //1023 or 1010?
          digitalWrite(j + 30, HIGH);
        }
      }
      delay(1000);
      break;
  }
}
