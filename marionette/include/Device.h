#include <stdint.h>



template <class DataType> // legacy: no need for template. 
  // Was used to enable data type flexibility. 
  // No longer helpful or necessary.
class Device {
  static const int off = 132; // ESC, servo = off when
    // PWM duty cycle == 132.
  public:
    char name[10];  // device name. Useful for: 
      // debugging
      // metadata
      // logging potential
    uint8_t pin;    // pin that device is connected to
    DataType* val;  // a pointer to (in this case int), where 
      // array's first and second `int` are stored. This skips multiple copies and 
      // optimizes for speed

    // constructor (these are pointers because DLL is used to enhance flexibility)
    Device(const char name[10], uint8_t pin, DataType* val) {
      // copies 9 characters, then puts a 0 (or end string char) after.
      strncpy(this->name, name,9);
      this->name[9]   = 0;
      this->pin       = pin;
      this->val       = val;
      *(this->val)    = off;
    };

    // destructor. Never really used, but there for garbage cleanup potential. 
    ~Device() {
      Serial.println("Destroying DeviceLL");
    };
};
