#include "Timer.h"
#include <EEPROM.h>
#include <EtherCard.h>
#include <avr/wdt.h>
#define ethCSpin 10 // put your CS/SS pin here.
#include "Wiegand.h"
void lock(int state);
void heartbeat(void);
int eeAddress = 0;
Wiegand wg( 2, 3);
long card = 0;
int buzzer = 5;
int led = 4;
static byte mymac[] ={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //{ 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };
const char website[] PROGMEM = "123.200.16.26";
char server[] = {123, 200, 16, 26};
byte Ethernet::buffer[700];
Stash stash;
byte session;
Timer locktimer(5000,lock,1);
Timer heartbeattimer(10000,heartbeat,0);
void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(buzzer, 1);
  digitalWrite(led, 1);
  //EEPROM.put(eeAddress, mymac);
  EEPROM.get(eeAddress, mymac);
  for(int i=0; i<6;i=i+1){
    Serial.print(mymac[i],HEX);
    Serial.print(':');
    
  }
  Serial.println();
  initialize_ethernet();

}

void loop() {

  ether.packetLoop(ether.packetReceive());

  if (card = wg.available()) {
    post((String)card);
    Serial.println(card);


  }
  response(ether.tcpReply(session));
  locktimer.elapsed();
  heartbeattimer.elapsed();
}
void right(int pin, int times) {

  for (int i = 0; i <= times; i++) {
    digitalWrite(pin, 0); // sets the digital pin 13 on
    delay(100);            // waits for a second
    digitalWrite(pin, 1);  // sets the digital pin 13 off
    delay(100);
  }

  lock(0); // Open the lock
}
void wrong(int pin) {

  digitalWrite(pin, 0); // sets the digital pin 13 on
  delay(1000);            // waits for a second
  digitalWrite(pin, 1);  // sets the digital pin 13 off
  delay(100);

}
void lock(int state) {
  digitalWrite(led, state);
  locktimer.reset();
}
void initialize_ethernet(void) {
  for (;;) { // keep trying until you succeed
    if (ether.begin(sizeof Ethernet::buffer, mymac, ethCSpin) == 0) {
      Serial.println( "Failed to access Ethernet controller");

    }

    if (!ether.dhcpSetup()) {
      Serial.println("DHCP failed");
      ether.staticSetup (ether.myip, ether.gwip, ether.dnsip, "");
      //continue;
    }


    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GW:  ", ether.gwip);
    ether.printIp("DNS: ", ether.dnsip);

    ether.hisip[0] = server[0];
    ether.hisip[1] = server[1];
    ether.hisip[2] = server[2];
    ether.hisip[3] = server[3];


    ether.printIp("SRV: ", ether.hisip);

    break;
  }
}
void post(String card) {
  Serial.println( "Sending " + card);
  byte sd = stash.create();
  stash.print("card=");
  stash.print(card + "&");
  stash.print("ip=");
  stash.print(String(ether.myip[0]) + "." + String(ether.myip[1]) + "." + String(ether.myip[2]) + "." + String(ether.myip[3]));
  stash.save();
  //prepare send data
  Stash::prepare(PSTR("POST /test/ HTTP/1.0" "\r\n"
                      "Host: $F" "\r\n"
                      "Connection: close" "\r\n"
                      "Content-Type: application/x-www-form-urlencoded" "\r\n"
                      "Content-Length: $D" "\r\n"
                      "\r\n"
                      "$H"),
                 website, stash.size(), sd);
  session = ether.tcpSend();
  int freeCount = stash.freeCount();
  if (freeCount <= 3) {
    Stash::initMap(56);
  }
}
void heartbeat(){
  byte sd = stash.create();
  stash.print("ip=");
  stash.print(String(ether.myip[0]) + "." + String(ether.myip[1]) + "." + String(ether.myip[2]) + "." + String(ether.myip[3]));
  stash.save();
  //prepare send data
  Stash::prepare(PSTR("POST /test/ HTTP/1.0" "\r\n"
                      "Host: $F" "\r\n"
                      "Connection: close" "\r\n"
                      "Content-Type: application/x-www-form-urlencoded" "\r\n"
                      "Content-Length: $D" "\r\n"
                      "\r\n"
                      "$H"),
                 website, stash.size(), sd);
  session = ether.tcpSend();
  int freeCount = stash.freeCount();
  if (freeCount <= 3) {
    Stash::initMap(56);
  }
  heartbeattimer.reset();
}
void response(String reply) {
  if (reply!=0) {
    //res = 0;
    Serial.println(reply);
    Serial.println(F(" >>>REPLY recieved...."));
    
    if (reply.endsWith("HBEAT")) {
      return;
    
    }
    if (reply.endsWith("OK")) {
      Serial.println("Passed.");
      right(buzzer, 2);
    } else {
      Serial.println("Verification failed.");
      wrong(buzzer);
    }
  }
}
void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}
