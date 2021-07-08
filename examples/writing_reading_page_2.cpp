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

  FLASH.erase();
  uint8_t array[256] = {};
  uint8_t array1[206] = {};
  uint8_t array2[50] = {};
  int i;

  DEBUG.print("Write enable bit: ");
  DEBUG.println(FLASH.getWriteEnable());
  DEBUG.println("");
  DEBUG.println("Writing array of 8 bytes.");
  DEBUG.println("");

  for (i = 0; i < 256; i++) {
    array[i] = i;
    DEBUG.println("Writing generated");
    DEBUG.println(array[i]);
    if (i == 255) {
      break;
    }
  }
#define addressP 50
  FLASH.write(array, addressP, 256);

  DEBUG.println("");
  DEBUG.println("Reading bytes");

  FLASH.read(array1, addressP, 206);

  FLASH.read(array2, 0, 50);

  for (int i = 0; i < 206; i++) {
    DEBUG.println(array1[i]);
    if (i == 205) {
      break;
    }
  }
  for (int i = 0; i < 50; i++) {
    DEBUG.println(array2[i]);
    if (i == 49) {
      break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
