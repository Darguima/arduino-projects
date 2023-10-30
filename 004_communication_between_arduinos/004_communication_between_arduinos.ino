#define LED 7
#define NET_OUT 8
#define NET_IN 9

void setup() {
  pinMode(LED, OUTPUT);

  pinMode(NET_OUT, OUTPUT);
  pinMode(NET_IN, INPUT);
}

void loop() {
  if (!digitalRead(NET_IN)) {
    digitalWrite(LED, HIGH);
    digitalWrite(NET_OUT, HIGH);
    delay(500);

    digitalWrite(LED, LOW);
    digitalWrite(NET_OUT, LOW);
    delay(500);
  }
}
