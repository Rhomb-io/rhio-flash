/**
 * Rhomb.io AT25DF512C library
 *
 * @author Jose Francisco Martí Martín
 * @version 0.0.1
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 */

#include <rhio-flash.h>

//*******Constructor*******

RhioFlash::RhioFlash(){};

RhioFlash::RhioFlash(uint8_t slaveSelectPin) {
  _slaveSelectPin = slaveSelectPin;
}

//******Initialization******

void RhioFlash::beginMemory() {
#ifdef Halley
  ioex.begin();
  ioex.setupIOEXHalley();
#else
  pinMode(_slaveSelectPin, OUTPUT);
#endif
#if defined(ARDUINO_SAMD_ZERO)
  SPIClass SPI(&sercom1, MISO, SCK, MOSI, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
  pinPeripheral(MOSI, PIO_SERCOM);  // Assign MOSI
  pinPeripheral(MISO, PIO_SERCOM);  // Assign MISO
  pinPeripheral(SCK, PIO_SERCOM);   // Assign SCK
#endif
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV2, MSBFIRST, SPI_MODE0));
#ifdef Halley
  ioex.disableSPIMemory();
#else
  digitalWrite(_slaveSelectPin, HIGH);
#endif
}

//******Device ID and manufacter ID******

uint32_t RhioFlash::readManufacterAndDeviceID() {  // Devolver uint32
  uint32_t jedecid;
  ComandFlash comandflash = READ_ALL_ID;
  chipSelect();
  SPI.transfer(comandflash);
  jedecid = SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid |= SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid |= SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid |= SPI.transfer(0);
  chipUnselect();
  return jedecid;
}

//******Write enable/disable******

void RhioFlash::writeEnable() { writeComand(WRITE_ENABLE); }

void RhioFlash::writeDisable() { writeComand(WRITE_DISABLE); }

//******Erase functions******

void RhioFlash::chipErase() { writeComand(CHIP_ERASE); }

// Mas funciones de borrado

//******Write functions******

void RhioFlash::writeByte(uint8_t value, uint32_t address) {
  ComandFlash comandflash = PAGE_PROGRAM;
  chipSelect();
  SPI.transfer(comandflash);
  SPI.transfer(address >> 16);
  SPI.transfer(address >> 8);
  SPI.transfer(address);
  SPI.transfer(value);
  chipUnselect();
}

//******Read functions******

uint8_t RhioFlash::readByte(uint32_t address) {
  ComandFlash comandflash = READ_ARRAY;
  uint8_t value;
  chipSelect();
  SPI.transfer(comandflash);
  SPI.transfer(address >> 16);  // Menos significativo primero
  SPI.transfer(address >> 8);
  SPI.transfer(address);
  SPI.transfer(0);
  value = SPI.transfer(0);
  chipUnselect();
  return value;
}

//*************************UTILITY*************************//

void RhioFlash::chipSelect() {
#ifdef Halley
  ioex.enableSPIMemory();
#else
  digitalWrite(_slaveSelectPin, LOW);
#endif
}

void RhioFlash::chipUnselect() {
#ifdef Halley
  ioex.disableSPIMemory();
#else
  digitalWrite(_slaveSelectPin, HIGH);
#endif
  SPI.endTransaction();
}

void RhioFlash::writeComand(ComandFlash comandflash) {
  chipSelect();
  SPI.transfer(comandflash);
  chipUnselect();
  SPI.endTransaction();
}
