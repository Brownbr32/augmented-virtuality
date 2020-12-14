#include <Arduino.h>
#include <stdint.h>
#include "DeviceLL.h"

struct DeviceManager {
  private:
    const unsigned int glassCeiling = 1023; // dutyCycle range is 0-1023
      // used in `withGlassCeiling` to ensure 1023 not exceeded
    DeviceLL<int>* deviceList; // Dynamic Linked List of Devices, 
      // each registerd with pinNum, name, and *int (int pointer)
    
  
  int freq = 90; // determines frequency of PWM. Currently set to 90Hz
  public:

    /*
     *  constructs DeviceManager object. 
     *    DeviceManager contains:
     *      Device_Linked_List
     *      const: glassCeiling
     * 
     *    Functions include:
     *      print:        prints device info
     *      dataLength:   gets length of data being used (deprecated)
     *      write:        writes packet data to devices, may print
     *      writeFreq:    allows live changes to PWM frequency
     *      wGlassCeil:   protects agains PWM over-writing
     */
    DeviceManager(int* packet) {
      int* valPtr = (int*) packet; // takes integer pointers from 
        // the packet array. Reduces data duplicity, reduces copy time

      deviceList = new DeviceLL<int>("ESC", D1, valPtr++);
        // ESC placed first, run last, because order inverts in use

      deviceList = new DeviceLL<int>("servo", D0, // adds servo
        valPtr++, deviceList); // will run first, then goes to pointer
          // associated with ESC

      analogWriteFreq(freq); // sets write frequency. 
        // Tested to be best-performant frequency for ESC, servo
    };

    void print(bool printHeader = false) {
      if(printHeader) Serial.println("\nprinting devices:"); // prettify

      DeviceLL<int>* tmp = deviceList;  // makes a temp pointer,
        // prevents changing of class DLL pointer.

      while (tmp) {
        printf("Device Name: %s\n", tmp->device->name); // prints name
        tmp = tmp->next;  // goes to next node in list.
      }
    }
    
    unsigned char dataLength() {
      return sizeof(int); // deprecated. should return 4
    };
    
    bool write(bool printEnabled) {
      
      DeviceLL<int>* tmp = deviceList;  // makes a temp pointer,
        // prevents changing of class DLL pointer.

      if(printEnabled)      // prints device data, if desired
        Serial.println("Writing the following values to devices:");

      while (tmp) {         // runs until end of list reached with NULL

        if(printEnabled)    // prints device data, if desired
          Serial.printf("%s:\t%d\n", tmp->device->name,
                withGlassCeiling(tmp->device->val));

        analogWrite(tmp->device->pin,     // writes data to device
              withGlassCeiling(tmp->device->val));

        tmp = tmp->next;  // goes to next node in list.
      }
      if(printEnabled) Serial.println("");

      return true;  // return value enabled for futore error handling
    };

    void writeFreq(int Hz) {
      freq = Hz;
      analogWriteFreq(freq);
    }

    // returns val, or 1023 (glassCeiling), whichever is LESSER
    int withGlassCeiling(int* val) {
      return ((unsigned int) *val > glassCeiling) ? 
            glassCeiling : (unsigned int) *val;
    }

    // just your normal DESTRUCTOR
    ~DeviceManager() {
        delete deviceList;
        Serial.println("Destroying DeviceManager");
    };
};
