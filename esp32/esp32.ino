#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>

/* ---------------- RFID ---------------- */
#define SS_PIN 5
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

/* ---------------- BUZZER ---------------- */
#define BUZZER_PIN 27

/* ---------------- WIFI ---------------- */
const char* ssid = "MUKESH_5G";
const char* password = "Mk@.2007.";

/* ---------------- SERVER ---------------- */
const char* serverURL = "http://192.168.0.106:5000/rfid";
const char* API_KEY  = "SMARTPARK_ESP32_KEY_2026";

/* ---------------- TIME (IST) ---------------- */
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

/* ---------------- USER DATABASE ---------------- */
struct User {
  String uid;
  String name;
  int age;
  bool isInside;
};

User users[] = {
  {"b57a3706", "Mukesh", 18, false},
  {"6293a65c", "Sudarsan", 19, false}
};

const int USER_COUNT = sizeof(users) / sizeof(users[0]);

/* ---------------- SCAN CONTROL ---------------- */
unsigned long lastScanTime = 0;
const unsigned long scanDelay = 3000;

/* ---------------- BUZZER FUNCTIONS ---------------- */
void entryBeep() {
  for (int i = 0; i < 2; i++) {
    ledcWriteTone(BUZZER_PIN, 2000);
    delay(100);
    ledcWriteTone(BUZZER_PIN, 0);
    delay(100);
  }
}

void exitBeep() {
  ledcWriteTone(BUZZER_PIN, 3500);
  delay(1000);
  ledcWriteTone(BUZZER_PIN, 0);
}

/* ---------------- SETUP ---------------- */
void setup() {
  Serial.begin(115200);

  SPI.begin();
  mfrc522.PCD_Init();

  ledcAttach(BUZZER_PIN, 2000, 8);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time synchronized");

  Serial.println("Smart Parking System Ready");
  Serial.println("Tap RFID card...");
}

/* ---------------- LOOP ---------------- */
void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;
  if (millis() - lastScanTime < scanDelay) return;

  lastScanTime = millis();

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }

  for (int i = 0; i < USER_COUNT; i++) {
    if (uid == users[i].uid) {

      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Time error");
        return;
      }

      char timeStr[25];
      strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

      String eventType = users[i].isInside ? "EXIT" : "ENTRY";
      users[i].isInside = !users[i].isInside;

      if (eventType == "ENTRY") entryBeep();
      else exitBeep();

      Serial.println("--------------------------------");
      Serial.println("Name  : " + users[i].name);
      Serial.println("Age   : " + String(users[i].age));
      Serial.println("UID   : " + uid);
      Serial.println("Event : " + eventType);
      Serial.println("Time  : " + String(timeStr));
      Serial.println("--------------------------------");

      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverURL);
        http.addHeader("Content-Type", "application/json");

        String payload =
          "{"
          "\"api_key\":\"" + String(API_KEY) + "\","
          "\"uid\":\"" + uid + "\","
          "\"name\":\"" + users[i].name + "\","
          "\"age\":" + String(users[i].age) + ","
          "\"time\":\"" + String(timeStr) + "\","
          "\"event\":\"" + eventType + "\""
          "}";

        int code = http.POST(payload);
        Serial.println("Server response code: " + String(code));
        http.end();
      }

      break;
    }
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
