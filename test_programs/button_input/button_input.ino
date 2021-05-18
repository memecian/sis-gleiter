/* defines */
#define BTN0 13
#define BTN1 12

/* global variables */
char *out, *out_buf = (char*) malloc(sizeof(char) * 16);

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
