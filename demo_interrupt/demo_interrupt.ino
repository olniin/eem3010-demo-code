/*
 *  Example code
 *
 *  Interrupt game.
 */
#include <inttypes.h>

#define BTN_PLAYER_1 2
#define BTN_PLAYER_2 3
#define LED_PLAYER_1 7
#define LED_PLAYER_2 8
#define LED_GAME 9
#define RESET_GAME 10

// interrupts need some variables stored in RAM
volatile uint8_t p1_w = 0;
volatile uint8_t p2_w = 0;
volatile uint8_t led_state = 0;
volatile uint8_t game_state = 1;


void setup()
{
  Serial.begin(115200);
  pinMode(BTN_PLAYER_1, INPUT_PULLUP);
  pinMode(BTN_PLAYER_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_PLAYER_1), p1_win, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BTN_PLAYER_2), p2_win, CHANGE);
  pinMode(LED_PLAYER_1, OUTPUT);
  pinMode(LED_PLAYER_2, OUTPUT);
  pinMode(LED_GAME, OUTPUT);
}


void loop()
{
  static uint8_t game_over = 0;
  static uint8_t game_start = 0;
  if (!game_start) {
    game_start = 1;
    long wait = random(500, 4000);
    start();
    delay(wait);
    digitalWrite(LED_GAME, 1);
    led_state = 1;
  }
  if (!game_state && !game_over) {
    Serial.print("Winner: ");
    if (p1_w) {
      Serial.println("BLUE");
      digitalWrite(LED_PLAYER_1, 1);
    }
    if (p2_w) {
      Serial.println("RED");
      digitalWrite(LED_PLAYER_2, 1);
    }
    game_over = 1;
  }
}

/*
 *  Interrupt routine 1.
 *
 *  Activates if player 1 presses the button to trigger an interrupt.
 */
void p1_win(void)
{
  if (game_state) {
    if (!p2_w && led_state) {
      p1_w = 1;
    } else {
      p2_w = 1;
    }
    game_state = 0;
  }
}

/*
 *  Interrupt routine 2.
 *
 *  Activates if player 2 presses the button to trigger an interrupt.
 */
void p2_win(void)
{
  if (game_state) {
    if (!p1_w && led_state) {
      p2_w = 1;
    } else {
      p1_w = 1;
    }
    game_state = 0;
  }
}

/*
 *  Blink LEDs at the start of the game.
 *
 *  Blinks all LEDs 3 times to signal that a game has started.
 */
void start(void)
{
  for (uint8_t i=0; i<3; i++) {
    digitalWrite(LED_GAME, 1);
    digitalWrite(LED_PLAYER_1, 1);
    digitalWrite(LED_PLAYER_2, 1);
    delay(400);
    digitalWrite(LED_GAME, 0);
    digitalWrite(LED_PLAYER_1, 0);
    digitalWrite(LED_PLAYER_2, 0);
    delay(400);
  }
}
