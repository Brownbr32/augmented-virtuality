#include <stdint.h>
#include "Device.h"



template <class DataType>
class DeviceLL {
  public:
    DeviceLL<DataType>* next;
    Device<DataType> *device;
    
    DeviceLL(const char name[10], uint8_t pin, DataType* val, DeviceLL<DataType>* next = NULL) {
      this->next = next;
      this->device = new Device<DataType>(name, pin, val);
    };

    ~DeviceLL() {
      if (this->next != NULL) delete this->next;
      delete this->device;
      Serial.println("Destroying DeviceLL\n");
    };
};
