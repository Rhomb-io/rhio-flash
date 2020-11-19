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

/***************************************************************************
 *                              REVISAR
 *
 * Añadir el writeEnable a las funciones que se necesiten para no tenerlo que
 *  hacer siempre en el main con la mayoria de funciones
 *
 **************************************************************************/

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
  chipUnselect();
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

uint16_t RhioFlash::readDeviceID() {
  uint16_t jedecid;
  ComandFlash comandflash = READ_ID;
  chipSelect();
  SPI.transfer(comandflash);
  jedecid = SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid = SPI.transfer(0);
  chipUnselect();
  return jedecid;
}

//******Security functions******

void RhioFlash::setOtpSecurity(uint8_t *value, uint32_t address, uint8_t size) {
  // Se necesita habilitar la escritura
  chipSelect();
  setComandAndAddress(address, PROGRAM_OTP);
  for (auto i = 0; i < size; i++) {
    SPI.transfer(value[i]);
  }
  chipUnselect();
}

void RhioFlash::readOtpSecurity(uint8_t *value, uint32_t address,
                                uint8_t size) {
  chipSelect();
  setComandAndAddress(address, READ_OTP);
  SPI.transfer(0);
  SPI.transfer(0);
  for (auto i = 0; i < size; i++) {
    value[i] = SPI.transfer(0);
  }
  chipUnselect();
}

//******Status Register******

void RhioFlash::setBlockProtectionLocked(uint8_t Status) {
  writeStatusRegister(7, WRITE_STATUS1, Status);
}
void RhioFlash::setBlockProtection(uint8_t Status) {
  writeStatusRegister(2, WRITE_STATUS1, Status);
}
void RhioFlash::setReset(uint8_t Status) {
  writeStatusRegister(2, WRITE_STATUS2, Status);
}
uint8_t RhioFlash::getBlockProtectionLocked() { return readStatusRegister(7); }
uint8_t RhioFlash::getError() { return readStatusRegister(5); }
uint8_t RhioFlash::getWriteProtect() { return readStatusRegister(4); }
uint8_t RhioFlash::getBlockProtection() { return readStatusRegister(2); }
uint8_t RhioFlash::getWriteEnable() { return readStatusRegister(1); }
uint8_t RhioFlash::getBusyStatus() { return readStatusRegister(0); }

//******Write enable/disable******

void RhioFlash::writeEnable() { writeComand(WRITE_ENABLE); }

void RhioFlash::writeDisable() { writeComand(WRITE_DISABLE); }

//******Erase functions******

void RhioFlash::chipErase() { writeComand(CHIP_ERASE); }

void RhioFlash::blockErase4KB(uint32_t address) {
  chipSelect();
  setComandAndAddress(address, BLOCK_ERASE_4KB);
  chipUnselect();
}

void RhioFlash::blockErase32KB(uint32_t address) {
  chipSelect();
  setComandAndAddress(address, BLOCK_ERASE_32KB);
  chipUnselect();
}

void RhioFlash::pageErase(uint8_t page) {
  ComandFlash comandflash = PAGE_ERASE;
  chipSelect();
  SPI.transfer(comandflash);
  SPI.transfer(0);
  SPI.transfer(page);
  SPI.transfer(0);
  chipUnselect();
}

//******Write functions******

void RhioFlash::writeByte(uint8_t value, uint32_t address) {
  chipSelect();
  setComandAndAddress(address, PAGE_PROGRAM);
  SPI.transfer(value);
  chipUnselect();
}

void RhioFlash::writePage(uint8_t *value, uint32_t address, uint8_t size) {
  chipSelect();
  setComandAndAddress(address, PAGE_PROGRAM);
  for (auto i = 0; i < size; i++) {
    SPI.transfer(value[i]);
  }
  chipUnselect();
}

//******Read functions******

uint8_t RhioFlash::readByte(uint32_t address) {
  uint8_t value;
  chipSelect();
  setComandAndAddress(address, READ_ARRAY);
  SPI.transfer(0);
  value = SPI.transfer(0);
  chipUnselect();
  return value;
}

void RhioFlash::readArray(uint8_t *value, uint32_t address, uint8_t size) {
  chipSelect();
  setComandAndAddress(address, READ_ARRAY);
  SPI.transfer(0);
  for (auto i = 0; i < size; i++) {
    value[i] = SPI.transfer(0);
  }
  chipUnselect();
}

//*********************Other functions*********************//

void RhioFlash::resetOperation() {
  ComandFlash comandflash = RESET;
  chipSelect();
  SPI.transfer(comandflash);
  SPI.transfer(0xD0);  // Confirmation byte for reset
  chipUnselect();
}

void RhioFlash::deepPowerDown() { writeComand(DEEP_POWER_DOWN); }

void RhioFlash::resumeFromDeepPowerDown() {
  writeComand(RESUME_FORM_DEEP_POWER_DOWN);
}

void RhioFlash::ultraDeepPowerDown() { writeComand(ULTRA_DEEP_POWER_DOWN); }

void RhioFlash::exitultraDeepPowerDown() {
  chipSelect();
  SPI.transfer(0);
  chipUnselect();
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

uint8_t RhioFlash::readStatusRegister(uint8_t bits) {
  ComandFlash comandflash = READ_STATUS;
  uint8_t value;
  chipSelect();
  SPI.transfer(comandflash);
  value = SPI.transfer(0);
  chipUnselect();
  value = (value >> bits) & 1;
  return value;
}

void RhioFlash::writeStatusRegister(uint8_t bits, ComandFlash comandflash,
                                    uint8_t Status) {
  uint8_t value = 0;
  Status = (Status << bits);
  value |= Status;
  chipSelect();
  SPI.transfer(comandflash);
  SPI.transfer(value);
  chipUnselect();
}

void RhioFlash::writeComand(ComandFlash comandflash) {
  chipSelect();
  SPI.transfer(comandflash);
  chipUnselect();
}

void RhioFlash::setComandAndAddress(uint32_t address, ComandFlash comandflash) {
  SPI.transfer(comandflash);
  SPI.transfer(address >> 16);  // Menos significativo primero
  SPI.transfer(address >> 8);
  SPI.transfer(address);
}
