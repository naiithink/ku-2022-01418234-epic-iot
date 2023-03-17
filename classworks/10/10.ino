#include <HTTPClient.h>
#include <Preferences.h>
#include <WiFi.h>

#define STR_LEN 255
#define CONNECT_TIMEOUT (1000 * 10)

#define WIFI_POOL_KEY_SSID "ssid"
#define WIFI_POOL_KEY_PASS "pass"

const char reqHeaderSSID[] = "ssid";
const char reqHeaderPASS[] = "pass";

const String iftttToken = "DgNT6OXwf1AkGVBhXINwA";
const String iftttEvent = "gmail_callback";

WiFiServer server(80);
Preferences pref;

void setup()
{
    Serial.begin(115200);
    pref.begin("wifi_pool", false);

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("Epic IoT", "epic_iot");

    // Check if WiFi ssid and pass is already exists
    if (pref.getString(WIFI_POOL_KEY_SSID, "") != "" && pref.getString(WIFI_POOL_KEY_PASS, "") != "")
    {
        char wifi_ssid[STR_LEN];
        char wifi_pass[STR_LEN];

        pref.getString(WIFI_POOL_KEY_SSID).toCharArray(wifi_ssid, STR_LEN);
        pref.getString(WIFI_POOL_KEY_PASS).toCharArray(wifi_pass, STR_LEN);

        Serial.print("wifi_ssid: ");
        Serial.println(wifi_ssid);
        Serial.print("wifi_pass: ");
        Serial.println(wifi_pass);

        WiFi.begin(wifi_ssid, wifi_pass);

        bool connected = true;
        unsigned long start = millis();

        while (WiFi.status() != WL_CONNECTED)
        {
            if (millis() - start >= CONNECT_TIMEOUT)
            {
                connected = false;
                pref.clear();
                Serial.println("Cannot connect to WiFi");
                break;
            }

            Serial.print(".");
            delay(1000);
        }

        if (connected == true)
        {
            Serial.print("Connected to WiFi: ");
            Serial.println(WiFi.localIP());

            iftttTriggerEvent("hello, world");
            pref.clear();
        }
    }

    server.begin();
    Serial.println(WiFi.softAPIP());
}

void loop()
{
    WiFiClient client = server.available();

    if (client)
    {
        Serial.println("New client connected");

        while (client.connected())
        {
            if (client.available())
            {
                String request = client.readStringUntil('\r');
                Serial.println(request);

                if (request.indexOf(reqHeaderSSID) >= 1)
                {
                    String ssid = request.substring(request.indexOf(' ') + 1);

                    Serial.print("SSID:");
                    Serial.println(ssid);

                    pref.putString(WIFI_POOL_KEY_SSID, ssid);
                }
                else if (request.indexOf(reqHeaderPASS) >= 1)
                {
                    String pass = request.substring(request.indexOf(' ') + 1);

                    Serial.print("PASS:");
                    Serial.println(pass);

                    pref.putString(WIFI_POOL_KEY_PASS, pass);
                }
            }
        }

        Serial.println("Client disconnected");
    }
}

void iftttTriggerEvent(String val)
{
    HTTPClient http;

    http.begin("https://maker.ifttt.com/trigger/" + iftttEvent + "/with/key/" + iftttToken + "?value1=" + val);

    int httpCode = http.GET();
    if (httpCode > 0)
    {
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
    }
    else
    {
        Serial.println("Error on HTTP request");
    }

    http.end();
}
