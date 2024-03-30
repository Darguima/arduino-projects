// Inspired from https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/

#include "WiFi.h"

#define WIFI_SSID "Your SSID"
#define WIFI_PASSWORD "Your Password"

#define SONAR_TRIGGER_PIN 14
#define SONAR_ECHO_PIN 12
#define SOUND_SPEED 0.034
#define DOOR_OPEN_DISTANCE_CM 33

#define TIMEOUT_TIME 2000

WiFiServer server(80);

#include "pages_h/landing.h"
const char *LANDING_PAGE_HTML = reinterpret_cast<const char *>(pages_landing_html);

#include "pages_h/404.h"
const char *PAGE_404_HTML = reinterpret_cast<const char *>(pages_404_html);

void setup()
{
  Serial.begin(115200);

  pinMode(SONAR_ECHO_PIN, INPUT);     // Sets the echoPin as an Input
  pinMode(SONAR_TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output

  setup_wifi_connection();

  server.begin();
}

void setup_wifi_connection()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  int available_wifi_qnt = WiFi.scanNetworks();
  Serial.printf("Available Networks (%d):\n", available_wifi_qnt);

  for (int i = 0; i < available_wifi_qnt; ++i)
  {
    Serial.printf("\t%d. %s (%d)%s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "" : "*");
  }

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  Serial.printf("[LOG] Trying to connect to %s\n", WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setHostname("open-door-detector");
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("[LOG] Connected successfully");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.print("[LOG] IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("[LOG] Disconnected");
  Serial.printf("[ERROR] WiFi disconnection reason: %d\n", info.wifi_sta_disconnected.reason);
  Serial.println("[LOG] Trying to Reconnect");

  WiFi.reconnect();
}

WiFiClient client;
unsigned long previous_time, current_time;
String current_line, header;

void loop()
{
  client = server.available();

  if (!client)
    return;

  Serial.println("[LOG] New client");

  current_time = millis();
  previous_time = current_time;

  current_line = "";
  header = "";

  while (client.connected() && current_time - previous_time <= TIMEOUT_TIME)
  {
    current_time = millis();

    if (!client.available())
      continue;

    char c = client.read();
    header += c;

    if (c == '\n')
    {
      if (current_line.length() == 0)
      {
        if (header.indexOf("GET / ") >= 0)
        {
          Serial.println("[LOG] Responding root page");
          server_response(client, 200, "text/html", LANDING_PAGE_HTML);
        }
        else if (header.indexOf("GET /is_the_door_open") >= 0)
        {
          // This should be made with Server Side Events - but I don't want to include a library for that
          Serial.println("[LOG] Responding sensor values");
          server_response(client, 200, "text/plain", is_the_door_open() ? "1" : "0");
        }
        else
        {
          Serial.println("[LOG] Responding 404 page");
          server_response(client, 404, "text/html", PAGE_404_HTML);
        }

        break;
      }
      else
      {
        current_line = "";
      }
    }
    else if (c != '\r')
    {
      current_line += c;
    }
  }

  client.stop();
  Serial.println("[LOG] Client disconnected\n");
}

void server_response(WiFiClient client, int code, String content_type, String content)
{
  client.println("HTTP/1.1 " + String(code) + " OK");

  client.println("Content-type:" + content_type);
  client.println("Connection: close");
  client.println("");
  client.println(content);
  client.println("");
}

long duration;
float distanceCm;

bool is_the_door_open(){
  // Clears the trigPin
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(SONAR_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIGGER_PIN, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(SONAR_ECHO_PIN, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;
  Serial.printf("[LOG] Distance: %f cm\n", distanceCm);

  return distanceCm <= DOOR_OPEN_DISTANCE_CM;
}
