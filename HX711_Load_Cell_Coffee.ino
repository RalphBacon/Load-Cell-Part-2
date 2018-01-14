#include "HX711.h"
#define RST 4
#define DOUT  3
#define CLK  2
#define LED 5
#define delayBetweenFlashes 60000

HX711 scale(DOUT, CLK);

float calibration_factor = -2680.00;
unsigned long time;

void setup() {
  Serial.begin(9600);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  // Reset pin (to rezero the weight not reset the board!)
  pinMode(RST, INPUT_PULLUP);

  // LED output pin
  pinMode(LED, OUTPUT);

  // Acknowledge all OK
  for (int cnt = 0; cnt < 6; cnt++) {
    digitalWrite(LED, !digitalRead(LED));
    delay(1000);
  }

  // Randomize
  randomSeed(analogRead(0));
}

void loop() {

  //Adjust to this calibration factor
  scale.set_scale(calibration_factor);

  // Read an average of X readings
  Serial.println("Reading weight");
  float weight = scale.get_units(10);

  // An intermediate weigth value that we round according to some rules
  int netWeight = 0 ;

  // Make scale more sensitive at lower end
  // Weight > X then just round to nearest integer
  if (weight >= 5.0) {
    netWeight = (weight * 10.0);
    weight = (int)(0.5 + (netWeight / 10.0));
  }
  // Weight < Y then call it zero
  else if (weight > -0.01 && weight <= 0.01) {
    weight = 0;
  }
  // Any other weight will have 1 dec. place of precision
  else {
    netWeight = (weight * 10.0);
    weight = (netWeight / 10.0);
  }

  Serial.print("Reading: ");
  Serial.print(weight, 2);
  Serial.println(" g");

  // If weight above zero flash LED every X seconds
  if (weight > 5) {
    Serial.println("Coffee in cup - LED flash?");

    if (millis() - time > delayBetweenFlashes) {

      Serial.println("It's been a minute or so - flash LED");

      for (int cnt = 0; cnt < 20; cnt++) {
        digitalWrite(LED, !digitalRead(LED));
        delay(100);
      }

      // Update our snapshot time so we start the next minute
      time = millis();
    }
    else if (random(300) < 10) {

      Serial.println("Just a random - flash LED");

      for (int cnt = 0; cnt < 2; cnt++) {
        digitalWrite(LED, !digitalRead(LED));
        delay(50);
      }
    }
  }

  // Has user requested a Zero Reset?
  Serial.println("Checking RST request");
  int rstRequest = digitalRead(RST);
  if (rstRequest == LOW) {
    Serial.println("Resetting ZERO value - LED flash");
    for (int cnt = 0; cnt < 10; cnt++) {
      digitalWrite(LED, !digitalRead(LED));
      delay(500);
    }
    scale.set_scale();
    scale.tare();  //Reset the scale to 0
  }
}
