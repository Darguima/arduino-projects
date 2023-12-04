#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// A4(SDA), A5(SCK)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    notifyErrorWithLED();
  }
  else
  {
    Serial.println(F("SSD1306 allocation success"));
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setRotation(2);
  display.display();

  delayMicroseconds(10);

  draw_each_pixel();
}

void loop()
{
  draw_sliding_text("Welcome to my Arduino Projects! Dont't forget to star my repo!");
  delay(500);
}

void notifyErrorWithLED()
{
  pinMode(LED_BUILTIN, OUTPUT);
  while (1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  };
}

void draw_each_pixel()
{
  display.clearDisplay();

  for (int x = 0; x < SCREEN_WIDTH; x += 4)
  {
    for (int y = 0; y < SCREEN_HEIGHT; y += 4)
    {
      display.drawPixel(x, y, WHITE);
      display.display();
    }
  }

  display.fillScreen(WHITE);
  display.display();
  delay(1000);
}

void draw_sliding_text(char *string)
{
  uint16_t text_w, text_h;
  int cursor_limit;

  display.setTextWrap(false);
  display.getTextBounds(string, 0, 0, NULL, NULL, &text_w, &text_h);
  cursor_limit = -text_w;

  display.clearDisplay();
  display.setCursor(0, (display.height() - text_h) / 2);
  display.print(string);
  display.display();
  delay(1000);

  for (int cursor_x = 0; cursor_x > cursor_limit; cursor_x--) {
    Serial.println(cursor_x);
    display.clearDisplay();
    display.setCursor(cursor_x, (display.height() - text_h) / 2);
    display.print(string);
    display.display();
  }
}