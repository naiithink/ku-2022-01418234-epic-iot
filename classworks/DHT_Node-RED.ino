#include <WiFi.h>
#include <WiFiClient.h>

#include <HTTPClient.h>

#include <DHT.h>

#define SSID "Narukami"
#define PASS "95131150"

#define DHT_PIN 17
#define DHT_TYPE DHT22

// Networking
WiFiClient client;

const char *host = "http://20.251.60.96";
const char *port = "1880";
const char *path = "dht";
char query[40];
char requestURL[40];
int respondCode;
String respondMessage;

// DHT
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  Serial.println("INFO: Connected to WiFi");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.printf("INFO: Humidity: %f\n", humid);
  Serial.printf("INFO: Temperature: %f\n", temp);

  sprintf(query, "t=%f&h=%f", temp, humid);
  sprintf(requestURL, "%s:%s/%s?%s", host, port, path, query);

  Serial.printf("Server URL: %s\n", requestURL);
  Serial.println("Request method: GET");

  HTTPClient http;
  http.begin(requestURL);
  Serial.printf("INFO: Connected to host %s:%s\n", host, port);

  respondCode = http.GET();

  delay(2000);

  respondMessage = http.getString();

  Serial.printf("Respond status: %d\n", respondCode);
  Serial.printf("Server response: %s\n", respondMessage);

  http.end();
}