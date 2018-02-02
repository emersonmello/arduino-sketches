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

void loop() {
  
  //turn on LEDs if a block is inserted
  for(int i = 0; i < 16; i++) {
    int led = i+30;

    //fix this
    if (analogRead(i) < 1010) {
      digitalWrite(led, HIGH);
    } 
    else {
      digitalWrite(led, LOW);
    }    
  }

  butVal = digitalRead(buttonPin);
  
  //if button press & not executing
  if(butVal == HIGH && pButVal == LOW && isExecuting == false) {
    isExecuting = true;
  }

  if(isExecuting) {
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
  
  isExecuting = false;
  pButVal = butVal;

  delay(30);
}
