#include <Preferences.h>
#include <WiFi.h>

#define WIFI_POOL_KEY_SSID "ssid"
#define WIFI_POOL_KEY_PASS "pass"

const char reqHeaderSSID[] = "ssid";
const char reqHeaderPASS[] = "pass";

WiFiServer server(80);
Preferences pref;

void setup()
{
    Serial.begin(115200);
    pref.begin("WiFi Pool", false);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("Epic IoT", "epic_iot");

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
