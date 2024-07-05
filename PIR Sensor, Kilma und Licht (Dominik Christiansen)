const int pirPin = 21; 
const int ledPin1 = 23; 
const int ledPin2 = 22; 
const int buttonPin = 19; 

unsigned long presenceStartTime = 0;
const unsigned long presenceThreshold = 10000; // Angabe ab wann eine Person als Anwesend gilt

int temperature = 20; // Testtemperatur, wird ersetzt mit Sensor werten
bool buttonLedState = LOW; // Lichtschalter zum An und Aus machen
bool lastButtonState = HIGH; // Vorheriger Zustand des Buttons (HIGH wegen Pull-Up)
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Entprellzeit in Millisekunden

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Button mit internem Pull-Up-Widerstand

  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Entprellung des Buttons
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) { // Button gedrückt
      buttonLedState = !buttonLedState; // LED-Zustand toggeln
    }
  }

  // Speichere den aktuellen Button-Zustand
  lastButtonState = reading;

  // Setze den LED-Zustand basierend auf dem toggelnden Wert
  digitalWrite(ledPin2, buttonLedState);

  // Anwesenheitserkennung mit PIR-Sensor
  int pirState = digitalRead(pirPin);

  if (pirState == HIGH) {
    if (presenceStartTime == 0) {
      presenceStartTime = millis(); // Startzeit der Anwesenheit setzen
    }
    if (millis() - presenceStartTime >= presenceThreshold) {
      Serial.println("Anwesend");
    }
  } else {
    presenceStartTime = 0; // Anwesenheit zurücksetzen
    Serial.println("Nicht anwesend");
  }

  // Klimaanlagen-LED basierend auf Temperaturwert
  if (temperature < 25) {
    digitalWrite(ledPin1, HIGH); // Klimaanlagen-LED einschalten
  } else {
    digitalWrite(ledPin1, LOW); // Klimaanlagen-LED ausschalten
  }

  delay(100); // Kurze Verzögerung zur Stabilisierung
}
