#ifndef _WALLY_H__
#define _WALLY_H__

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <SDHT.h>
#include "gameoflife.h"

#define TEMP_BLUE 0
#define TEMP_RED 40

int lock_counter = 0;
short temp = 0;
short remainder = 0;
SDHT dht;

void display_temp(Adafruit_NeoMatrix *mat, int pin)
{

  if (dht.read(DHT11, pin)){
    temp = double(dht.celsius) / 10;
    remainder = dht.celsius % 10;
  }

  temp = 25;
  remainder = 3;


  Serial.print("   Celsius => ");
  Serial.print(temp);
  Serial.print(".");
  Serial.println(remainder);

  mat->clear();
  mat->setTextWrap(false);
  int red_val = map(temp, TEMP_BLUE, TEMP_RED, 0, 31);
  int blu_val = map(temp, TEMP_BLUE, TEMP_RED, 31, 0);
  mat->setTextColor((red_val << 11) + blu_val);

  mat->setCursor(0, 0);
  mat->print(temp / 10);
  mat->setCursor(5, 3);
  mat->print(temp % 10);

  mat->drawLine(9, 0, 8, 0, 31 << 5); //green 'C'
  mat->drawLine(8, 2, 8, 0, 31 << 5); //green 'C'
  mat->drawLine(8, 2, 9, 2, 31 << 5); //read MatrixGFXDemo.ino
  mat->fillRect(5, 0, 2, 2, (16 << 11 | 16 << 5));
  mat->fillRect(0,9,remainder, 1, 0xFFFF);
  mat->show();
};

void display_game(Adafruit_NeoPixel *pixel_ptr)
{

  if (lock_counter > 20 || count_cells() == 0)
  {
    // 10 iterations of presumidly the same pattern, time to bail.
    randomize_board();
    Serial.println("have to reset");
    lock_counter = 0;
  }

  //display the board.

  for (int i = 0; i < BOARD_SIZE; i++)
  {
    //every second row is reversed, so we must account for that.
    int pixel_index = (i / BOARD_WIDTH) % 2 ? ((i / BOARD_WIDTH) * BOARD_WIDTH) + (BOARD_WIDTH - (i % BOARD_WIDTH)) - 1 : i;

    if (gameboard[i] & 1)
    {
      pixel_ptr->setPixelColor(pixel_index, pixel_ptr->gamma32(pixel_ptr->ColorHSV(random() % 65535L)));
    }
    else
    { //explicitly turn things off to blank out previous matrix
      pixel_ptr->setPixelColor(pixel_index, pixel_ptr->Color(0, 0, 0));
    }
  }

  pixel_ptr->show();

  if (process_board() == 0)
  {
    //no net change, increment counter
    lock_counter++;
    Serial.print("No Change ");
    for (int i = 0; i < lock_counter; i++)
    {
      Serial.print("!");
    }
    Serial.println();
  }
  else
  {
    lock_counter = 0;
  }
  next_generation();
}

#endif
