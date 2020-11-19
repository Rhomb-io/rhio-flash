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
  DEBUG.println("INFORMACION READING STATUS: ");
  DEBUG.print("Bit de protección: ");
  DEBUG.println(FLASH.getBlockProtection());
  DEBUG.print("Bit de bloqueo protección: ");
  DEBUG.println(FLASH.getBlockProtectionLocked());

  // FLASH.writeEnable(); Si lo pongo se bloquea (para page no hace falta)
  FLASH.pageErase(0);

  uint8_t array[8] = {};
  uint8_t array1[8] = {};
  uint8_t variable = 0;

  FLASH.writeEnable();

  DEBUG.print("Bit escritura habilitado: ");
  DEBUG.println(FLASH.getWriteEnable());
  DEBUG.println("");
  DEBUG.println("A continuación se va a escribir un array de 8 bytes.");
  DEBUG.println("");

  for (int k = 0; k < 8; k++) {
    variable = variable + 2 * 2;  // 4, 8, 12, 16 20, 24...
    array[k] = {variable};
    Serial.println("Generando escritura");
    Serial.println(array[k]);
  }
  FLASH.writePage(array, 0, 8);

  DEBUG.println("");
  DEBUG.println("Lectura de bytes");

  FLASH.readArray(array1, 0, 8);

  for (int i = 0; i < 8; i++) {
    DEBUG.println(array1[i]);
  }
  /*for (int i = 0; i < 8; i++) {
    DEBUG.println(FLASH.readByte(i), DEC);
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:
}
