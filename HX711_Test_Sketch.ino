#include "Arduino.h"
#include "HX711.h"

#define RST 4
#define DOUT  3
#define CLK  2

// Create our LoadCell object
HX711 scale(DOUT, CLK);

// --------------------------------------------------------------------------------------
// SET UP	SET UP   SET UP   SET UP   SET UP   SET UP   SET UP   SET UP   SET UP
// --------------------------------------------------------------------------------------
void setup() {
	Serial.begin(9600);

	scale.set_scale();
	scale.tare();  //Reset the scale to 0
}

// --------------------------------------------------------------------------------------
// LOOP    LOOP    LOOP    LOOP    LOOP    LOOP    LOOP    LOOP    LOOP    LOOP     LOOP
// --------------------------------------------------------------------------------------
void loop() {
	//Adjust to this calibration factor
	scale.set_scale(360.3);

	// Read an average of X readings
	Serial.println("Reading weight");
	float weight = scale.get_units(10);

	Serial.print(weight);
	Serial.println(" g");
}