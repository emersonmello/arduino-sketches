import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
char instruction;

void setup() 
{
  size(200, 200);
  
  println(Serial.list());

  String portName = Serial.list()[3];
  for (int i = 0; i < Serial.list().length; i++ ) {
    println("["+i+"] "+Serial.list()[i]);
  }
  
  myPort = new Serial(this, portName, 9600);
}

void draw() {
  background(255);    
}

void keyPressed() { 
  instruction = key;
  myPort.write(instruction);  
}




