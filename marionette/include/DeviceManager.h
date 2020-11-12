#include <Arduino.h>
#include <stdint.h>
#include "DeviceLL.h"

struct DeviceManager {
  private:
    unsigned int glassCeiling = 1023; // only have dutyCycle range of 0-1023 on PWM
    DeviceLL<int>* deviceList;
    
  
  int freq = 90;
  public:
    DeviceManager(int* packet) {
      int* valPtr = (int*) packet;
      deviceList = new DeviceLL<int>("ESC", D1, valPtr++);
      deviceList = new DeviceLL<int>("servo", D0, valPtr++, deviceList);
      analogWriteFreq(freq);
    };

    void print(bool printHeader = false) {
      if(printHeader) Serial.println("\nprinting devices:");
      DeviceLL<int>* tmp = deviceList;
      while (tmp) {
        printf("Device Name: %s\n", tmp->device->name);
        tmp = tmp->next;
      }
    }
    
    unsigned char dataLength() {
      return sizeof(int);
    };
    
    bool write(bool printEnabled) {
      DeviceLL<int>* tmp = deviceList;
      if(printEnabled) Serial.println("Writing the following values to devices:");
      while (tmp) {
        if(printEnabled) Serial.printf("%s:\t%d\n", tmp->device->name,withGlassCeiling(tmp->device->val));
        analogWrite(tmp->device->pin, withGlassCeiling(tmp->device->val));
        tmp = tmp->next;
      }
      if(printEnabled) Serial.println("");
      return true;
    };

    void writeFreq(int Hz) {
      freq = Hz;
      analogWriteFreq(freq);
    }

    int withGlassCeiling(int* val) {
      return ((unsigned int) *val > glassCeiling) ? glassCeiling : (unsigned int) *val;
    }

    ~DeviceManager() {
        delete deviceList;
        Serial.println("Destroying DeviceManager");
    };
};
