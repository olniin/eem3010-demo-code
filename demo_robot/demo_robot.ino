/*
 *  Example code
 *
 *  Robot platform test.
 */
#include <inttypes.h>
#include <Servo.h>

#define SERVO_PIN 2

#define RIGHT_DIR 3
#define LEFT_DIR 4
#define RIGHT_MOTOR 5
#define LEFT_MOTOR 6

#define US_TRIG 7
#define US_ECHO 8

#define BUZZER A0

#define LINE_L A1
#define LINE_M A2
#define LINE_R A3

#define LED_SDA A4
#define LED_SCL A5

Servo servo;


void setup()
{
  uint8_t outputs[8] = {RIGHT_DIR, LEFT_DIR, RIGHT_MOTOR, LEFT_MOTOR, US_TRIG,
    BUZZER, LED_SDA, LED_SCL};
  uint8_t inputs[4] = {US_ECHO, LINE_L, LINE_M, LINE_R};
  for (uint8_t i=0; i<8; i++) {
    pinMode(outputs[i], 1);
  }
  for (uint8_t i=0; i<4; i++) {
    pinMode(inputs[i], 0);
  }
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
}


void loop()
{
  static uint16_t distance;
  static uint8_t object_detected, line_data, angle, speed;
  distance = get_sensor_distance(US_TRIG, US_ECHO);
  object_detected = (distance <= 20) ? 1 : 0;
  line_data = get_line_sensor_data(LINE_L, LINE_M, LINE_R);
  delay(100);
  digitalWrite(BUZZER, object_detected);
  angle = 180 / 7.0 * line_data;
  servo.write(angle);
  speed = 255 / 7.0 * line_data;
  run_motors(RIGHT_DIR, object_detected, RIGHT_MOTOR, speed);
  run_motors(LEFT_DIR, object_detected, LEFT_MOTOR, speed);
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

/*
 *  Return line sensor data.
 *
 *  Returns line sensors' readings as one uint8_t (three bits of data from
 *  the LSB).
 *
 *  @param line_l - left line sensor pin
 *  @param line_m - middle line sensor pin
 *  @param line_r - right line sensor pin
 */
uint8_t get_line_sensor_data(uint8_t line_l, uint8_t line_m, uint8_t line_r)
{
  uint8_t sensor_data = 0, ld, md, rd;
  ld = digitalRead(line_l);
  md = digitalRead(line_m);
  rd = digitalRead(line_r);
  sensor_data = ((ld^1) << 2) + ((md^1) << 1) + (rd^1);
  return sensor_data;
}

/*
 *  Run one side's motors.
 *
 *  @param dir_pin - motor direction pin
 *  @param dir - desired motor direction (0 | 1)
 *  @param motor_pin - motor PWM pin
 *  @param speed - motor speed (0-255)
 */
void run_motors(uint8_t dir_pin, uint8_t dir, uint8_t motor_pin, uint8_t speed)
{
  digitalWrite(dir_pin, dir);
  analogWrite(motor_pin, speed);
}
