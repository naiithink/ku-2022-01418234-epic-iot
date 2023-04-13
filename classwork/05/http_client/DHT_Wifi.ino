#include "DHT.h"
#include "WiFi.h"


#define DHTPIN 17     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

const char* ssid = "TEE";
const char* pswd = "Tee00000";
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  dht.begin();
}

int i =0;
void loop() {


  if (client.connect("192.168.124.6",80)){
    Serial.println("connected");
  }
  else{
    Serial.println("cannot connected");
  }

  char buf[40];
  sprintf(buf,"GET /?u=%f?t=%f HTTP/1.1 \r\n\r\n%d",dht.readTemperature(),dht.readHumidity(),i++);
  delay(1000);
  client.println(buf);
  client.println("Host: worasait.pythonanywhere.com");
  client.println("Connection: close");
  client.println();
  while(client.connected()){
    while(client.available()){
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
    client.flush();
  }


  
//  // Wait a few seconds between measurements.
//  delay(2000);
//
//  // Reading temperature or humidity takes about 250 milliseconds!
//  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//  float h = dht.readHumidity();
//  // Read temperature as Celsius (the default)
//  float t = dht.readTemperature();
//  // Read temperature as Fahrenheit (isFahrenheit = true)
//  float f = dht.readTemperature(true);
//
//  // Check if any reads failed and exit early (to try again).
//  if (isnan(h) || isnan(t) || isnan(f)) {
//    Serial.println(F("Failed to read from DHT sensor!"));
//    return;
//  }
//
//  // Compute heat index in Fahrenheit (the default)
//  float hif = dht.computeHeatIndex(f, h);
//  // Compute heat index in Celsius (isFahreheit = false)
//  float hic = dht.computeHeatIndex(t, h, false);
//
//  Serial.print(F("Humidity: "));
//  Serial.print(h);
//  Serial.print(F("%  Temperature: "));
//  Serial.print(t);
//  Serial.print(F("°C "));
//  Serial.print(f);
//  Serial.print(F("°F  Heat index: "));
//  Serial.print(hic);
//  Serial.print(F("°C "));
//  Serial.print(hif);
//  Serial.println(F("°F"));
}
