#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2901.h>
#include <BLE2902.h>
#include <DHTesp.h>

// initialize dht sensor
DHTesp dht;

// dht11 data pin
const int DHT_PIN = 15;


// environmental sensing service
#define SERVICE_UUID "0000181A-0000-1000-8000-00805F9B34FB"

// standard bluetooth sig characteristics
#define TEMP_UUID "00002A6E-0000-1000-8000-00805F9B34FB"
#define HUM_UUID  "00002A6F-0000-1000-8000-00805F9B34FB"

// custom timestamp characteristic
#define TIME_UUID "12345678-1234-5678-1234-56789ABCDEF1"

BLEServer *server;
BLEService *service;

BLECharacteristic *tempChar;
BLECharacteristic *humChar;
BLECharacteristic *timeChar;

bool deviceConnected = false;

// handle client connect/disconnect
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *) override {
    deviceConnected = true;
    Serial.println("BLE client connected");
  }

  void onDisconnect(BLEServer *s) override {
    deviceConnected = false;
    Serial.println("BLE client disconnected");
    delay(100);
    s->startAdvertising();
  }
};

void setup() {

  // initialize serial monitor
  Serial.begin(115200);

  // initialize dht11
  dht.setup(DHT_PIN, DHTesp::DHT11);

  // initialize ble
  BLEDevice::init("ESP32 Environmental Sensor");

  // create server
  server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  // create environmental sensing service
  service = server->createService(SERVICE_UUID);

  // create temperature characteristic
  tempChar = service->createCharacteristic(
      TEMP_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_NOTIFY);

  BLE2901 *tempDesc = new BLE2901();
  tempDesc->setDescription("Temperature (C)");
  tempChar->addDescriptor(tempDesc);
  tempChar->addDescriptor(new BLE2902());

  // create humidity characteristic
  humChar = service->createCharacteristic(
      HUM_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_NOTIFY);

  BLE2901 *humDesc = new BLE2901();
  humDesc->setDescription("Humidity (%)");
  humChar->addDescriptor(humDesc);
  humChar->addDescriptor(new BLE2902());

  // create timestamp characteristic
  timeChar = service->createCharacteristic(
      TIME_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_NOTIFY);

  BLE2901 *timeDesc = new BLE2901();
  timeDesc->setDescription("Timestamp (seconds)");
  timeChar->addDescriptor(timeDesc);
  timeChar->addDescriptor(new BLE2902());

  // start service
  service->start();

  // start advertising
  BLEAdvertising *adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("BLE Environmental Sensor Started");
}

void loop() {

  // read sensor every 3 seconds
  TempAndHumidity data = dht.getTempAndHumidity();

  if (!isnan(data.temperature) && !isnan(data.humidity)) {

    // encode according to bluetooth sig specification
    int16_t tempValue = (int16_t)(data.temperature * 100);
    uint16_t humValue = (uint16_t)(data.humidity * 100);

    tempChar->setValue((uint8_t *)&tempValue, sizeof(tempValue));
    humChar->setValue((uint8_t *)&humValue, sizeof(humValue));

    String timestamp = String(millis() / 1000);
    timeChar->setValue(timestamp.c_str());

    if (deviceConnected) {
      tempChar->notify();
      humChar->notify();
      timeChar->notify();
    }

    Serial.println("----------------------------");
    Serial.print("Temperature : ");
    Serial.print(data.temperature);
    Serial.println(" C");

    Serial.print("Humidity    : ");
    Serial.print(data.humidity);
    Serial.println(" %");

    Serial.print("Timestamp   : ");
    Serial.println(timestamp);
  } else {
    Serial.println("Failed to read DHT11");
  }

  delay(3000);
}
