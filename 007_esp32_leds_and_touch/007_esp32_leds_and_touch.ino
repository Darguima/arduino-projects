#define TOUCH_PIN 4
#define LED_PIN 16
#define TRESHOLD 40

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

touch_value_t touch;

void loop() {
  touch = touchRead(TOUCH_PIN);
  Serial.println(touch);

  if (touch < TRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(10);
}
