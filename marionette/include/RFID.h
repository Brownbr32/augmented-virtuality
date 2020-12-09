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
      SPI.begin();
      PCD_Init();
      PCD_DumpVersionToSerial();
      reactivationTime = 0;
    }

    uint checkMe(bool printEnable = false) {
      if (PICC_IsNewCardPresent() && PICC_ReadCardSerial()) {
        char list[4];
        for(int i = 0; i < 12; i++)
          list[i] = (char)uid.uidByte[i];
        if(printEnable) Serial.printf(tagNotice.c_str(),list);
        reactivationTime = millis() + MILLIS_DELAY;
      }
      return reactivationTime;
    }
};