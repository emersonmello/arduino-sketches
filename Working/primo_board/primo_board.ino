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

//#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINT(x)
#endif

//4.7kΩ RED Block
const int forwardVal = 330;
//220Ω Yellow Block
const int rightVal = 60;
//10kΩ Green Block
const int functionVal = 530;
//100kΩ Blue Block
const int leftVal = 940;

const int emptyEdge = 1010;

//range
const int gap = 70;

const int buttonPin = 50;

int butVal, pButVal;

boolean isExecuting = false;
boolean wrongInstruction = false;
boolean thereIsFunctionBlock = false;
boolean thereIsNoBlock = true;

int vals[16];
char sequenceCommand[16];

// piezo 
const int audioPin = 8;
const int NOTE_SUSTAIN = 100;

const int firstLedPin = 30;
const int lastLedPin = 45;


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    //led init
    for (int i = firstLedPin; i <= lastLedPin; i++) {
        pinMode(i, OUTPUT);
    }

    pinMode(buttonPin, INPUT);
    pinMode(audioPin, OUTPUT);

    Serial.begin(9600);

    //startup sound
    // playMelody();
}


/* blink all leds to say: wrong sequence, man!*/
void wrongFeedback() {
    // sayNoNoNo();
    int led = firstLedPin;
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
    return (sequenceCommand[pos] == 'P') ? true : false;
}

void readAnalogicPins() {
    for (int i = 0; i < 16; i++) {
        vals[i] = analogRead(i);
        sequenceCommand[i] = whichBlock(vals[i]);
    }
}

// for debug purpose
void print_analogic_pins_values(){
    for (int i = 0; i < 16; i++) {
        DEBUG_PRINT("seq, vals: ");
        DEBUG_PRINT(sequenceCommand[i]);
        DEBUG_PRINT(" , ");
        DEBUG_PRINT(vals[i]);
        DEBUG_PRINT("\n");
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
        int led = i + firstLedPin;
        turnOnLED(i, led);
        if (i < 12) { // regular line
            if (isAFunctionBlock(i)) {
                thereIsFunctionBlock = true;
            }
        }
    }

    // To see leds out of sequence blinking
    delay(100);

    // blink leds out of sequence
    for (int i = 1; i < 16; i++) {
        int led = i + firstLedPin;
        // blink the current led if there is no previous block inserted  
        if ((sequenceCommand[i] != 'U') && (sequenceCommand[i-1] == 'U') && (i != 12)){
            digitalWrite(led, LOW);
            wrongInstruction = true;
        }
    }

    // function block at function line is not allowed
    for (int i = 12; i < 16; i++){
        int led = i + firstLedPin;
        if ((isAFunctionBlock(i)) || ((sequenceCommand[i] != 'U') && (!thereIsFunctionBlock))){ 
            digitalWrite(led, LOW);
            wrongInstruction = true;
        }
    }

    butVal = digitalRead(buttonPin);

    //if button press & not executing
    if (butVal == HIGH && pButVal == LOW && isExecuting == false) {
        isExecuting = true;
    }

    if (isExecuting) {

#ifdef DEBUG
        print_analogic_pins_values();
#endif

        if ((wrongInstruction) || (thereIsNoBlock)) {
            wrongFeedback();
            DEBUG_PRINT("errado\n");
        } else {
            for (int i = 0; i < 12; i++) {
                if (sequenceCommand[i] != 'U'){
                    route(i,-1);
                }else {
                    break;
                }
            }
        }
    }

    isExecuting = false;
    pButVal = butVal;

    delay(50);
}






// took from Arduino Samples
void playMelody(){
    for (int thisNote = 0; thisNote < 8; thisNote++) {

        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(audioPin, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(audioPin);
    }
}

void sayNoNoNo(){
    tone(audioPin,NOTE_G4);
    delay(250);
    tone(audioPin,NOTE_C4);
    delay(500);
    noTone(audioPin);
    delay(100);
}
