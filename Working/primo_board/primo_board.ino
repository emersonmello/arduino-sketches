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

void turnOnLED(int i, int led){
  if (analogRead(i) < 1010) {
      digitalWrite(led, HIGH);
    } 
    else {
      digitalWrite(led, LOW);
    }
}

boolean isAFunctionBlock(int pos){
  int posValue = analogRead(pos); 
  if ((posValue > (functionVal - gap)) && (posValue < (functionVal + gap))){
    return true;
  }
  return false;
}

void loop() {
  wrongInstruction = false;
  //turn on LEDs if a block is inserted
  for(int i = 0; i < 16; i++) {
    int led = i+30;
    turnOnLED(i, led); 
    
    
  }
  delay(100);

  boolean previousFunctionBlock = false;
  
  // blink leds out of sequence
  for(int i = 0; i < 16; i++) {
    int led = i+30;
    if ((i > 0) && (i < 12)){ // regular line
      int lastLED = digitalRead(led-1);
      if ((digitalRead(led) == HIGH) && (lastLED == LOW)){ 
        digitalWrite(led, LOW);
        
      }
    }else if (i>12){ // function area (green line)

          int previousBlock = analogRead(i-1);       
          int lastLED = digitalRead(led-1);

          // a previous function block will not allow execution of next blocks
          if (isAFunctionBlock(i-1)){
              previousFunctionBlock = true;
          }
          // blink leds of current block if previous block is a function block or block is out of sequence
          if (((digitalRead(led) == HIGH) && (lastLED == LOW)) || (previousFunctionBlock)){
            digitalWrite(led, LOW);
            wrongInstruction = true;
          }
    }    
  }

  // first position on function line should not be a function block because it will result in a infinite loop
  if (isAFunctionBlock(12)){
  int firstPosFunctionLine = analogRead(12); 
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
