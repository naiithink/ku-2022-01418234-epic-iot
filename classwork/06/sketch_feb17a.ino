/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLAllvhdfB"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "G0wm0Nyzni1EW2FnW5Xu5tq4tvkhSicc"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


// Temp
#define DHTPIN 17
#define DHTTYPE DHT22

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

DHT dht(DHTPIN, DHTTYPE);
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TEE";
char pass[] = "Tee00000";

BlynkTimer timer;
int i = 0;
void sendSensor()
{
  
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  char buffer[100];

  sprintf(buffer,"%.2f",t);
  
//  if (isnan(t)) {
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  }
  Serial.println(buffer);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, i++);
  
  Blynk.virtualWrite(V5 , buffer); // select your virtual pins accordingly
}

// This function is called every time the Virtual Pin 0 state changes
// BLYNK_WRITE(V0)
// {
//   // Set incoming value from pin V0 to a variable
//   int value = param.asInt();

//   // Update state
//   Blynk.virtualWrite(V1, value);

//   Serial.println(value == 0 ? '0' : '1');
// }

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}



// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, i++);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN , ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(2000L, sendSensor);
  dht.begin();
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
