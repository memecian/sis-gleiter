/* includes */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Servo.h>

/* defines */

// display
#define ADDR 	0x3c
#define H 		32
#define W		 	128
#define RST 	-1

// reserved pins
#define WINDUP 	13
#define START 	12
#define SERVO 	2

// etc
#define FLIGHT_TIME (115 * 60000) // Flight time in minutes
#define SERVO_TIME 	(20 * 1000) 	// Servo rotation time in seconds TODO: add actual rotation time

/* global vars */
bool started, released = false;
unsigned long remaining = FLIGHT_TIME;
char* remainOut = (char*) malloc(sizeof(char) * 8); // reserve 8 chars for formatted time output
unsigned long current = 0;

Adafruit_SSD1306 display(W, H, &Wire, RST);
Servo servo;

/* function declarations */

//void timeFormat(); // TODO : add implemention!

void setup(){

 	// setup here
  display.begin(SSD1306_SWITCHCAPVCC, ADDR);
  display.setRotation(1); 
	display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); 
  display.setTextSize(0);
	
	servo.attach(SERVO);
  
	pinMode(START, INPUT_PULLUP);
	pinMode(WINDUP, INPUT_PULLUP);
}

void loop(){
  // loop here
	
	/* winding sequence 
	 * just a loop to affix the glider to the balloon */
	
	servo.write(90); 								// Stop servo movement
	while (!digitalRead(WINDUP)) {
		servo.write(0);
	}
	

	/* start sequence
	 * confirm launch by having the user press
	 * the START button twice									*/

	if (!started && !digitalRead(START)) {
		clearRect(0,0, 128, 9);
		display.print("You sure?");
		current = millis() + 10000;
		// ten second timeout 
		while (millis < current) {	
			if (!digitalRead(START)) { 
				clearRect(0, 0, 128, 32);
				display.setCursor(0,0);
				display.print("Time until release");
				started = true;
				break;
			}
		}
	}

	/* main loop 
	 * the loop that will be working most of the flight */

	while (started) {
 		delay(1000);
  	if (remaining) remaining -= 1000;
		if (!remaining) {
			started = false; 
			released = true;
			}
		clearRect(0, 9, 128, 32);
  	display.setCursor(0,8);
  	display.print(String(remaining)); // TODO : Add formatted time output and replace String(remaining) with remainOut 
  	display.display();
		
  	}
  
	/* release sequence 
	 * release the glider, print a song quote. */
	
	if (released) { 
  	display.setCursor(0,0);
		clearRect(0,0,128,32);
		display.print("es gibt nichts\nwas mich hält\nau revoir");
		current = millis() + SERVO_TIME;
		while (millis < current)servo.write(180); // unwind the nuts
		servo.write(90);													// stop rotation
		while (1) asm volatile("nop"); 						// idle
  }	
}

void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  display.drawRect(x, y, w, h, BLACK);
  display.fillRect(x, y, w, h, BLACK);
}
