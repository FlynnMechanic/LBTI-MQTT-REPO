#include <WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT configuration parameters
const char* ssid = "******";
const char* password = "******";
const char* mqtt_server = "broker.hivemq.com"; // Public MQTT broker

// WiFi and MQTT client objects
WiFiClient espClient;
PubSubClient client(espClient);

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
}

// Main loop function
void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  //
}