# arduinorfid
#include "Wiegand.h"
Wiegand wg( 2, 3); 
long card=0;
void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  
}

void loop() {
  
  if(card=wg.available()){
   
    Serial.println(card);

    
  }


}
