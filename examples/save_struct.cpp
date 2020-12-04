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
  DEBUG.println("");
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

  FLASH.setBlockProtection(0);
  DEBUG.println("");
  DEBUG.println("READING STATUS: ");
  DEBUG.print("Protection bit: ");
  DEBUG.println(FLASH.getBlockProtection());
  DEBUG.print("Block protection bit: ");
  DEBUG.println(FLASH.getBlockProtectionLocked());

  FLASH.pageErase(0);

  DEBUG.print("Write enable bit: ");
  DEBUG.println(FLASH.getWriteEnable());
  DEBUG.println("");
  DEBUG.println("Writing array of 8 bytes.");
  DEBUG.println("");

  struct Data {
    int d1;
    float d2;
    char d3[10];
  } data = {500, -12.20, {'h', '0', 'l', 'a'}};

  Data array;
  uint8_t* d = (uint8_t*)&data;
  int dataLen = sizeof data;

  FLASH.write(d, 0, dataLen);  // Write a pointer to struct memory
  for (auto i = 0; i < dataLen; i++) {
    DEBUG.println(d[i]);
  }

  DEBUG.println("");
  DEBUG.println("Reading bytes");

  uint8_t array1[dataLen] = {};

  FLASH.read(array1, 0, dataLen);

  for (auto i = 0; i < dataLen; i++) {
    DEBUG.println(array1[i]);
  }

  uint8_t* Rd = array1;
  memcpy(&array, Rd, dataLen);

  DEBUG.print("INT: ");
  DEBUG.println(array.d1);
  DEBUG.print("FLOAT: ");
  DEBUG.println(array.d2);
  DEBUG.print("CHAR: ");
  DEBUG.println(array.d3);
}

void loop() {
  // put your main code here, to run repeatedly:
}
