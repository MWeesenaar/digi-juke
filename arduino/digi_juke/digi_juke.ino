/*
 * @Author: Mikki Weesenaar (github.com/mweesenaar)
 * @Hardwares: M5Core + Unit RFID
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5NanoC6.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>
//#include <ESPAsyncWebServer.h> // For future implementation of a webserver

#include "MFRC522_I2C.h"
#include "secrets.h"
//#include "root_ca.h" // For future implementation for TLS verification of the HA server


MFRC522 mfrc522(0x28);
NetworkClientSecure *client = new NetworkClientSecure;


void setupWifi() {
  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  NanoC6.begin();
  Wire.begin();

  setupWifi();
  NanoC6.setClock();
  delay(50);

  //client->setCACert(root_ca); // For future implementation for TLS verification of the HA server
  client->setInsecure();

  mfrc522.PCD_Init();
  Serial.println("Please put the card.");
}


String readData() {
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";  // Leading zero for single hex digits
    uidString += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) uidString += " ";  // Add space between bytes
  }
  if (uidString == "") {
    return "";
  }

  NanoC6.toggle_led();
  uidString.toUpperCase();
  Serial.println("UID: " + uidString);
  return uidString;
}

void sendToHomeAssistant(String uid) {
  HTTPClient https;
  Serial.printf("Sending the request to HA.");

  if (https.begin(*client, String(HA_WEBHOOK_URL) + HA_WEBHOOK_ID)) {
    https.addHeader("Content-Type", "Content-Type: application/json");
    String jsondata = "{\"UID\":\"" + uid + "\"}";

    int httpCode = https.POST(jsondata);

    // httpCode will be negative on error
    if (httpCode > 0) {
      Serial.printf("[HTTPS] code: %d", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTPS] Request... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  return;
}


void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(200);
    return;
  }

  String uidString = readData();
  if (uidString == "") {
    return;
  }

  sendToHomeAssistant(uidString);

  delay(3000);
  NanoC6.toggle_led();
}
