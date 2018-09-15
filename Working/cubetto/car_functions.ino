#include <PString.h>

//one wheel is composed of 32 slices
//16 black and 16 white.

const int whiteSlice = 1;
const int blackSlice = 0;

int readEncoder(int enc){
    int value = analogRead(enc);
    return (value < bwVal) ? blackSlice : whiteSlice;
}

//rotate wheels until the next black sign
void initialize(){
    //forward left
    analogWrite(leftForward, 88);
    digitalWrite(leftReverse, LOW);

#ifdef DEBUG
    print_inline_encoders();
#endif
    while (readEncoder(leftEncoder) == blackSlice);

    stopLeft();

    //forward right
    analogWrite(rightForward, 88);
    digitalWrite(rightReverse, LOW);

    while (readEncoder(rightEncoder) == blackSlice);

    stopRight();
#ifdef DEBUG
    print_inline_encoders();
#endif
}

/* MOTION FUNCTIONS */

// Red block
void forward(int speed, int steps){
    int counter = 0;
    int value = -1;
    int previousValue = -1;

    int vL = -1;
    int pvL = -1;
    int counterL = 0;

    int vR = -1;
    int pvR = -1;
    int counterR = 0;

    pvL = readEncoder(leftEncoder);
    pvR = readEncoder(rightEncoder);

#ifdef DEBUG
    init_debug_counters();
#endif

    //left forward
    analogWrite(leftForward, speed);
    digitalWrite(leftReverse, LOW);

    //right forward
    //ToDo My right motor is faster than left one. Why? I don't know.
    // Workaround: set it a little bit slow
    analogWrite(rightForward, (speed - 30));
    digitalWrite(rightReverse, LOW);

    while (counterL <= steps || counterR <= steps){
        if (counterL <= steps){
            vL = readEncoder(leftEncoder);
            if (vL != pvL){
                counterL++;
                pvL = vL;
            }
        }
        if (counterR <= steps){
            vR = readEncoder(rightEncoder);
            if (vR != pvR){
                counterR++;
                pvR = vR;
            }
        }
#ifdef DEBUG
        count_wheel_turns();
#endif
    }

    stop();
#ifdef DEBUG
    print_count_wheel_turns();
#endif
}

// direction = (L)eft or (R)ight
// Left = Blue block
// Right = Yellow block
void turn(int speed, int steps, char direction){
    int counter = 0;
    int value = -1;
    int previousValue = -1;
    int encoder = -1;

#ifdef DEBUG
    init_debug_counters();
#endif

    if (direction == LEFT){
        encoder = leftEncoder;
        DEBUG_PRINT("Turn LEFT");

        //left backward
        digitalWrite(leftForward, LOW);
        analogWrite(leftReverse, speed);

        //right forward
        analogWrite(rightForward, speed);
        digitalWrite(rightReverse, LOW);
    }
    else{
        encoder = rightEncoder;
        DEBUG_PRINT("Turn RIGHT");

        //right reverse
        digitalWrite(rightForward, LOW);
        analogWrite(rightReverse, speed);

        //left forward
        digitalWrite(leftReverse, LOW);
        analogWrite(leftForward, speed);
    }
    //count rotation
    while (counter <= steps){
        value = readEncoder(encoder);
        if (value != previousValue){
            counter++;
            previousValue = value;
        }
#ifdef DEBUG
        count_wheel_turns();
#endif
    }
    stop();
#ifdef DEBUG
    print_count_wheel_turns();
#endif
}

/* STOP FUNCTIONS */
void stopLeft(){
    digitalWrite(leftForward, LOW);
    digitalWrite(leftReverse, LOW);
}

void stopRight(){
    digitalWrite(rightForward, LOW);
    digitalWrite(rightReverse, LOW);
}

void stop(){
    stopLeft();
    stopRight();
}

void hardStop(){
    digitalWrite(leftForward, HIGH);
    digitalWrite(leftReverse, HIGH);
    digitalWrite(rightForward, HIGH);
    digitalWrite(rightReverse, HIGH);
}