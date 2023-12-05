#define ANALOG_PIN A0

// Based on this post: https://circuits4you.com/2016/05/13/arduino-resistance-measurement/

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Voltage read by the Analog Port
  // This is the voltage across the fix resistor (1k Ohm)
  float sensor_voltage = analogRead(ANALOG_PIN) * (5.0 / 1024.0);

  // Current flowing through the circuit: I = V / R
  float current_flowing = sensor_voltage / 1000; // 1k resistor

  // Voltage across the variable resistor: V = 5 - Vr
  float resistor_voltage = 5 - sensor_voltage;

  // R = V / I
  float resistance_calculated = resistor_voltage / current_flowing;

  Serial.println("Voltage: " + String(sensor_voltage) + "V");
  Serial.println("Current: " + String(current_flowing * 1000000) + "uA");
  Serial.println("Resistance: " + String(resistance_calculated) + " Ohm");
  Serial.println(" ");

  delay(1000);
}