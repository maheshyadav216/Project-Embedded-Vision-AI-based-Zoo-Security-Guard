//=================================================================================================================//
// Project/Tutorial       - DIY Project - Embedded Vision AI based Zoo Security Guard
// Author                 - https://www.hackster.io/maheshyadav216
// Hardware               - Grove Vision AI V2,  XIAO ESP32S3, Buzzer        
// Sensors                - NA
// Software               - Arduino IDE
// GitHub Repo of Project - https://github.com/maheshyadav216/Project-Embedded-Vision-AI-based-Zoo-Security-Guard
// Code last Modified on  - 27/06/2024
//=================================================================================================================//

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Seeed_Arduino_SSCMA.h>

SSCMA AI;

//Replace with your credentials
const char* ssid     = "";
const char* password = "";

// Telegram Chat ID and Bot Token
#define CHAT_ID ""
#define BOTtoken ""  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Message Send Interval Settings
const unsigned long sendInterval = 30000;
unsigned long previousTime = 0;


void setup()
{
  AI.begin();
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
}

void loop()
{
  String message = "Zoo Danger Area Security Guard : \n";
  message += "\nAlert: Human presence detected!! \nPlease Check the area.\n";
  unsigned long currentTime = millis();

  if (!AI.invoke())
  {
    Serial.println("invoke success");
    Serial.print("perf: prepocess=");
    Serial.print(AI.perf().prepocess);
    Serial.print(", inference=");
    Serial.print(AI.perf().inference);
    Serial.print(", postpocess=");
    Serial.println(AI.perf().postprocess);

    for (int i = 0; i < AI.boxes().size(); i++)
    {
      Serial.print("Box[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.boxes()[i].target);
      Serial.print(", score=");
      Serial.print(AI.boxes()[i].score);
      Serial.print(", x=");
      Serial.print(AI.boxes()[i].x);
      Serial.print(", y=");
      Serial.print(AI.boxes()[i].y);
      Serial.print(", w=");
      Serial.print(AI.boxes()[i].w);
      Serial.print(", h=");
      Serial.println(AI.boxes()[i].h);
    }
    for (int i = 0; i < AI.classes().size(); i++)
    {
      Serial.print("Class[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.classes()[i].target);
      if (AI.classes()[i].target == 1 && AI.classes()[i].score > 80 ) {
        if (currentTime - previousTime >= sendInterval) {
          bot.sendMessage(CHAT_ID, message, "");
          previousTime = currentTime;
        }
      }
      Serial.print(", score=");
      Serial.println(AI.classes()[i].score);
    }
    for (int i = 0; i < AI.points().size(); i++)
    {
      Serial.print("Point[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.points()[i].target);
      Serial.print(", score=");
      Serial.print(AI.points()[i].score);
      Serial.print(", x=");
      Serial.print(AI.points()[i].x);
      Serial.print(", y=");
      Serial.println(AI.points()[i].y);
    }
  }
}

//=================================================================================================================//