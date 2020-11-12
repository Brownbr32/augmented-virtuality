#include <WiFiUdp.h>
#include "DeviceManager.h"



class Udp2Pwm: public WiFiUDP {
	private:
		unsigned const int packetBytes = 256;
		int packet[256/4];
		int port;
		DeviceManager* devices;
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

		uint8_t begin() {
			return WiFiUDP::begin(this->port);
		}

		void checkAndWrite(bool printEnabled, uint activationTime) {
			int packetSize = parsePacket();
			
			if (read((char*)packet, parsePacket()) && millis() > activationTime) {
				// packet handling
				if (packet[0] == 0 && packet[2] >= 50)
					devices->writeFreq(packet[2]);
				else
					devices->write(printEnabled);
			} else if (packetSize && printEnabled) Serial.printf("packet size: %u\n",packetSize);
		}
};