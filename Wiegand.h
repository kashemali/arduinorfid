// Copyright  : Dennis Buis (2017)
// License    : MIT
// Platform   : Arduino
// Library    : Simple Wiegand Library for Arduino
// File       : Wiegand.h
// Purpose    : Receiving IDs from a Wiegand compatible device (reader / keypad)
// Repository : https://github.com/DennisB66/Simple-Wiegand-Library-for-Arduino

#ifndef _WIEGAND_H
#define _WIEGAND_H
class Wiegand {
    
  public:
    Wiegand( int =2, int =3);     // initialize Wiegand device defining pins for line D0 / D1

    void begin();                                             // initializes the Wiegand device connection (line D0 / D1)
    void reset();                                             // reset active tag / key values
    bool hasDevice();                                         // checks is a Wiegand device is connected to pinD0 / pinD1
    unsigned long available();                                         // checks if a new Wiegand ID has been received
    static volatile unsigned long card;
    static volatile unsigned long _card;
    static volatile byte _available;
    unsigned long Card();                                  // returns the last active Wiegand code
      int _D0;                                               // digital pin for reading line D0
  int _D1;
  static void _pulseD0();                                   // process bits coming on line D0
  static void _pulseD1();  // convert keypad based data to key code
  static void _writeDx( volatile byte);
  static volatile int recvBitCount; 
  
  volatile byte isCardReadOver;
private:


};

#endif
