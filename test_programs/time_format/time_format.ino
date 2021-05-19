/* includes */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

/* defines */

// display
#define ADDR 		0x3c
#define H 			32
#define W		 	128
#define RST 		-1

Adafruit_SSD1306 display(W, H, &Wire, RST);

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, ADDR);
  	display.setRotation(2); 
	display.clearDisplay();
  	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0,0); 
  	display.setTextSize(0);
    Serial.begin(9600);
}

void loop() {
    Serial.println(formatTime(millis()));
}

char* formatTime(long time) {
	char out[20] ;
	long hh, mm, ss;
	time = floor(time/1000); // convert to seconds from milliseconds
	mm = time / 60;
	ss = time % 60;
	hh = mm / 60;	
	mm = mm % 60;
	sprintf(out, "%d:%d:%d", hh, mm ,ss);
	Serial.print(hh);
    Serial.print(':');
    Serial.print(mm);
    Serial.print(':');
    Serial.println(ss);
    return out;
}