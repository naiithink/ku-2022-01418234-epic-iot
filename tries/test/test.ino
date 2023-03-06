#include <DHT.h>
#include <TridentTD_LineNotify.h>
#include <WiFi.h>

#define LOG(x) Serial.printf("INFO: %s\n", x)

#define THRESHOLD 26.5
#define DELAY_SEC 10

#define STR_LEN 255
#define ASCII_DEGREE 248

#define SSID "NAI-II"
#define PASS "epic_iot"

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
    char msg[STR_LEN] = "";
    float temp = 0.0f;

    if ((temp = dht.readTemperature()) >= THRESHOLD)
    {
        sprintf(msg, "%.2f%cC %s", temp, ASCII_DEGREE, MESSAGE);
        Serial.println(LINE.notify(msg) == 1 ? "INFO: Message sent." : "INFO: Failed sending message.");
    }
    else
        Serial.printf("INFO: Normal temperature: %.2f%cC\n", temp, ASCII_DEGREE);

    delay(1000 * DELAY_SEC);
}
