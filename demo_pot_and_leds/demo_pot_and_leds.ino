/*
 *  Example code
 *
 *  Controlling LEDs with one potentiometer.
 */
#include <inttypes.h>

#define LED_PIN_1 2
#define LED_PIN_2 3
#define LED_PIN_3 4
#define LED_PIN_4 5
#define LED_PIN_5 6

#define PWM_LED_PIN 9

#define ANALOG_INPUT A0
#define ADC_RES 10
#define ADC_REF 5.0

const uint8_t led_array[5] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4, LED_PIN_5};


void setup()
{
  Serial.begin(115200);
  for (uint8_t i=0; i<5; i++) {
    pinMode(led_array[i], 1);
  }
  pinMode(PWM_LED_PIN, 1);
}


void loop()
{
  static float voltage = 0;
  voltage = get_voltage(ANALOG_INPUT, ADC_RES, ADC_REF);
  Serial.print("Voltage: ");
  Serial.println(voltage, 4);
  glow_led_array(led_array, voltage);
  glow_pwm_led(PWM_LED_PIN, voltage, ADC_REF);
}

/*
 *  Read analog pin and return voltage.
 *
 *  Reads the given analog pin, calculates the voltage on the pin and returns
 *  it as a float.
 *
 *  @param analog_pin - the analog pin to be read
 *  @param adc_res - ADC resolution in bits
 *  @param adc_ref - ADC reference voltage
 */
float get_voltage(uint8_t analog_pin, uint8_t adc_res, float adc_ref)
{
  float voltage = 0;
  int reading = analogRead(analog_pin);
  voltage = adc_ref * reading / ((1 << adc_res) - 1);
  return voltage;
}

/*
 *  Glow 5 LEDS.
 *
 *  Turn 5 LEDS on/off based on input voltage.
 *
 *  @param led_array[5] - array of 5 LED pins
 *  @param voltage - input voltage
 */
void glow_led_array(uint8_t led_array[5], float voltage)
{
  for (uint8_t i=0; i<5; i++) {
    (voltage >= (i + 1)) ? digitalWrite(led_array[i], 1) : digitalWrite(led_array[i], 0);
  }
}

/*
 *  Glow a LED using PWM.
 *
 *  @param pwm_led - pwm controllable LED pin
 *  @param voltage - input voltage
 *  @param max_voltage - ADC reference voltage
 */
void glow_pwm_led(uint8_t pwm_led, float voltage, float max_voltage)
{
  int pwm_value = voltage / max_voltage * 255;
  analogWrite(pwm_led, pwm_value);
}
