#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <HardwareSerial.h>

// wifi credentials
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// mqtt configuration
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "jammu/smartcity/airquality";

// sensor definitions
#define MQ135_PIN 34
#define DHT_PIN 15
#define DHT_TYPE DHT22

// alert peripherals
const int buzzerPin = 18;
const int ledPin = 2;

// dht object
DHT dht(DHT_PIN, DHT_TYPE);

// serial port for pms5003
HardwareSerial pmsSerial(2);

// network clients
WiFiClient espClient;
PubSubClient client(espClient);

// timing variables
unsigned long lastPublish = 0;
const long publishInterval = 10000;

// sensor values
int gasLevel = 0;
float temperature = 0;
float humidity = 0;
int pm25 = 0;
int pm10 = 0;

// connect wifi
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

    if (client.connect("JammuAQINode")) {
      Serial.println("mqtt connected");
    } else {
      delay(3000);
    }
  }
}

// estimate aqi category
String getAQIStatus(int aqi) {

  if (aqi <= 50) return "GOOD";
  if (aqi <= 100) return "SATISFACTORY";
  if (aqi <= 200) return "MODERATE";
  if (aqi <= 300) return "POOR";
  if (aqi <= 400) return "VERY_POOR";

  return "SEVERE";
}

// simplified aqi estimation
int calculateAQI(int pm25, int gasLevel) {

  int particulateScore = pm25 * 2;
  int gasScore = map(gasLevel, 0, 4095, 0, 300);

  return max(particulateScore, gasScore);
}

// calculate heat index
float calculateHeatIndex(float temp, float hum) {

  return -8.784695 +
         1.61139411 * temp +
         2.338549 * hum -
         0.14611605 * temp * hum -
         0.012308094 * temp * temp -
         0.016424828 * hum * hum +
         0.002211732 * temp * temp * hum +
         0.00072546 * temp * hum * hum -
         0.000003582 * temp * temp * hum * hum;
}

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // pms5003 serial communication
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);

  connectWiFi();

  client.setServer(mqttServer, mqttPort);
}

void loop() {

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  if (millis() - lastPublish >= publishInterval) {

    // read sensors
    gasLevel = analogRead(MQ135_PIN);

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    // simulated pms5003 values
    pm25 = random(15, 180);
    pm10 = random(30, 250);

    int aqi = calculateAQI(pm25, gasLevel);

    String aqiStatus = getAQIStatus(aqi);

    float heatIndex = calculateHeatIndex(
      temperature,
      humidity
    );

    // pollution alert
    if (aqi > 200) {

      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ledPin, HIGH);

    } else {

      digitalWrite(buzzerPin, LOW);
      digitalWrite(ledPin, LOW);
    }

    // health advisory generation
    String advisory;

    if (aqi <= 100)
      advisory = "SAFE_FOR_OUTDOOR_ACTIVITY";
    else if (aqi <= 200)
      advisory = "LIMIT_PROLONGED_EXPOSURE";
    else
      advisory = "AVOID_OUTDOOR_ACTIVITY";

    // mqtt payload
    String payload = "{";

    payload += "\"temperature\":" + String(temperature) + ",";
    payload += "\"humidity\":" + String(humidity) + ",";
    payload += "\"gas_level\":" + String(gasLevel) + ",";
    payload += "\"pm25\":" + String(pm25) + ",";
    payload += "\"pm10\":" + String(pm10) + ",";
    payload += "\"aqi\":" + String(aqi) + ",";
    payload += "\"aqi_status\":\"" + aqiStatus + "\",";
    payload += "\"heat_index\":" + String(heatIndex) + ",";
    payload += "\"health_advisory\":\"" + advisory + "\"";

    payload += "}";

    client.publish(
      mqttTopic,
      payload.c_str()
    );

    Serial.println(payload);

    lastPublish = millis();
  }
}
