/* includes */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

/* defines */

// display
#define ADDR 0x78
#define H 128
#define W 64
#define RST -1
// reserved pins
#define START 0
#define SERVO // TODO add pin

// etc
#define FLIGHT_TIME (115 * 60000)

/* global vars */
bool started, released = false;
unsigned long remaining = FLIGHT_TIME;
char* remainOut = (char*) malloc(sizeof(char) * 8); // reserve 8 chars for formatted time output
unsigned long current;

Adafruit_SSD1306 display(W, H, &Wire, RST);

/* function declarations */

void timeFormat(); // TODO : add implemention!

int main(void) {
	// setup here
	display.begin(SSD1306_SWITCHCAPVCC, ADDR);
	display.clearDisplay();
	display.setTextSize(2);
	display.print("time till release:");
	display.setCursor(0,16);
	// remainOut = timeFormat();
	display.print(String(remaining)); // TODO : Add formatted time output and replace String(remaining) with remainOut 

	pinMode(START, INPUT);
	while(1) {
		// loop here
		display.display(); // update what has been drawn on display
		if (!started && digitalRead(START)) {
			display.setCursor(0,32);
			display.print("Sure?");

		}
	}	
}
