
void blinkRunningBlock(int led, int functionBlock){
  for(int i = 0 ; i < 3 ; i++){
    digitalWrite(led, LOW);
    if (functionBlock > 0){
      digitalWrite(functionBlock, LOW);
    }
    delay(600);
    digitalWrite(led, HIGH);
    if (functionBlock > 0){
      digitalWrite(functionBlock, HIGH);
    }
    delay(600);
  }
  digitalWrite(led, LOW);
  if (functionBlock > 0){
      digitalWrite(functionBlock, LOW);
    }
}

void route(int i, int functionBlock) {
  int led = i + 30;

  switch (whichBlock(vals[i])){
    case 'F':
      Serial.println('F');
      blinkRunningBlock(led,functionBlock);
      break;
    case 'L':
      Serial.println('L');
      blinkRunningBlock(led,functionBlock);
      break;
    case 'R':
      Serial.println('R');
      blinkRunningBlock(led,functionBlock);
      break;
    case 'P': // function
      
      //route function line
      for ( int j = 12; j < 16; j++) {
        switch (whichBlock(vals[j])){
          case 'F':
          case 'L':
          case 'R':
            route(j,led);
        }
      }
//      digitalWrite(led, LOW);
      //turn function leds back on
      for (int j = 12; j < 16; j++) {
        if (vals[j] < emptyEdge) { //1023 or 1010?
          digitalWrite((j + firstLedPin), HIGH);
        }
      }
      break;
  }
}
