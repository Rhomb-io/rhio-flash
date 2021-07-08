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
  DEBUG.println("");
  FLASH.beginMemory();
  uint32_t jedecid = FLASH.readManufacturerAndDeviceID();
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

  FLASH.pageErase(0);
  uint8_t array[256] = {};
  uint8_t array1[256] = {};

  DEBUG.println("Writing");
  DEBUG.println("");

  for (int k = 0; k < 256; k++) {
    array[k] = k;
    DEBUG.println(array[k]);
    if (k == 255) {
      break;
    }
  }

  FLASH.write(array, 256, 256);
  FLASH.bytesErase(256, 20);
  DEBUG.println("");
  DEBUG.println("Reading bytes");

  FLASH.read(array1, 256, 256);

  for (int i = 0; i < 256; i++) {
    DEBUG.println(array1[i]);
    if (i == 255) {
      break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
