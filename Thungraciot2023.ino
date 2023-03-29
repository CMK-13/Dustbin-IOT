#include "Function.h"

Dustbin Dustbin;

void setup()
{
  Dustbin.Initialization_Pin();
}

void loop() 
{
  Dustbin.DustbinRun();
  Dustbin.ThingSpeakRun();
  delay(2000);
  Dustbin.BlynkRun();
}