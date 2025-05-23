#include <WiFi.h>
#include <WiFiClientSecure.h>

// Replace with your network credentials
const char* ssid = "wifi ssid";
const char* password = "wifi password";

// Replace with your Telegram Bot Token and Chat ID
const char* botToken = "**********:*****************_*************_***";
const char* chatID = "*********";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());

  // Send a Telegram message
  sendTelegramMessage("Hello from ESP32!");
}

void loop() {
  // Your loop code here
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Use this if you don't have a root certificate

    // Connect to Telegram API
    if (client.connect("api.telegram.org", 443)) {
      String url = "/bot";
      url += botToken;
      url += "/sendMessage?chat_id=";
      url += chatID;
      url += "&text=";
      url += message;

      // Send HTTP GET request
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: api.telegram.org\r\n" +
                   "Connection: close\r\n\r\n");

      // Wait for response
      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      // Print response
      String response = client.readString();
      Serial.println("Telegram Response:");
      Serial.println(response);
    } else {
      Serial.println("Connection to Telegram API failed.");
    }
  } else {
    Serial.println("Wi-Fi not connected.");
  }
}
