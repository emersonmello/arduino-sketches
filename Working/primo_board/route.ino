
void blinkRunningBlock(int led, int functionBlock, char direction){
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

    switch (sequenceCommand[i]){
        case 'F':
        case 'L':
        case 'R':
            Serial.println(sequenceCommand[i]);
            blinkRunningBlock(led,functionBlock,sequenceCommand[i]);
            delay(1000); // 1200 + 1000 , because cubetto spends 1600 to move forward
            break;
        case 'P': // function      
            //route function line
            for (int j = 12; j < 16; j++) {
                switch (sequenceCommand[j]){
                    case 'F':
                    case 'L':
                    case 'R':
                        route(j,led);
                }
            }
            //turn function leds back on
            for (int j = 12; j < 16; j++) {
                if (sequenceCommand[j] != 'U') {
                    digitalWrite((j + firstLedPin), HIGH);
                }
            }
            break;
    }
}
