#include<WString.h>
#include <ArduinoOTA.h>


// inherits ArduinoOTA class. Used for IP device reprogramming
class MyOTA: public ArduinoOTAClass {
	public:
		

		// constructor function. Please add security to me. :)
			// It would be GREAT to have a PASSWORD required in order
				// to actually program the device.
		MyOTA () {
  		setHostname("rc-module");

			// ArduinoOTA depends somewhat on user-defined functions.
				// These user-defined functions are passed as function
					// pointers to a variable defined in the on*(fPtr)
					// functions referenced below. 
			onStart(start);
			onEnd(end);
			onProgress(prog);
			onError(err);
  		begin();
		}

		
		static void start() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH) 
				// flashes entire system's storage
				type = "sketch";
			else // U_FS
				type = "filesystem";	// adds/edits a file 
					//in a given FS
			

			// NOTE: if updating FS this would be the place to 
				// unmount FS using FS.end()
			Serial.println("Start updating " + type); 
				// debugging stuff, visible over IP.
		}

		static void end() {
			Serial.println("\nEnd");
			// simple sign-off
		}

		static void prog(unsigned int progress, unsigned int total) {
			if (progress%32456 == 0) {
				// periodic progress report. done in % completion.
				Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
			}
		}

		// different error enums.
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