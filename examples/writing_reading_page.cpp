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

  FLASH.setBlockProtection(0);
  DEBUG.println("");
  DEBUG.println("READING STATUS: ");
  DEBUG.print("Protection bit: ");
  DEBUG.println(FLASH.getBlockProtection());
  DEBUG.print("Block protection bit: ");
  DEBUG.println(FLASH.getBlockProtectionLocked());

  FLASH.pageErase(0);
  uint8_t array[8] = {};
  uint8_t array1[8] = {};
  uint8_t variable = 0;

  DEBUG.print("Write enable bit: ");
  DEBUG.println(FLASH.getWriteEnable());
  DEBUG.println("");
  DEBUG.println("Writing array of 8 bytes.");
  DEBUG.println("");

  for (int k = 0; k < 8; k++) {
    variable = variable + 2 * 2;  // 4, 8, 12, 16 20, 24...
    array[k] = {variable};
    DEBUG.println("Writing generated");
    DEBUG.println(array[k]);
  }
  FLASH.write(array, 0, 8);

  DEBUG.println("");
  DEBUG.println("Reading bytes");

  FLASH.read(array1, 0, 8);

  for (int i = 0; i < 8; i++) {
    DEBUG.println(array1[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
