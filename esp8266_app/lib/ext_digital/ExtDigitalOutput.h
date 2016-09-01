#ifndef ExtDigitalOutput_h
#define ExtDigitalOutput_h
#include "Arduino.h"

class ExtDigitalOutput
{

  public:
		ExtDigitalOutput(byte dataPin, byte latchPin , byte clockPin, byte series);
		void write(byte pin,boolean value);
    void begin();
  private:
    byte _dataPin;
    byte _latchPin;
    byte _clockPin;
    byte _pin;
    byte _value;
    unsigned int _data;
    byte _series;

    void shiftRegister(unsigned int data);
};

#endif
