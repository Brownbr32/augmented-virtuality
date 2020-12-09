#include <stdint.h>
#include "Device.h"



template <class DataType> // legacy, see Device.h

// creates variable length linked list of devices.
// done so as to facilitate addition of new devices 
  // (none currently needed, however)
class DeviceLL {
  public:
    DeviceLL<DataType>* next;
    Device<DataType> *device;
    
    DeviceLL(const char name[10], uint8_t pin, 
        DataType* val, DeviceLL<DataType>* 
        next = NULL) {
      this->next = next;
      this->device = new Device<DataType>(name, 
          pin, val);
    };

    // destructor; never used. But I feel like it's 
      // good practice. ;) Also, calls destructor of 
      // classes to which it ponits
    ~DeviceLL() {
      if (this->next != NULL) delete this->next;
      delete this->device;
      Serial.println("Destroying DeviceLL\n");
    };
};
