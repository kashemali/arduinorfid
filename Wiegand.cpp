

// Copyright  : Dennis Buis (2017)
// License    : MIT
// Platform   : Arduino
// Library    : Simple Wiegand Library for Arduino
// File       : Wiegand.cpp
// Purpose    : Receiving IDs from a Wiegand compatible device (reader / keypad)
// Repository : https://github.com/DennisB66/Simple-Wiegand-Library-for-Arduino

#include "Arduino.h"
#include "Wiegand.h"
 volatile int Wiegand::recvBitCount = 0;
 volatile unsigned long Wiegand::card = 0;
 volatile byte Wiegand::_available=0;
 volatile unsigned long Wiegand::_card = 0;
Wiegand::Wiegand( int D0, int D1)
{
  // prepare data buffer

  pinMode(_D0 = D0, INPUT);                        // Set pin used for line D0 as input
  pinMode(_D1 = D1, INPUT);                        // Set pin used for line D1 as input
  card = 0;
  recvBitCount = 0;
  isCardReadOver = 0;
  _available=0;
  begin();
}
void Wiegand::begin()
{
  // disable interupts while attaching
  if(_D0==2 && _D1==3){
  attachInterrupt(0, _pulseD0, FALLING );  //data0/tx is connected to pin 2, which results in INT 0
  attachInterrupt(1, _pulseD1, FALLING );  //data1/rx is connected to pin 3, which results in INT 1
  }else{
    attachInterrupt(digitalPinToInterrupt(_D0), _pulseD0, FALLING );  //data0/tx is connected to pin 2, which results in INT 0
    attachInterrupt(digitalPinToInterrupt(_D1), _pulseD1, FALLING );  //data1/rx is connected to pin 3, which results in INT 1
  }

}
void Wiegand::_pulseD0() {
  recvBitCount++;
  
    _writeDx( 0x00);
    
 
}

// handle interrupt1
void Wiegand::_pulseD1() {
  recvBitCount++;
  
    _writeDx( 0x01);
    
  
}
void Wiegand::_writeDx( byte bit)
{
  if ( recvBitCount >= 26) {
    _available = 1;
     return ;
  }; 
  if(recvBitCount>1){                           
  card <<= 1;                                              // shift lo bits left, new lo bit 0 = 0
  card |= ( bit & 0x01);
  }
  
   // add new bit 0 to lo bit 0
}

// reset active tag / key values
void Wiegand::reset()
{
  _available = 0;   
  _card=card;
  card = 0;
  recvBitCount = 0;
}

// checks is a Wiegand device is connected to pinD0 / pinD1
bool Wiegand::hasDevice()
{ // return true = Wiegand device connected
  return ( digitalRead( _D0) == HIGH) && ( digitalRead( _D0) == HIGH);
}

unsigned long Wiegand::available() {
  if(_available){
    reset();
    return _card;
  }else{
    return 0;
  }
}
