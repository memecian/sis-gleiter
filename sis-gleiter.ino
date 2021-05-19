/* includes */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Servo.h>

/* defines */

// display
#define ADDR 		0x3c
#define H 			32
#define W		 	128
#define RST 		-1

// reserved pins
#define WINDUP 	11
#define START 	12
#define SERVO 	2

// etc
#define FLIGHT_TIME (115 * 60000UL) 	// Flight time in minutes
#define SERVO_TIME 	(20 * 1000)	 		// Servo rotation time in seconds TODO: add actual rotation time
#define DEBUG

/* global vars */
bool started, dialogue, released = false;
unsigned long remaining = FLIGHT_TIME;
unsigned long current = 0;

/* global constants */
const char ready[] = "Ready to launch";

Adafruit_SSD1306 display(W, H, &Wire, RST);
Servo servo;

void setup() {
    // setup here
#ifdef DEBUG
	Serial.begin(9600);
#endif // DEBUG

	display.begin(SSD1306_SWITCHCAPVCC, ADDR);
  	display.setRotation(2); 
	display.clearDisplay();
  	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0,0); 
  	display.setTextSize(0);

    servo.attach(SERVO);

	pinMode(START, INPUT_PULLUP);	// ATTENTION!
	pinMode(WINDUP, INPUT_PULLUP);	// TO ACTIVATE BUTTONS, PINS NEED TO BE PULLED LOW!  

	display.print(ready);
    display.display();
}

void loop(){
	
	if (!digitalRead(START)) {
#ifdef DEBUG
	Serial.println("START pressed");
#endif // DEBUG
		dialogue = true;
		clearRect(0, 0, 128, 9);
		display.setCursor(0, 0);
		display.print("Are you sure?");
		display.display();
		delay(500);
		while (dialogue) {
			if(!digitalRead(START)) {

#ifdef DEBUG
				Serial.println("START PRESSED AGAIN");
#endif // DEBUG
				display.setTextSize(2);
				current = millis();	
				while (remaining > 0) {
					clearRect(0,0,128,32);
					display.setCursor(0,0);
					remaining = FLIGHT_TIME - (millis() - current);
#ifdef DEBUG
					Serial.println(formatTime(remaining));
#endif // DEBUG			
					display.print(formatTime(remaining));
					display.display();
				}
			}

			if (!digitalRead(WINDUP)) {
#ifdef DEBUG
				Serial.println("left dialogue");
#endif // DEBUG			
				dialogue = false;
				clearRect(0, 0, 128, 9);
				display.setCursor(0,0);
				display.print(ready);
				display.display();
			}
		} 
	}
} // loop

/* function declarations */

void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	display.drawRect(x, y, w, h, BLACK);
	display.fillRect(x, y, w, h, BLACK);
}

/* formatTime(time in ms)
 * returns remaining time in hh:mm:ss format */

char* formatTime(long time) {
	char out[10] ;
	long hh, mm, ss;
	time = floor(time/1000); // convert to seconds from milliseconds
	mm = time / 60;
	ss = time % 60;
	hh = mm / 60;	
	mm = mm % 60;
	sprintf(out, "%d:%d:%d", hh, mm ,ss);
	return out;
}