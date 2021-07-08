#define DEBUG Serial
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

  FLASH.erase();
  FLASH.write(0xE3, 0);
  DEBUG.print("Reading address 0: ");  // 11100011
  DEBUG.println(FLASH.read(0), BIN);
}

void loop() {
  // put your main code here, to run repeatedly:
}
