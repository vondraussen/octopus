#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>

#define PIN 13 //NeoPixl

char ssid[] = "CHANGEME";
char pass[] = "CHANGEME";

IPAddress server(192, 168, 1, 1); // add the ip of your mqtt broker hardware
// Callback function header
void callback(char* topic, byte* payload, unsigned int length);
void colorWipe(uint32_t c, uint8_t wait);

Adafruit_BME280 bme; // I2C
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRBW + NEO_KHZ800);
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(0, 0, 0), 10);
  delay(1000);

  Serial.println(F("BME280 test"));
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);              // Connect to WPA2 network
    uint8_t timeout = 10;                         // Set a timeout variable
    while (timeout && (WiFi.status() != WL_CONNECTED)) {
      timeout--;                                  // Decrement timeout
      delay(1000);                                // Delay for one second
    }
  }
  Serial.println("Connected to network.");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (client.connect("arduinoClient")) {
    client.subscribe("octo/led1/red");
    client.subscribe("octo/led1/green");
    client.subscribe("octo/led1/blue");
  }
}

void loop() {
  char buffer[10];

  dtostrf(bme.readPressure() / 100.0F, 5, 1, buffer);
  client.publish("octo/press",buffer);
  dtostrf(bme.readTemperature(), 5, 1, buffer);
  client.publish("octo/temp",buffer);
  itoa(bme.readHumidity(), buffer, 10);
  client.publish("octo/humi",buffer);
  client.loop();
  delay(100);
}

void callback(char* topic, byte* payload, unsigned int length) {
  static uint8_t red=0, green=0, blue=0;

  char* buf_str = (char*)calloc(length+1, sizeof(char));
  memcpy(buf_str, (const char*)payload, length);

  if (!strcmp(topic, "octo/led1/red")) {
    red = strtol((const char*)buf_str, NULL, 10);
  }
  if (!strcmp(topic, "octo/led1/green")) {
    green = strtol((const char*)buf_str, NULL, 10);
  }
  if (!strcmp(topic, "octo/led1/blue")) {
    blue = strtol((const char*)buf_str, NULL, 10);
  }
  strip.setPixelColor(0, red, green, blue, 0);
  strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
