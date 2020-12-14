#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>




const int netSize = 3;
struct LocalNetwork {
    String name = "pi_camera";      // name of network
    String pass = "seniorproject";  // network password
    String hostname = "rc-module";  // hostname, enables automated
      // node location
    unsigned int port = 4210;       // UDP port. Listens for two
      // ints and writes them to:
        // ESC
        // Servo
    String ip_port;   // ip address with port in form:
      // x.x.x.x:4210 (port is defined above)
		IPAddress piCam;  // gets ipAddress of piCam 
      // Discovered upon hostname resolution below

    // takes hard-coded information above and attempts to connect
      // to Raspberry Pi 0w's network.
    bool connect() {
      
      Serial.print("\nConfiguring access point\n"); // serial debug
      
      // attempts connection 3 times, OR until connection is secured
        // breaks when one of the two events occurs.
      for (int i = 0; i< netSize && (WiFi.status() != WL_CONNECTED); i++)
      {

        WiFi.mode(WIFI_STA);
        WiFi.begin(this->name, this->pass);
      
        Serial.printf("Connecting to %s",name.c_str());
        int initMillis = millis();
        while(WiFi.status() != WL_CONNECTED && millis()-initMillis<7500)
        {
          delay(250);
          Serial.print(".");
        }
        if (WiFi.status() != WL_CONNECTED)
          Serial.print("Failed.");

        Serial.println();
      }
      // tests for connection
      if (WiFi.status() != WL_CONNECTED)
        Serial.println("Connection failed. Sorry. :/"); // debugging
      // may add auto-reboot above. May not, as well.
      WiFi.hostname(hostname); // sets hostname to String `hostname`.
			WiFi.hostByName("pi.cam",piCam);  // searches network for hostname:
        // 'pi.cam' and saves the IP to `piCam`

      
      ip_port = WiFi.localIP().toString() + ":" + port; // saves IP
        // address and port in format:
          // x.x.x.x:udp_port, such as:
            // 192.168.4.7:4210
      Serial.printf("Connected!\nNetwork: %s\nIP:\t%s\n", 
          name.c_str(), ip_port.c_str());   // used for debugging purposes
      
      return true;
    }
};
