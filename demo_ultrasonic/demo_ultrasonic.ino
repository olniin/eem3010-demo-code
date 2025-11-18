/*
 *  Example code
 *
 *  Sounding a buzzer when the sensor detects something.
 */
#include <inttypes.h>

#define SONIC_TRIG_PIN 2
#define SONIC_ECHO_PIN 3

#define BUZZER_PIN 4


void setup()
{
  Serial.begin(115200);
  pinMode(SONIC_TRIG_PIN, 1);
  pinMode(SONIC_ECHO_PIN, 0);
  pinMode(BUZZER_PIN, 1);
}


void loop()
{
  static uint16_t distance = 0;
  distance = get_sensor_distance(SONIC_TRIG_PIN, SONIC_ECHO_PIN);
  delay(100);
  Serial.print("Distance: ");
  Serial.println(distance);
  (distance < 20) ? digitalWrite(BUZZER_PIN, 1) : digitalWrite(BUZZER_PIN, 0);
}

/*
 *  Return ultrasonic sensor distance.
 *
 *  @param trig_pin - ultrasonic sensor's trig pin
 *  @param echo_pin - ultrasonic sensor's echo pin
 */
uint16_t get_sensor_distance(uint8_t trig_pin, uint8_t echo_pin)
{
  uint16_t distance = 0;
  digitalWrite(trig_pin, 1);
  delayMicroseconds(10);
  digitalWrite(trig_pin, 0);
  distance = pulseIn(echo_pin, 1) / 58.8;
  return distance;
}
