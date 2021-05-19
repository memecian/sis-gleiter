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

/* global vars */
bool started, released = false;
unsigned long remaining = FLIGHT_TIME;
char* remainOut = (char*) malloc(sizeof(char) * 21); // reserve 8 chars for formatted time output. 21 bytes reserved, so the compiler shuts up
unsigned long current = 0;

Adafruit_SSD1306 display(W, H, &Wire, RST);
Servo servo;

void setup() {
    // setup here
	display.begin(SSD1306_SWITCHCAPVCC, ADDR);
  	display.setRotation(2); 
	display.clearDisplay();
  	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0,0); 
  	display.setTextSize(0);

    servo.attach(SERVO);
	pinMode(START, INPUT_PULLUP);	// ATTENTION!
	pinMode(WINDUP, INPUT_PULLUP);	// TO ACTIVATE BUTTONS, PINS NEED TO BE LOW! 
    display.print(F("Ready to launch."));
    display.display();
}

void loop(){

	}



/* function declarations */

void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	display.drawRect(x, y, w, h, BLACK);
	display.fillRect(x, y, w, h, BLACK);
}


/* formatTime(time in ms)
 * returns remaining time in hh:mm:ss format */

char* formatTime(long time) {
	char* out = (char *) malloc(sizeof(char) * 21); // stops any complaints from the compiler.
	long hh, mm, ss;
	time = time/1000; // convert to seconds from milliseconds
	mm = time / 60;
	ss = time % 60;
	hh = mm / 60;	
	mm = mm % 60;
	sprintf(out, "%02.2hd:%02.2hd:%02.2hd", hh, mm ,ss);	
	return out;
}