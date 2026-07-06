#include <WiFi.h>
#include <PubSubClient.h>

// wifi credentials
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// mqtt configuration
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "jammu/smartcity/waste";

// ultrasonic sensor pins
const int trigPin = 5;
const int echoPin = 18;

// additional sensors
const int gasPin = 34;
const int flamePin = 25;
const int tiltPin = 26;

// alerts
const int buzzerPin = 27;
const int ledPin = 2;

// bin configuration
const float binHeight = 60.0;

// mqtt objects
WiFiClient espClient;
PubSubClient client(espClient);

// publish timer
unsigned long lastPublish = 0;
const long publishInterval = 10000;

// wifi connection
void connectWiFi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nwifi connected");
}

// mqtt reconnect
void reconnectMQTT() {

  while (!client.connected()) {

    if (client.connect("JammuWasteNode")) {
      Serial.println("mqtt connected");
    } else {
      delay(3000);
    }
  }
}

// ultrasonic distance measurement
float getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  return duration * 0.034 / 2.0;
}

// collection priority logic
String getPriority(float fillLevel) {

  if (fillLevel < 50)
    return "LOW";

  if (fillLevel < 80)
    return "MEDIUM";

  if (fillLevel < 95)
    return "HIGH";

  return "URGENT";
}

void setup() {

  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(flamePin, INPUT);
  pinMode(tiltPin, INPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  connectWiFi();

  client.setServer(mqttServer, mqttPort);
}

void loop() {

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  if (millis() - lastPublish >= publishInterval) {

    // measure bin level
    float distance = getDistance();

    float fillLevel =
      ((binHeight - distance) / binHeight) * 100;

    fillLevel = constrain(fillLevel, 0, 100);

    // gas sensor reading
    int gasLevel = analogRead(gasPin);

    // flame detection
    bool fireDetected = digitalRead(flamePin) == LOW;

    // tamper detection
    bool tiltDetected = digitalRead(tiltPin);

    // estimated remaining capacity
    float remainingCapacity = 100 - fillLevel;

    // collection priority
    String priority = getPriority(fillLevel);

    // estimated collection time
    String estimatedPickup;

    if (fillLevel < 50)
      estimatedPickup = "WITHIN_48_HOURS";
    else if (fillLevel < 80)
      estimatedPickup = "WITHIN_24_HOURS";
    else
      estimatedPickup = "IMMEDIATE_COLLECTION";

    // emergency alert logic
    bool emergency =
      fireDetected ||
      gasLevel > 2500 ||
      tiltDetected ||
      fillLevel >= 95;

    digitalWrite(buzzerPin, emergency);
    digitalWrite(ledPin, emergency);

    // simulate battery monitoring
    int batteryLevel = random(60, 100);

    // mqtt payload
    String payload = "{";

    payload += "\"fill_level\":" + String(fillLevel, 1) + ",";
    payload += "\"remaining_capacity\":" + String(remainingCapacity, 1) + ",";
    payload += "\"collection_priority\":\"" + priority + "\",";
    payload += "\"estimated_pickup\":\"" + estimatedPickup + "\",";
    payload += "\"gas_level\":" + String(gasLevel) + ",";
    payload += "\"fire_detected\":" + String(fireDetected ? "true" : "false") + ",";
    payload += "\"tamper_detected\":" + String(tiltDetected ? "true" : "false") + ",";
    payload += "\"battery_level\":" + String(batteryLevel);

    payload += "}";

    client.publish(mqttTopic, payload.c_str());

    Serial.println(payload);

    lastPublish = millis();
  }
}
