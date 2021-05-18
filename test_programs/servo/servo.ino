/* includes */
#include <Servo.h>

/* defines */
#define BTN0 13
#define BTN1 12
#define SERVO 2

/* global variables */
Servo servo;

void setup() {
    pinMode(BTN0, INPUT_PULLUP);
    pinMode(BTN1, INPUT_PULLUP);
    Serial.begin(9600);
    servo.attach(SERVO); 
}

void loop() {
    while (!digitalRead(BTN0)) {
        if (!(millis() % 500)) Serial.print("Spinning (0)");  
        servo.write(0); 
    }
    while (!digitalRead(BTN1)) {
       if (!(millis() % 500)) Serial.print("Spinning (180)"); // gebe jede 500ms eine statusmeldung aus  
       servo.write(180); 
    }
    servo.write(90); // servo anhalten
    if (!(millis() % 500)) Serial.print("Stopped.");  
}
