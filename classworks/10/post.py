import requests

URL = "http://192.168.4.1/"
DATA = "Hello, ESP32!"

WIFI_SSID = "NAI"
WIFI_PASS = "epic_iot555"

HEADERS = {
    "ssid": WIFI_SSID,
    "pass": WIFI_PASS,
    "Connection": "close"
}

# response = requests.post(URL, data=DATA)
# print(response.text)

response = requests.post(URL, headers=HEADERS)
print(response.text)
