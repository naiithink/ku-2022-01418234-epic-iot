#include <HTTPClient.h>
#include "DHT.h"

#define DHT_PIN 17
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "Narukami";
const char* password = "95131150";
const String key = "DgNT6OXwf1AkGVBhXINwA";
const String event = "line_us";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pinMode(DHT_PIN, INPUT);
  dht.begin();
}

void loop() {
  
  send_event(dht.readHumidity());
  delay(1000 * 5);
}

void send_event(float val) {
  HTTPClient http;

  http.begin("https://maker.ifttt.com/trigger/" + event + "/with/key/" + key + "?value1=" + val);

  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  } else {
    Serial.println("Error on HTTP request");
  }

  http.end();
}