#include <stdint.h>



template <class DataType>
class Device {
  static const int off = 132;
  public:
    char name[10];
    uint8_t pin;
    DataType* val;
    
    Device(const char name[10], uint8_t pin, DataType* val) {
      strncpy(this->name, name,9);
      this->name[9]   = 0;
      this->pin       = pin;
      this->val       = val;
      *(this->val)    = off;
    };

    ~Device() {
      Serial.println("Destroying DeviceLL");
    };
};
