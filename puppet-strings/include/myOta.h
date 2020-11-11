#include<WString.h>
#include <ArduinoOTA.h>

class MyOTA: public ArduinoOTAClass {
	public:
		
		// static MyOTA* thisOta;

		MyOTA () {
  		setHostname("rc-module");

			onStart(start);
			onEnd(end);
			onProgress(prog);
			onError(err);

			// thisOta = this;

  		begin();
		}


		static void start() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH)
				type = "sketch";
			else // U_FS
				type = "filesystem";
			

			// NOTE: if updating FS this would be the place to unmount FS using FS.end()
			Serial.println("Start updating " + type);
		}

		static void end() {
			Serial.println("\nEnd");
		}

		static void prog(unsigned int progress, unsigned int total) {
			if (progress%32456 == 0) {
				Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
			}
		}

		static void err(ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  }
};