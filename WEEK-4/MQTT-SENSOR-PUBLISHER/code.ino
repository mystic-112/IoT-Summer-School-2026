#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

// wokwi provides a free virtual wifi network
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// mqtt broker details
const char* mqttBroker = "test.mosquitto.org";
const int mqttPort = 1883;

// mqtt topics used in this project
const char* temperatureTopic =
"iitjammu/summer26/abhishektandon/temperature";

const char* humidityTopic =
"iitjammu/summer26/abhishektandon/humidity";

const char* ledControlTopic =
"iitjammu/summer26/abhishektandon/led_control";

// hardware pin assignments
const int dhtPin = 15;
const int ledPin = 2;

// create wifi and mqtt client objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// create dht sensor object
DHTesp dht;

// timer variables for periodic publishing
unsigned long previousPublishTime = 0;
const unsigned long publishInterval = 5000;


// connect esp32 to wifi network
void connectToWiFi() {

  Serial.print("connecting to wifi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("wifi connected");

  Serial.print("ip address: ");
  Serial.println(WiFi.localIP());
}


// handle incoming mqtt messages
void mqttCallback(char* topic, byte* payload,
                  unsigned int length) {

  String message = "";

  // convert byte array into string
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("message received on topic: ");
  Serial.println(topic);

  Serial.print("payload: ");
  Serial.println(message);

  // turn led on if mqtt message is ON
  if (message == "ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("led turned on");
  }

  // turn led off if mqtt message is OFF
  else if (message == "OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("led turned off");
  }
}


// reconnect automatically if mqtt connection drops
void connectToMQTT() {

  while (!mqttClient.connected()) {

    Serial.print("connecting to mqtt broker");

    // generate a unique client id
    String clientId =
      "esp32-client-" + String(random(1000, 9999));

    if (mqttClient.connect(clientId.c_str())) {

      Serial.println(" connected");

      // subscribe for remote led control
      mqttClient.subscribe(ledControlTopic);

      Serial.println("subscribed to led control topic");
    }

    else {

      Serial.print("failed with error code ");
      Serial.println(mqttClient.state());

      Serial.println("retrying in 2 seconds");
      delay(2000);
    }
  }
}


void setup() {

  Serial.begin(115200);

  // configure onboard led pin
  pinMode(ledPin, OUTPUT);

  // initialize dht22 sensor
  dht.setup(dhtPin, DHTesp::DHT22);

  // establish wifi connection
  connectToWiFi();

  // configure mqtt broker settings
  mqttClient.setServer(mqttBroker, mqttPort);

  // register callback function
  mqttClient.setCallback(mqttCallback);
}


void loop() {

  // reconnect if mqtt connection is lost
  if (!mqttClient.connected()) {
    connectToMQTT();
  }

  // keep mqtt client alive
  mqttClient.loop();

  // publish sensor values every 5 seconds
  if (millis() - previousPublishTime >= publishInterval) {

    previousPublishTime = millis();

    // read current sensor values
    TempAndHumidity sensorData =
      dht.getTempAndHumidity();

    // prepare temperature json payload
    String temperaturePayload =
      "{\"value\": " +
      String(sensorData.temperature) +
      ", \"unit\": \"C\", \"ts\": " +
      String(millis()) +
      "}";

    // prepare humidity json payload
    String humidityPayload =
      "{\"value\": " +
      String(sensorData.humidity) +
      ", \"unit\": \"%\", \"ts\": " +
      String(millis()) +
      "}";

    // publish temperature data
    mqttClient.publish(
      temperatureTopic,
      temperaturePayload.c_str()
    );

    // publish humidity data
    mqttClient.publish(
      humidityTopic,
      humidityPayload.c_str()
    );

    Serial.println();

    Serial.print("temperature published: ");
    Serial.println(temperaturePayload);

    Serial.print("humidity published: ");
    Serial.println(humidityPayload);

    Serial.println();
  }
}
