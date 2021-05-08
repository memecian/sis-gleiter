 /* includes */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

/* defines */

// display
#define ADDR 0x3c
#define H 32
#define W 128
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
unsigned long current = 0;

Adafruit_SSD1306 display(W, H, &Wire, RST);

/* function declarations */

//void timeFormat(); // TODO : add implemention!

void setup(){
 // setup here
  display.begin(SSD1306_SWITCHCAPVCC, ADDR);
    
  display.clearDisplay();
  display.drawPixel(10,10, SSD1306_WHITE);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); 
  display.setTextSize(0);
  display.print("time till release:");
  // remainOut = timeFormat();
  display.setCursor(0,10);
  display.setTextSize(2);
  
  pinMode(START, INPUT);
}

void loop(){
    // loop here
	
	display.print(String(remaining)); // TODO : Add formatted time output and replace String(remaining) with remainOut 
    display.display(); // update what has been drawn on display
   	if (!started) {
	 	current = millis();
		while(current < millis() + 1000);
		remaining -= 1000;
	}	
	
}
