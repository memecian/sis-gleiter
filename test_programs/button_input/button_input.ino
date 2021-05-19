/* defines */
#define BTN0 11
#define BTN1 12

/* global variables */
char out[8], out_buf[8];

void setup() {
    pinMode(BTN0, INPUT_PULLUP);
    pinMode(BTN1, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    sprintf(out, "%x|\t%x", digitalRead(BTN0), digitalRead(BTN1)); // zustaende der knoepfe werden hier gespeichert
    if (strcmp(out, out_buf)) Serial.println(out); // sende nur, wenn sich die zustaende aendern 
    strcpy(out_buf, out); // puffer update
}
