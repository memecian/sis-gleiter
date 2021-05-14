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
#define WINDUP 	13
#define START 	12
#define SERVO 	2

// etc
#define FLIGHT_TIME (115 * 60000UL) 	// Flight time in minutes
#define SERVO_TIME 	(20 * 1000)	 		// Servo rotation time in seconds TODO: add actual rotation time

#define DEBUG 1 // Comment this line to disable debug features

/* global vars */
bool started, released = false;
unsigned long remaining = FLIGHT_TIME;
char* remainOut = (char*) malloc(sizeof(char) * 21); // reserve 8 chars for formatted time output. 21 bytes reserved, so the compiler shuts up
unsigned long current = 0;

Adafruit_SSD1306 display(W, H, &Wire, RST);
Servo servo;

#ifdef DEBUG
	char debugOut[128] = {0};
	long slu = milllis() + 1000; //SinceLastUpdate
#endif

void setup(){

#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Starting display setup");
#endif 	
    // setup here
	display.begin(SSD1306_SWITCHCAPVCC, ADDR);
  	display.setRotation(2); 
	display.clearDisplay();
  	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0,0); 
  	display.setTextSize(0);

#ifdef DEBUG
    Serial.println("Display setup complete.");
#endif

    servo.attach(SERVO);
  
	pinMode(START, INPUT_PULLUP);	// ATTENTION!
	pinMode(WINDUP, INPUT_PULLUP);	// TO ACTIVATE BUTTONS, PINS NEED TO BE LOW! 

    display.print("Ready to launch.");
    display.display();
#ifdef DEBUG
    Serial.println("Entering loop");
#endif
}

void loop(){
  	// loop here
#ifdef DEBUG
	Serial.println("Loop!");
	if (slu <= millis()) {
		remainOut = formatTime(remaining);
		sprintf(debugOut, "started = %u\n released = %u\n remaining = %lu\n, remainOut = %s\n, current = %lu", started, released, remaining, remainOut, current);
		Serial.println(debugOut);
		slu = millis() + 1000;
}
#endif
    /* winding sequence 
	 * just a loop to affix the glider to the balloon */
	if (!digitalRead(SERVO)){
    	servo.write(90);// if the button isn't pressed, stop the servo. 
    }
	else { 
    	servo.write(0); 		
#ifdef DEBUG
    Serial.println("---Servo winding---");
#endif 
    }

	/* start sequence
	 * confirm launch by having the user press
	 * the START button twice				*/

	if (!started && !digitalRead(START)) {
		clearRect(0,0, 128, 9);
		display.setCursor(0,0);
		display.print("You sure?");
        display.display();
#ifdef DEBUG
        Serial.println("--Start dialogue activated");
#endif
		delay(2000);
		current = millis() + 10000;
		// ten second timeout 
		while (millis < current) {	
			if (!digitalRead(START)) { 
				clearRect(0, 0, 128, 32);
				display.setCursor(0,0);
				display.print("Time until release");
				started = true;
				display.setTextSize(2);
				break;
			}
		clearRect(0,0, 128, 9);
		display.setCursor(0,0);
		display.print("Ready to launch");		
		}
	}

	/* main loop 
	 * the loop that will be working most of the flight */

	while (started) {
#ifdef DEBUG
		Serial.println("Flight loop!");
		sprintf(debugOut, "started = %u\n released = %u\nremaining = %lu\nremainOut = %s\ncurrent = %lu", started, released, remaining, formatTime(remaining), current);
    	Serial.println(debugOut);
#endif
		remaining = FLIGHT_TIME;
       	clearRect(0, 9, 128, 32);
  		display.setCursor(0,8);
  		display.print(formatTime(remaining));
#ifdef DEBUG
        Serial.println(formatTime(remaining));
#endif
        display.display(); 
 		delay(1000);
  		if (remaining) remaining -= 1000;
		if (!remaining) {
			started = false; 
			released = true;
		}
  	}
  
	/* release sequence 
	 * release the glider, print a song quote. */

	if (released) {
#ifdef DEBUG
        Serial.println("Releasing glider");
#endif
  	    display.setCursor(0,0);
		clearRect(0,0,128,32);
		display.print("es gibt nichts\nwas mich haelt\nau revoir");
		display.display();
        current = millis() + SERVO_TIME;
		while (millis < current)servo.write(180); 	// unwind the nuts
		servo.write(90);							// stop rotation
		while (1) asm volatile("nop"); 				// idle
  	}	
}

/* function declarations */

void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	display.drawRect(x, y, w, h, BLACK);
	display.fillRect(x, y, w, h, BLACK);
}

char* formatTime(long time) {
	char* out = (char *) malloc(sizeof(char) * 21); // stops any complaints from the compiler.
	long hh, mm, ss;
	time = time/60000;
	hh = (long) floor(time / 60);
	mm = time % 60;
	ss = 0;
	sprintf(out, "%02.2hd:%02.2hd:%02.2hd", hh, mm ,ss);	
	return out;
}
