/*
http://primo.io

This sketch is part of the Primo Prototype Documentation.
http://docs.primo.io

Tested on the Arduino UNO.
Load this into Cubetto, the little cube robot.
 */

// Uncomment line below to enable debug serial messages
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINT(x)
#endif

#define FORWARD 1
#define BACKWARD 0

//serial protocol
#define STOP 'O'
#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'
#define BACKWARD 'B'
#define INIT 'I'
#define STATUS 'S'
#define WRONG 'W'

// Setting I/O pins
//left motor - M1
const int leftEnable = 4;
const int leftForward = 5;
const int leftReverse = 6;

//right motor - M2
const int rightEnable = 9;
const int rightForward = 10;
const int rightReverse = 11;

//encoders CNY70
const int leftEncoder = 3; // moving to pin 3 that generates interruption
const int rightEncoder = 2;

//one wheel is composed of 32 slices
//16 black and 16 white.
const int WHITE_SLICE = 1;
const int BLACK_SLICE = 0;

class Motor{
    private:
        int motor_enable;
        int motor_forward;
        int motor_reverse;
        int encoder;  

    public:
        Motor(int enable, int forward, int reverse, int enc){
            this->motor_enable  = enable;
            this->motor_forward = forward;
            this->motor_reverse = reverse;
            this->encoder       = enc;

            pinMode(this->motor_enable, OUTPUT);
            pinMode(this->motor_forward, OUTPUT);
            pinMode(this->motor_reverse, OUTPUT);
            pinMode(this->encoder, INPUT_PULLUP); 

            // enable motor
            digitalWrite(this->motor_enable, HIGH);
        }

        void move(char direction, int speed){
            switch (direction){
                case FORWARD:
                    analogWrite(this->motor_forward, speed);
                    digitalWrite(this->motor_reverse, LOW);
                    break;
                case BACKWARD:
                    digitalWrite(this->motor_forward, LOW);
                    analogWrite(this->motor_reverse, speed);
                    break;
            }
        }

        void init(){
            analogWrite(this->motor_forward, 88);
            digitalWrite(this->motor_reverse, LOW);
            while(this->readEncoder() == BLACK_SLICE);
            this->stop();
        }

        void stop(){
            digitalWrite(this->motor_forward, LOW);
            digitalWrite(this->motor_reverse, LOW);
        }

        int readEncoder(){
            return digitalRead(this->encoder);
        }

        int getEncoder(){
            return this->encoder;
        }
};

class Cubetto{
    private:
        Motor *left;
        Motor *right;

    public:
        Cubetto(Motor *m1, Motor *m2){
            this->left  = m1;
            this->right = m2;
        }

        void init_motors(){
            this->left->init();
            this->right->init();    
            delay(500);
        }

        void stop_motors(){
            this->right->stop();      
            this->left->stop();
            delay(500);
        }

        void forward_by_time(int speed, unsigned long miliseconds, int speed_r){
            unsigned long start = millis();
            unsigned long current;

            this->left->move(FORWARD, speed);
            this->right->move(FORWARD, (speed - speed_r));// (speed - 30)?

            do{
                current = millis();
            }while((start+miliseconds) >= current);
            this->stop_motors();
        }

        void forward(int speed, int steps){
            int counter = 0;
            int vL = -1;
            int pvL = this->left->readEncoder();
            int vR = -1;
            int pvR = this->right->readEncoder();

            this->left->move(FORWARD, speed);
            this->right->move(FORWARD, speed);// (speed - 30)?

            while (counter <= steps){
                vL = this->left->readEncoder();
                if (vL != pvL) {
                    counter++;
                    pvL = vL;
                }
            }
            this->stop_motors();
        }

        void turn_by_time(int speed, unsigned long miliseconds, char direction){
            unsigned long start = millis();
            unsigned long current;
            switch(direction){
                case LEFT:
                    this->left->move(BACKWARD, speed);
                    this->right->move(FORWARD, speed);
                    break;
                case RIGHT:
                    this->right->move(BACKWARD, speed);
                    this->left->move(FORWARD, speed);
                    break;
            }

            do{
                current = millis();
            }while((start+miliseconds) >= current);
            this->stop_motors();
        }

        // direction = (L)eft or (R)ight
        // Left = Blue block
        // Right = Yellow block
        void turn(int speed, int steps, char direction) {
            int counter = 0;
            int value = -1;
            int previousValue = -1;
            Motor *motor;

            switch(direction){
                case LEFT:
                    motor = this->left;
                    previousValue = motor->readEncoder();
                    this->left->move(BACKWARD, speed);
                    this->right->move(FORWARD, speed);
                    break;
                case RIGHT:
                    motor = this->right;
                    previousValue = motor->readEncoder();
                    this->right->move(BACKWARD, speed);
                    this->left->move(FORWARD, speed);
                    break;
            }

            //count rotation
            while (counter <= steps) {
                value = motor->readEncoder();
                if (value != previousValue) {
                    counter++;
                    previousValue = value;
                }
            }
            this->stop_motors();
        }
};


Motor l_m1 = Motor(leftEnable, leftForward, leftReverse, leftEncoder);
Motor r_m2 = Motor(rightEnable, rightForward, rightReverse, rightEncoder);

Cubetto cubo = Cubetto(&l_m1, &r_m2);


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(9600);
    delay(500);
    //  cubo.init_motors();
    //  delay(500);
}

void loop() {
    //read rom the xbee
    if (Serial.available() > 0) {
        String command  = Serial.readStringUntil(','); // INSTRUCTION,MILLISECONDS,SPEED FOR RIGHT MOTOR
        Serial.read(); // to consume comma 
        String time_s = Serial.readStringUntil(',');
        Serial.read(); // to consume comma 
        String speed_s  = Serial.readStringUntil('\0');

        char instruction = command.charAt(0);
        int  mili = time_s.toInt();
        int  speed_r = speed_s.toInt();

        if (mili == 0){
            mili = 1000;
        }

        if (speed_r == 0){
            speed_r = 20;
        }

        //decode instruction
        switch (instruction) {
            case FORWARD: // Red block
            case 'f':
                //      cubo.forward(128, 16); //128,16 ~= 12cm
                cubo.forward_by_time(128, (mili+1600), speed_r);
                break;
            case LEFT: // Blue block
            case 'l':
                //      cubo.turn(128, 9, LEFT); //128,9
                cubo.turn_by_time(128, mili, LEFT);
                break;
            case RIGHT: // Yellow block
            case 'r':
                //      cubo.turn(128, 10, RIGHT); //138,7
                cubo.turn_by_time(128, mili, RIGHT);
                break;
            case INIT:
            case 'i':
                cubo.init_motors();
                break;
            case STOP:
            case 'o':
                cubo.stop_motors();
                break;
        }
        instruction = 'x';
    }
}
