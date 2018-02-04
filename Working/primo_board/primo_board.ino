/*
http://primo.io

This sketch is part of the Primo Prototype Documentation.
http://docs.primo.io

Tested on the Arduino Mega 2560.
Load this into the Interface Board.

ANALOG VALUES FOR DIFFERENT RESISTORS:
10KΩ = 500
100KΩ = 920
4.7KΩ = 319
220Ω = 20
*/

//4.7kΩ RED Block
int forwardVal = 326;
//220Ω Yellow Block
int rightVal = 22;
//10kΩ Green Block
int functionVal = 511;
//100kΩ Blue Block
int leftVal = 931;

//range
int gap = 50;

int buttonPin = 50;
int butVal, pButVal;

boolean isExecuting = false;
boolean wrongInstruction = false;

int limit = 12;

int vals[16];

void setup() {
  //led init
  for (int i = 30; i < 47; i++) {
    pinMode(i, OUTPUT);
  }

  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);

}

/* blink all leds to say: wrong sequence, man!*/
void wrongFeedback(){
  Serial.write('W'); // sending Wrong message to cubetto
  int led = 30;
  for (int i=0; i < 16; i++) {
    digitalWrite(i+led, LOW);
  }
  for(int j=0; j< 3; j++){
    for (int i=0; i < 4; i++) {
      digitalWrite(i+led, HIGH);
      digitalWrite(7-i+led, HIGH);
      digitalWrite(i+8+led, HIGH);
      digitalWrite(i+12+led, HIGH);
      delay(100);
    }
    for (int i=3; i >=0 ; i--) {
      digitalWrite(i+led, LOW);
      digitalWrite(7-i+led, LOW);
      digitalWrite(i+8+led, LOW);
      digitalWrite(i+12+led, LOW);
      delay(100);
    }
  }
}

void turnOnLED(int i, int led){
  if (vals[i] < 1010) {
      digitalWrite(led, HIGH);
    }
    else {
      digitalWrite(led, LOW);
    }
}

boolean isAFunctionBlock(int pos){
  if ((vals[pos] > (functionVal - gap)) && (vals[pos] < (functionVal + gap))){
    return true;
  }
  return false;
}

void readAnalogicPins() {
    limit = 12;
  for(int i = 0; i < 16; i++) {
    vals[i] = analogRead(i);
    if ((i<12) && (isAFunctionBlock(i)) ){
        limit = 16;
    }
  }
}

void loop() {

  wrongInstruction = false;

  readAnalogicPins();

  //turn on LEDs if a block is inserted
  for(int i = 0; i < 16; i++) {
    int led = i+30;
    turnOnLED(i, led);
    if ((i>=12) && (limit == 12)){
        digitalWrite(led, LOW);
    }
  }
  delay(100);

  boolean previousFunctionBlock = false;

  // blink leds out of sequence
  for(int i = 0; i < limit; i++) {
    int led = i+30;
    if ((i > 0) && (i < 12)){ // regular line
      if ((vals[i] < 1010 ) && (vals[i-1] >= 1010)){
        digitalWrite(led, LOW);
        wrongInstruction = true;
      }
    }else if (i>12){ // function area (green line)

          // a previous function block will not allow execution of next blocks
          if (isAFunctionBlock(i-1)){
              previousFunctionBlock = true;
          }
          // blink leds of current block if previous block is a function block or block is out of sequence
          if (((vals[i] < 1010 ) && (vals[i-1] >= 1010)) || (previousFunctionBlock)){
            digitalWrite(led, LOW);
            wrongInstruction = true;
          }
    }
  }

  // first position on function line should not be a function block because it will result in a infinite loop
  if (isAFunctionBlock(12)){
    digitalWrite(12+30, LOW);
    wrongInstruction = true;
  }

  butVal = digitalRead(buttonPin);

  //if button press & not executing
  if(butVal == HIGH && pButVal == LOW && isExecuting == false) {
    isExecuting = true;
  }

  if(isExecuting) {

    if (wrongInstruction){
      // TODO: shake cubetto to say: no no no no
      wrongFeedback();
    }else{
      readPins();
      for ( int i = 0; i < 12; i++) {
        if (vals[i] < 1010) {
          route(i);
        }
        else {
          break;
        }
      }
    }
  }

  isExecuting = false;
  pButVal = butVal;

  delay(30);
}
