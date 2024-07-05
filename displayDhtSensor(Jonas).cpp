#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "DHTesp.h"

// ILI9341 Pins
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST 12

// Initialisierung des ILI9341 Displays
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const int DHT_PIN = 1;

DHTesp dhtSensor;

void setup() {
Serial.begin(115200);
dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
tft.begin();
tft.setRotation(3); // Setze die Ausrichtung auf "Querformat"
tft.fillScreen(ILI9341_BLACK);
tft.setTextColor(ILI9341_WHITE);
tft.setTextSize(2);
}

void loop() {

TempAndHumidity  data = dhtSensor.getTempAndHumidity();

// Lösche den vorherigen Text
tft.fillScreen(ILI9341_BLACK);

tft.setCursor(10, 50);
tft.print("Temp: ");
tft.print((data.temperature, 2));

tft.setCursor(50, 50);
tft.print("Luftfeuchtigkeit: ")
tft.print((data.temperature, 1));


// Warte 2 Sekunden, bevor die Werte erneut gelesen werden
delay(2000);
}
