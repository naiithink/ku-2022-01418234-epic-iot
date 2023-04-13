#include <DHT.h>
#include <TridentTD_LineNotify.h>
#include <WiFi.h>

#define STR_LEN 255
#define THRESHOLD 26.5
#define DELAY_SEC 10

#define SSID "Narukami"
#define PASS "95131150"

#define DHT_PIN 17
#define DHT_TYPE DHT22

#define LINE_TOKEN "rT0CwNk6zuRyvbedybN1GDJSfQXoNEAkmKZkuwDHg3Z"
#define MESSAGE "ร้อนเกิ้น"

DHT dht(DHT_PIN, DHT_TYPE);

void setup()
{
    Serial.begin(115200);
    pinMode(DHT_PIN, INPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.println();
    Serial.println("INFO: Connected to WiFi");
    Serial.println(WiFi.localIP());

    LINE.setToken(LINE_TOKEN);
    LINE.notify("Connected");

    dht.begin();
}

void loop()
{
    char msg[STR_LEN];
    float temp = dht.readTemperature();

    if (temp >= THRESHOLD)
    {
        sprintf(msg, "%.2f˚C %s", temp, MESSAGE);
        Serial.println(LINE.notify(msg));
    }
    else
        Serial.printf("Normal temperature: %.2f˚C\n", temp);

    delay(1000 * DELAY_SEC);
}
