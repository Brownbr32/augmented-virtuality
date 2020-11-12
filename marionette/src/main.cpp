#include <Udp2Pwm.h>
#include <myOta.h>
#include <RFID.h>
#include "LocalNetwork.h"



MyOTA* otaServer;
Udp2Pwm* Udp;
RFID* bananaDetector;

/* TCP
 *  1. hey, um, you there?
 *  2. yep! I'm here
 *  1. can I send stuff to you?
 *  2. sure!
 *  1. okay! I'm gonna send it to port 234
 *  2. ok. send it.
 *  1. send
 *  2. ok I got it
 *  1. what did you get
 *  2. i got this...
 *  1. i don't think that's what i sent. let's go back to step 1
 * 
 * UDP
 *  1. send.
*/ 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Booting");
  delay(1000);



  LocalNetwork localNetwork;
  
  Udp = new Udp2Pwm(localNetwork.port);
  otaServer = new MyOTA();
  bananaDetector = new RFID();
  Udp->begin();
  
  localNetwork.connect();

  Serial.print("send datagrams to: ");
  Serial.printf("%s: %s",otaServer->getHostname().c_str(), localNetwork.ip_port.c_str());
  Serial.println("\n\n");
}

uint waitTime = 0;

void loop() {
  otaServer->handle();
  waitTime = bananaDetector->checkMe(true);
  Udp->checkAndWrite(true/*enables print*/, waitTime);
  waitTime = 0;
}
