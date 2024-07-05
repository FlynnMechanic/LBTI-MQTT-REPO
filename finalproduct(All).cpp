//Programm konnte nicht zusammengefügt getestet werden, da Wokwi zu lange zum Kompelieren gebraucht hat.

#include <WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT configuration parameters
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com"; // Public MQTT broker

// WiFi and MQTT client objects
WiFiClient espClient;
PubSubClient client(espClient);

// Sensor and actuator pins
const int pirPin = 21; 
const int ledPin1 = 23; 
const int ledPin2 = 22; 
const int buttonPin = 19;

unsigned long presenceStartTime = 0;
const unsigned long presenceThreshold = 10000; // Time threshold for presence detection

int temperature = 20; // Test temperature, will be replaced by sensor values
bool buttonLedState = LOW; // State of the button-controlled LED
bool lastButtonState = HIGH; // Previous state of the button (HIGH due to Pull-Up)
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

// Function to set up WiFi connection
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Callback function for incoming MQTT messages
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Function to ensure MQTT broker connection
void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT Broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected");
      client.subscribe("test/topic");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 5 seconds...");
      delay(5000);
    }
  }
}

// Function to publish a message to a topic
void publish_message(const char* topic, const char* message) {
  client.publish(topic, message);
}

// Initialization function
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Button with internal Pull-Up resistor

  Serial.println("Setup complete");
}

// Main loop function
void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  int reading = digitalRead(buttonPin);

  // Debounce the button
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) { // Button pressed
      buttonLedState = !buttonLedState; // Toggle LED state
      publish_message("button/state", buttonLedState ? "ON" : "OFF");
    }
  }

  // Save the current button state
  lastButtonState = reading;

  // Set the LED state based on the toggling value
  digitalWrite(ledPin2, buttonLedState);

  // Presence detection with PIR sensor
  int pirState = digitalRead(pirPin);

  if (pirState == HIGH) {
    if (presenceStartTime == 0) {
      presenceStartTime = millis(); // Set the start time of presence
    }
    if (millis() - presenceStartTime >= presenceThreshold) {
      Serial.println("Presence detected");
      publish_message("sensor/presence", "Presence detected");
    }
  } else {
    presenceStartTime = 0; // Reset presence
    Serial.println("No presence");
    publish_message("sensor/presence", "No presence");
  }

  // Climate control LED based on temperature value
  if (temperature < 25) {
    digitalWrite(ledPin1, HIGH); // Turn on climate control LED
    publish_message("climate/led", "ON");
  } else {
    digitalWrite(ledPin1, LOW); // Turn off climate control LED
    publish_message("climate/led", "OFF");
  }

  delay(100); // Short delay for stability
}
