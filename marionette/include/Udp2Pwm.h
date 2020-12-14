#include <WiFiUdp.h>
#include "DeviceManager.h"



class Udp2Pwm: public WiFiUDP {
	private:
		unsigned const int packetBytes = 256; // byte count of packet
		int packet[256/4]; // unnecessarily large packet, but it works.
		int port;	// port on which dutyCycle is received.
		DeviceManager* devices; // holds device info, operates on devices


	public:
		Udp2Pwm(int port)
		{
			this->port = port;
			
			// begin(this->port);
  
			devices = new DeviceManager(packet);  // created:
																							// ESC
																							// servo
			// Serial.println("\nprinting devices:");
			devices->print(true);                     // will print:
																								// servo
																								// ESC
		}

		// begins listening on specified port.
		uint8_t begin() {
			return WiFiUDP::begin(this->port);
		}

		// verifies data is RELATIVELY valid, then gives data to `devices`
		void checkAndWrite(bool printEnabled, uint activationTime) {
			int packetSize = parsePacket();	// counts unread bytes
			

			if (read((char*)packet, parsePacket()) && // if there are unread bytes
							millis() > activationTime) { // AND RFID's activation time has expired
				// THEN: read packet.

				// standard packet handling:
					// packet[0] is dutyCycle for ESC
					// packet[1] is dutyCycle for Servo
				// neither should ever be 0
				if (packet[0] == 0 && packet[2] >= 50)
					devices->writeFreq(packet[2]); // if zero, change writeFreq
				else
					devices->write(printEnabled);	// else, write as normal
			} 
			else if (packetSize && printEnabled) // prints info if desired.
				Serial.printf("packet size: %u\n",packetSize);
		}
};