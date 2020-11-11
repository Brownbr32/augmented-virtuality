#include <Udp2Pwm.h>
#include <myOta.h>
#include <RFID.h>
#include "LocalNetwork.h"



MyOTA* otaServer;
Udp2Pwm* Udp;
RFID* bananaDetector;

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
