#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>




const int netSize = 3;
struct LocalNetwork {
    // String names[netSize] = {"N.U.T.S.2", "The Anti-Nexus", "Mr Brown Can Moo"};
    // String passes[netSize] = {"BestD@S0Frrr", "Dape+Vape", "C@n y0u?"};
    // String pass = "BestD@S0Frrr";
    String name = "pi_camera";
    String pass = "seniorproject";
  //  String name = "Mr Brown Can Moo";
  //  String pass = "C@n y0u?";
    String hostname = "rc-module";
    String ip_port;
		IPAddress piCam;
    unsigned int port = 4210;
    bool connect() {
      
      Serial.print("\nConfiguring access point\n");
      int i;
      for (i = 0; i< netSize && (WiFi.status() != WL_CONNECTED); i++)
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
      if (WiFi.status() != WL_CONNECTED)
        Serial.println("Connection failed. Sorry. :/");
      WiFi.hostname(hostname);
			WiFi.hostByName("pi.cam",piCam);

      
      ip_port = WiFi.localIP().toString() + ":" + port;
      Serial.printf("Connected!\nNetwork: %s\nIP:\t%s\n", name.c_str(), ip_port.c_str());
      
      return true;
    }
};
