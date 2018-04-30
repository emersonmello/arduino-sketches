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

#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


//4.7kΩ RED Block
const int forwardVal = 326;
//220Ω Yellow Block
const int rightVal = 22;
//10kΩ Green Block
const int functionVal = 511;
//100kΩ Blue Block
const int leftVal = 931;

const int emptyEdge = 1010;

//range
const int gap = 50;

const int buttonPin = 50;

int butVal, pButVal;

boolean isExecuting = false;
boolean wrongInstruction = false;
boolean thereIsFunctionBlock = false;
boolean thereIsNoBlock = true;

int limit = 12;

int vals[16];

// piezo 
const int audioPin =8;
const int NOTE_SUSTAIN = 100;


void setup() {
  //led init
  for (int i = 30; i < 47; i++) {
    pinMode(i, OUTPUT);
  }

  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  
  //startup sound
  playMelody();
}

// took from Arduino Samples
void playMelody(){
   for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(13, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(13);
  }
}

void sayNoNoNo(){
   tone(13,NOTE_G4);
   delay(250);
   tone(13,NOTE_C4);
   delay(500);
   noTone(13);
    delay(100);
}

/* blink all leds to say: wrong sequence, man!*/
void wrongFeedback() {
  Serial.write('W'); // sending Wrong message to cubetto
  sayNoNoNo();
  int led = 30;
  for (int i = 0; i < 16; i++) {
    digitalWrite(i + led, LOW);
  }
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(i + led, HIGH);
      digitalWrite(7 - i + led, HIGH);
      digitalWrite(i + 8 + led, HIGH);
      digitalWrite(i + 12 + led, HIGH);
      delay(100);
    }
    for (int i = 3; i >= 0 ; i--) {
      digitalWrite(i + led, LOW);
      digitalWrite(7 - i + led, LOW);
      digitalWrite(i + 8 + led, LOW);
      digitalWrite(i + 12 + led, LOW);
      delay(100);
    }
  }
}

void turnOnLED(int i, int led) {
  if (vals[i] < emptyEdge) {
    digitalWrite(led, HIGH);
    thereIsNoBlock = false;
  }
  else {
    digitalWrite(led, LOW);
  }
}

boolean isAFunctionBlock(int pos) {
  return (whichBlock(vals[pos]) == 'P') ? true : false;
}

void readAnalogicPins() {
  limit = 12;
  for (int i = 0; i < 16; i++) {
    vals[i] = analogRead(i);
    if ((i < 12) && (isAFunctionBlock(i)) ) {
      limit = 16;
    }
  }
}

char whichBlock(int value) {
  //forward - Red
  if (value > (forwardVal - gap) && value < (forwardVal + gap)) {
    return 'F';
  }
  //left - Blue
  if (value > (leftVal - gap) && value < (leftVal + gap)) {
    return 'L';
  }
  //right - Yellow
  if (value > (rightVal - gap) && value < (rightVal + gap)) {
    return 'R';
  }
  //function - Green
  if (value > (functionVal - gap) && value < (functionVal + gap)) {
    return 'P'; // function or procedure
  }
  return 'U'; // unknown
}


void loop() {

  thereIsNoBlock = true;
  thereIsFunctionBlock = false;
  wrongInstruction = false;

  readAnalogicPins();

  //turn on LEDs if a block is inserted
  for (int i = 0; i < 16; i++) {
    int led = i + 30;
    turnOnLED(i, led);
    if (i < 12) { // regular line
      if (isAFunctionBlock(i)) {
        thereIsFunctionBlock = true;
      }
    }
  }

  delay(100);

  // blink leds out of sequence
  for (int i = 0; i < 16; i++) {
    int led = i + 30;
    // blink the current led if there is no previous block inserted
    if (i > 0) { // regular line
      if ((vals[i] < emptyEdge ) && (vals[i - 1] >= emptyEdge)) {
        if ((i != 12) || ((i==12) && (!thereIsFunctionBlock))){
          digitalWrite(led, LOW);
          wrongInstruction = true;
        }
      }
    }
    
    // function area (green line)
    if (i > 11){
      if (isAFunctionBlock(i)){ 
        digitalWrite(led, LOW);
        wrongInstruction = true;
      }else if ((!thereIsFunctionBlock) && (whichBlock(vals[i]) != 'U')){
        digitalWrite(led, LOW);
        wrongInstruction = true;
      }
    }

  }

  butVal = digitalRead(buttonPin);

  //if button press & not executing
  if (butVal == HIGH && pButVal == LOW && isExecuting == false) {
    isExecuting = true;
  }

  if (isExecuting) {

    if ((wrongInstruction) || (thereIsNoBlock)) {
      wrongFeedback();
    } else {
      readPins();
      for ( int i = 0; i < 12; i++) {
        if (vals[i] < emptyEdge) {
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
