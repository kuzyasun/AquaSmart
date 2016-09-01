/*
 ExtDigitalOutput.cpp
 Extend Digital Output using 74HC595 for arduino or ESP8266
 Library originally added 18 Oktober 2015
 by Edwin NIcholas
 */
#include "Arduino.h"
#include "ExtDigitalOutput.h"

ExtDigitalOutput::ExtDigitalOutput(byte dataPin, byte latchPin , byte clockPin,byte series)
{
  _dataPin  = dataPin;
  _latchPin = latchPin;
  _clockPin = clockPin;
  _data=0;
  _series=series;
}

void ExtDigitalOutput::write(byte pin,boolean value)
{
  _pin=pin;
  _value=value;
  digitalWrite(_latchPin,LOW);

  for(int c=0;c<_series;c++)
  {
    bitWrite(_data,_pin,_value);
   //shiftOut(_dataPin,_clockPin, MSBFIRST,_data);
    shiftRegister(_data);
  }
  digitalWrite(_latchPin,HIGH);
}

void ExtDigitalOutput::shiftRegister(unsigned int data)
{
  for(int i=(7*_series);i>=0;i--)
    {
      digitalWrite(_clockPin,LOW);

      digitalWrite(_dataPin,bitRead(data,i));

      digitalWrite(_clockPin,HIGH);
    }
}

void ExtDigitalOutput::begin()
{
  pinMode(_dataPin,OUTPUT);
  pinMode(_latchPin,OUTPUT);
  pinMode(_clockPin,OUTPUT);
  digitalWrite(_latchPin,LOW);
  //shiftOut(_dataPin,_clockPin, MSBFIRST,0);
  shiftRegister(0);
  digitalWrite(_latchPin,HIGH);
}
