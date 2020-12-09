#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include "wally.h"

#define DISPLAY_SLOWNESS 200

#define MINUTES 60 * 100
#define TIMER_GAME 5 * MINUTES
#define TIMER_TEMP 2 * MINUTES

#define DHT_PIN 9
#define LED_PIN 11

unsigned long last_change = 0;
bool show_game = true;

Adafruit_NeoPixel pixels(BOARD_SIZE, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(BOARD_WIDTH, BOARD_HEIGHT, LED_PIN,
                                                    NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
                                                        NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                                    NEO_GRB + NEO_KHZ800);

void setup()
{
  pinMode(A0, INPUT);
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  pixels.setBrightness(100);
  pixels.begin();

  matrix->begin();
  add_pattern(PATTERN_ACORNS, 2, 2);
  last_change = millis();
}

void loop()
{
  if (show_game)
  {
    if ((millis() - last_change) > TIMER_GAME)
    {
      show_game = false;
      last_change = millis();
    }
    Serial.println("Displaying Game");
    display_game(&pixels);
  }
  else
  {
    if ((millis() - last_change) > TIMER_TEMP)
    {
      show_game = true;
      last_change = millis();
    }
    Serial.println("Displaying Temp");
    display_temp(matrix, DHT_PIN);
  }
  delay(DISPLAY_SLOWNESS);
}
