#define DEBUG SerialUSB
#include "rhio-flash.h"

RhioFlash FLASH;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  DEBUG.begin(115200);
  while (!DEBUG)
    ;

  DEBUG.println("Set");
  FLASH.beginMemory();
  uint32_t jedecid = FLASH.readManufacterAndDeviceID();
  uint8_t jedecid1, jedecid2, jedecid3, jedecid4;
  jedecid1 = jedecid >> 24;
  jedecid2 = jedecid >> 16;
  jedecid3 = jedecid >> 8;
  jedecid4 = jedecid;
  DEBUG.print("Manufacter ID: ");
  DEBUG.println(jedecid1, HEX);
  DEBUG.print("Device ID (Part1): ");
  DEBUG.println(jedecid2, HEX);
  DEBUG.print("Device ID (Part2): ");
  DEBUG.println(jedecid3, HEX);
  DEBUG.print("Extended: ");
  DEBUG.println(jedecid4, HEX);

  FLASH.erase();
  while (FLASH.getBusyStatus() == 1) {
  }

  DEBUG.println("");
  DEBUG.println("Writing page ");
  // Write an array of 256 bytes
  uint8_t array[256] = {}, i, array2[256];
  for (i = 0; i < 256; i++) {
    array[i] = i;
    DEBUG.println(array[i]);
    if (i == 255) {
      break;
    }
  }
  DEBUG.println("");
  DEBUG.println("First for finished");
  DEBUG.println("");
  delay(5000);

  FLASH.write(array, 0, 256);
  // Erase 20 bytes from 250 address
  FLASH.bytesErase(250, 20);
  DEBUG.println("");
  DEBUG.println("Reading page ");
  FLASH.read(array2, 0, 256);

  for (i = 0; i < 256; i++) {
    array[i] = i;
    DEBUG.println(array2[i]);
    if (i == 255) {
      break;
    }
  }

  DEBUG.println("");
  DEBUG.println("Second for finished");
  DEBUG.println("");
}

void loop() {
  // put your main code here, to run repeatedly:
}
