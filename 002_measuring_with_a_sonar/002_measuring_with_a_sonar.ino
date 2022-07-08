#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// A4(SDA), A5(SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

# define triggerPin 6
# define echoPin 7

void setup() {
  Serial.begin(9600);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(triggerPin, LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    notifyErrorWithLED();
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.setTextSize(2);
  display.setRotation(2);
  display.display();

  delayMicroseconds(10);
}

void loop() {
  long duration, centimeters;

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  centimeters = microsecondsToCentimeters(duration);

  Serial.print(centimeters);
  Serial.println(" centimeters");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(centimeters);
  display.print("centimeters");
  display.display();
  
  delay(100);
}

long microsecondsToCentimeters(long  microseconds) {
  // Speed of sound (20 cº) = 343 m/s = 0.343 m/ms = 0.000343 m/μs = 0.0343 cm / ms
  return (microseconds / 2) * 0.0343;
} 

void notifyErrorWithLED () {
  pinMode(LED_BUILTIN, OUTPUT);
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(75);
    digitalWrite(LED_BUILTIN, LOW);
  };
}
