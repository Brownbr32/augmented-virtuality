#include <Udp2Pwm.h>
#include <myOta.h>
#include <RFID.h>
#include "LocalNetwork.h"



MyOTA* otaServer;     // used to program the arduino "Over The Air"
                        // via UDP ( I believe )
                        //
                      // Inherits from ArduinoOTA and adds
                        // system-specific defaults
Udp2Pwm* Udp;         // takes UDP messages and writes values to
                        // devices
                      // Inherits from WiFiUDP (an arduino 
                        // Library)
RFID* bananaDetector; // reads from RFID chip and determines
                        // action to be taken.
                      // Inherits from SPI and MFRC522
                        // adopted from The Circuit tutorial

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // initiates serial communication
                          // with baud_rate=115200
                        // used in last-resort debugging
  Serial.println("Booting"); // last-resort debugging
  delay(1000); // delay 1 second to allow for serial connection



  LocalNetwork localNetwork; // contains network-specific data
      // such as hostname, network name, passphrase, port number, etc.
  
  Udp = new Udp2Pwm(localNetwork.port);
      // takes network information and creates UDP->pwm object
  otaServer = new MyOTA();
      // this allows for OTA programming
          // Over The Air programming
  bananaDetector = new RFID();
  Udp->begin(); // uses inherited begin(port) from WiFiUDP.
    // inserts localNetwork.port from above.
    // UDP begins listening on specified port.
  
  localNetwork.connect(); // connects to the internet using
    // network name and passphrase.
    // tries 3 times, then dies. May try to reboot. Suggested
    // power cycle usint rst button.

  Serial.print("send datagrams to: "); // more serial last-resort
    // debugging
  Serial.printf("%s: %s",otaServer->getHostname().c_str(), 
      localNetwork.ip_port.c_str()); // yet more of it
  Serial.println("\n\n"); // now prettify me!!!
}

uint waitTime = 0; // this is used to ignore commands sent for
  // bananaDetector-specified time when banana == detected

void loop() {
  otaServer->handle(); // this allows for OTA programming by 
    // listening for reprogramming requests
  waitTime = bananaDetector->checkMe(true /*enables print*/); 
    // checks for a banana
  Udp->checkAndWrite(true /*enables print*/, waitTime);
    // checks for UDP packet, writes values received to:
      // ESC
      // Servo
  waitTime = 0; // resets waitTime. Why? Probably so it 
      // isn't accidentally enabled forever...
}
