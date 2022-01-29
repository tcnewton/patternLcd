/* Main.ino file generated by New Project wizard
 *
 * Created:   dom jan 16 2022
 * Processor: ATmega328P
 * Compiler:  Arduino AVR (Proteus)
 */
#include <Arduino.h>
#include "patternlcd.h"

// flag button
bool flag_bttfwd = false;
bool flag_bttent = false;
bool flag_bttrwd = false;
bool pressFwd = false;
bool pressEnt = false;
bool pressRwd = false;

bool debounceButton(uint8_t button, bool &btt_flag)
{
  if (!button)
    btt_flag = true;
  if (button && btt_flag)
  {
    btt_flag = false;
    return true;
  }
  return false;
}

void setup()
{

  Serial.begin(9600);
  pinMode(btt_fwd, INPUT_PULLUP);
  pinMode(btt_ent, INPUT_PULLUP);
  pinMode(btt_rwd, INPUT_PULLUP);
  // declare variables of struct
  lcd.begin(16, 2);
  printStructInt();
  setTokenPosition();
}

void loop()
{
  pressFwd = debounceButton(digitalRead(btt_fwd), flag_bttfwd);
  pressEnt = debounceButton(digitalRead(btt_ent), flag_bttent);
  pressRwd = debounceButton(digitalRead(btt_rwd), flag_bttrwd);

  if (pressEnt)
  {
    changeTokenPos();
  }

  if (pressFwd)
  {
    incStructValue();
  }

  if (pressRwd)
  {
    decStructValue();
  }
}