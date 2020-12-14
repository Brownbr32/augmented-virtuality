#include <SPI.h>
#include <MFRC522.h>

#define MILLIS_DELAY 1000

class RFID: public MFRC522 {
  String tagNotice = "Hit: 0x%X!\n";
  uint reactivationTime;

  int toInt(char* ptr) {
    return *(int*)ptr;
  }
  public:
    RFID(int sda = 15, int rst = 0) : MFRC522(sda, rst) {
      SPI.begin();  // initializes SPI communication
      PCD_Init();   // inits state for RFID tag
      PCD_DumpVersionToSerial();  // dumps data to serial, purpose: dbg
      reactivationTime = 0; // resets reactivationTime
    }

    // if tag is run over, resets reactivationTime
    uint checkMe(bool printEnable = false) {

      // if both of these trigger, then a tag has been run over
      if (PICC_IsNewCardPresent() && PICC_ReadCardSerial()) { // therefore
        // begin debug info
        char list[4];
        for(int i = 0; i < 12; i++)
          list[i] = (char)uid.uidByte[i];
        if(printEnable) Serial.printf(tagNotice.c_str(),list);
        // end debug info
      
        // resets reactivationTime
        reactivationTime = millis() + MILLIS_DELAY;
      }
      return reactivationTime; // returns remaining time to reactivation
        // if reactivationTime <= time
          // then heed incoming UDP packets.
    }
};