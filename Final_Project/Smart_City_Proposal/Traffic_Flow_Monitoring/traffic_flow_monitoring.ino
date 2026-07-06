#include <WiFi.h>
#include <PubSubClient.h>

// wifi credentials
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// mqtt configuration
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "jammu/smartcity/traffic";

// traffic sensors
const int lane1Sensor = 4;
const int lane2Sensor = 5;

// congestion alert
const int buzzerPin = 18;

// vehicle counters
volatile int lane1Count = 0;
volatile int lane2Count = 0;

// sensor state tracking
bool previousLane1 = HIGH;
bool previousLane2 = HIGH;

// timing variables
unsigned long previousPublish = 0;
unsigned long interval = 10000;

// mqtt objects
WiFiClient espClient;
PubSubClient client(espClient);

// connect to wifi
void connectWiFi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nwifi connected");
}

// reconnect mqtt
void reconnectMQTT() {

  while (!client.connected()) {

    Serial.println("connecting mqtt...");

    if (client.connect("JammuTrafficNode")) {
      Serial.println("mqtt connected");
    }
    else {
      delay(3000);
    }
  }
}

// classify traffic density
String getTrafficStatus(int totalVehicles) {

  if (totalVehicles < 10)
    return "LOW";

  if (totalVehicles < 25)
    return "MEDIUM";

  if (totalVehicles < 40)
    return "HIGH";

  return "CONGESTED";
}

// recommend signal duration
int recommendedSignalTime(int totalVehicles) {

  if (totalVehicles < 10)
    return 20;

  if (totalVehicles < 25)
    return 35;

  if (totalVehicles < 40)
    return 50;

  return 70;
}

void setup() {

  Serial.begin(115200);

  pinMode(lane1Sensor, INPUT);
  pinMode(lane2Sensor, INPUT);

  pinMode(buzzerPin, OUTPUT);

  connectWiFi();

  client.setServer(mqttServer, mqttPort);
}

void loop() {

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  // lane 1 vehicle detection
  bool currentLane1 = digitalRead(lane1Sensor);

  if (previousLane1 == HIGH && currentLane1 == LOW) {
    lane1Count++;
  }

  previousLane1 = currentLane1;

  // lane 2 vehicle detection
  bool currentLane2 = digitalRead(lane2Sensor);

  if (previousLane2 == HIGH && currentLane2 == LOW) {
    lane2Count++;
  }

  previousLane2 = currentLane2;

  // publish every 10 seconds
  if (millis() - previousPublish >= interval) {

    int totalVehicles = lane1Count + lane2Count;

    String trafficStatus = getTrafficStatus(totalVehicles);

    int signalTime = recommendedSignalTime(totalVehicles);

    float vehicleRate = totalVehicles * 6.0;

    // congestion alert
    if (trafficStatus == "CONGESTED") {
      digitalWrite(buzzerPin, HIGH);
    }
    else {
      digitalWrite(buzzerPin, LOW);
    }

    String payload = "{";
    payload += "\"lane1_count\":" + String(lane1Count) + ",";
    payload += "\"lane2_count\":" + String(lane2Count) + ",";
    payload += "\"total_vehicles\":" + String(totalVehicles) + ",";
    payload += "\"vehicle_rate_per_min\":" + String(vehicleRate) + ",";
    payload += "\"traffic_status\":\"" + trafficStatus + "\",";
    payload += "\"recommended_green_time\":" + String(signalTime);
    payload += "}";

    client.publish(mqttTopic, payload.c_str());

    Serial.println(payload);

    lane1Count = 0;
    lane2Count = 0;

    previousPublish = millis();
  }
}
