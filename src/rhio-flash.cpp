/**
 * Rhomb.io AT25DF512C library
 *
 * @author Jose Francisco Martí Martín
 * @version 0.0.2
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 */

#include <rhio-flash.h>

#if defined(ARDUINO_SAMD_ZERO)
SPIClass rhSPI(&sercom1, MISO, SCK, MOSI, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
#define RH_SPI rhSPI
#else
#define RH_SPI SPI
#endif

//*******Constructor*******
RhioFlash::RhioFlash(){};

RhioFlash::RhioFlash(uint8_t slaveSelectPin) {
  _slaveSelectPin = slaveSelectPin;
}

//******Initialization******

void RhioFlash::beginMemory() {
#if MODE == Halley
  ioex.begin();
  ioex.setupIOEXHalley();
#elif MODE == MEMORY_DUINO_ZERO
  pinMode(MUX_SW, OUTPUT);
  digitalWrite(MUX_SW, LOW);
  pinMode(MEM_CS, OUTPUT);
#elif MODE == NORMAL
  pinMode(_slaveSelectPin, OUTPUT);
#endif

#if defined(ARDUINO_SAMD_ZERO)
  pinPeripheral(MOSI, PIO_SERCOM);  // Assign MOSI
  pinPeripheral(MISO, PIO_SERCOM);  // Assign MISO
  pinPeripheral(SCK, PIO_SERCOM);   // Assign SCK
#endif
  RH_SPI.begin();
  RH_SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV2, MSBFIRST, SPI_MODE0));
  chipUnselect();
}

//******Device ID and manufacter ID******

uint32_t RhioFlash::readManufacturerAndDeviceID() {  // Devolver uint32
  uint32_t jedecid;
  ComandFlash comandflash = READ_ALL_ID;
  chipSelect();
  RH_SPI.transfer(comandflash);
  jedecid = RH_SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid |= RH_SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid |= RH_SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid |= RH_SPI.transfer(0);
  chipUnselect();
  return jedecid;
}

uint16_t RhioFlash::readDeviceID() {
  uint16_t jedecid;
  ComandFlash comandflash = READ_ID;
  chipSelect();
  RH_SPI.transfer(comandflash);
  jedecid = RH_SPI.transfer(0);
  jedecid = jedecid << 8;
  jedecid = RH_SPI.transfer(0);
  chipUnselect();
  return jedecid;
}

//******Security functions******

void RhioFlash::setOtpSecurity(uint8_t *value, uint32_t addressM,
                               uint8_t size) {
  // Se necesita habilitar la escritura
  writeEnable();
  chipSelect();
  setComandAndAddress(addressM, PROGRAM_OTP);
  for (auto i = 0; i < size; i++) {
    RH_SPI.transfer(value[i]);
  }
  chipUnselect();
}

void RhioFlash::readOtpSecurity(uint8_t *value, uint32_t addressM,
                                uint8_t size) {
  chipSelect();
  setComandAndAddress(addressM, READ_OTP);
  RH_SPI.transfer(0);
  RH_SPI.transfer(0);
  for (auto i = 0; i < size; i++) {
    value[i] = RH_SPI.transfer(0);
  }
  chipUnselect();
}

//******Status Register******

void RhioFlash::setBlockProtectionLocked(uint8_t Status) {
  writeEnable();
  writeStatusRegister(7, WRITE_STATUS1, Status);
  delay(10);
}
void RhioFlash::setBlockProtection(uint8_t Status) {
  writeEnable();
  writeStatusRegister(2, WRITE_STATUS1, Status);
  delay(10);
}
void RhioFlash::setReset(uint8_t Status) {
  writeEnable();
  writeStatusRegister(2, WRITE_STATUS2, Status);
  delay(10);
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

void RhioFlash::erase() {
  writeEnable();
  writeComand(CHIP_ERASE);
  while (getBusyStatus() == 1) {
  }
}

void RhioFlash::blockErase4KB(uint32_t addressM) {
  writeEnable();
  chipSelect();
  setComandAndAddress(addressM, BLOCK_ERASE_4KB);
  chipUnselect();
  while (getBusyStatus() == 1) {
  }
}

void RhioFlash::blockErase32KB(uint32_t addressM) {
  writeEnable();
  chipSelect();
  setComandAndAddress(addressM, BLOCK_ERASE_32KB);
  chipUnselect();
  while (getBusyStatus() == 1) {
  }
}

void RhioFlash::pageErase(uint8_t page) {
  writeEnable();
  ComandFlash comandflash = PAGE_ERASE;
  chipSelect();
  RH_SPI.transfer(comandflash);
  RH_SPI.transfer(0);
  RH_SPI.transfer(page);
  RH_SPI.transfer(0);
  chipUnselect();
  while (getBusyStatus() == 1) {
  }
}

void RhioFlash::bytesErase(uint32_t addressM, uint16_t size) {
  uint8_t pag = 0, discountSize = 0, realSize = 0;
  uint32_t addresRead1, real_address;
  real_address = addressM;
  realSize = size;
  while (addressM >= 256) {
    addressM -= 256;
    pag += 1;
  }
  if ((addressM + (realSize - 1)) >= 256) {
    uint8_t array3[(-realSize + 256)] = {};
    while ((addressM + (size - 1)) >= 256) {
      size -= 1;
      discountSize += 1;
    }
    addresRead1 = discountSize + (pag * 256);
    read(array3, addresRead1, (-realSize + 256));
    pageErase(pag);
    write(array3, addresRead1, (-realSize + 256));
  } else if (addressM == 0) {
    uint8_t array4[(-realSize + 256)] = {};
    uint32_t addresRead3;
    addresRead3 = real_address + realSize;
    read(array4, addresRead3, (-realSize + 256));
    pageErase(pag);
    write(array4, addresRead3, (-realSize + 256));
  } else {
    uint8_t array1[(-size + 256 - addressM)] = {};
    uint8_t array2[(256 - (256 - addressM))] = {};
    uint32_t addresRead2;
    addresRead1 = real_address + size;
    read(array1, addresRead1, (-size + 256 - addressM));
    addresRead2 = pag * 256;
    read(array2, addresRead2, (256 - (256 - addressM)));
    pageErase(pag);
    write(array1, addresRead1, (-size + 256 - addressM));
    write(array2, addresRead2, (256 - (256 - addressM)));
  }
}

//******Write functions******

void RhioFlash::write(uint8_t value, uint32_t addressM) {
  writeEnable();
  chipSelect();
  setComandAndAddress(addressM, PAGE_PROGRAM);
  RH_SPI.transfer(value);
  chipUnselect();
  while (getBusyStatus() == 1) {
  }
}

void RhioFlash::write(uint8_t *value, uint32_t addressM, uint16_t size) {
  writeEnable();
  chipSelect();
  setComandAndAddress(addressM, PAGE_PROGRAM);
  for (auto i = 0; i < size; i++) {
    RH_SPI.transfer(value[i]);
  }
  chipUnselect();
  while (getBusyStatus() == 1) {
  }
}


//******Read functions******

uint8_t RhioFlash::read(uint32_t addressM) {
  uint8_t value;
  chipSelect();
  setComandAndAddress(addressM, READ_ARRAY);
  RH_SPI.transfer(0);
  value = RH_SPI.transfer(0);
  chipUnselect();
  while (getBusyStatus() == 1) {
  }
  return value;
}

void RhioFlash::read(uint8_t *value, uint32_t addressM, uint16_t size) {
  chipSelect();
  setComandAndAddress(addressM, READ_ARRAY);
  RH_SPI.transfer(0);
  for (auto i = 0; i < size; i++) {
    value[i] = RH_SPI.transfer(0);
  }
  chipUnselect();
  while (getBusyStatus() == 1) {
  }
}

//*********************Other functions*********************//

void RhioFlash::resetOperation() {
  ComandFlash comandflash = RESET;
  chipSelect();
  RH_SPI.transfer(comandflash);
  RH_SPI.transfer(0xD0);  // Confirmation byte for reset
  chipUnselect();
}

void RhioFlash::deepPowerDown() { writeComand(DEEP_POWER_DOWN); }

void RhioFlash::resumeFromDeepPowerDown() {
  writeComand(RESUME_FORM_DEEP_POWER_DOWN);
}

void RhioFlash::ultraDeepPowerDown() { writeComand(ULTRA_DEEP_POWER_DOWN); }

void RhioFlash::exitultraDeepPowerDown() {
  chipSelect();
  RH_SPI.transfer(0);
  chipUnselect();
}

//*************************UTILITY*************************//

void RhioFlash::chipSelect() {
#if MODE == Halley
  ioex.enableSPIMemory();
#elif MODE == MEMORY_DUINO_ZERO
  digitalWrite(MEM_CS, LOW);
#elif MODE == NORMAL
  digitalWrite(_slaveSelectPin, LOW);
#endif
}

void RhioFlash::chipUnselect() {
#if MODE == Halley
  ioex.disableSPIMemory();
#elif MODE == MEMORY_DUINO_ZERO
  digitalWrite(MEM_CS, HIGH);
#elif MODE == NORMAL
  digitalWrite(_slaveSelectPin, HIGH);
#endif
  RH_SPI.endTransaction();
}

uint8_t RhioFlash::readStatusRegister(uint8_t bits) {
  ComandFlash comandflash = READ_STATUS;
  uint8_t value;
  chipSelect();
  RH_SPI.transfer(comandflash);
  value = RH_SPI.transfer(0);
  chipUnselect();
  value = (value >> bits) & 1;
  return value;
}

void RhioFlash::writeStatusRegister(uint8_t bits, ComandFlash comandflash,
                                    uint8_t Status) {
  uint8_t value = 0;
  if (comandflash == WRITE_STATUS1) {
    uint8_t BPL = readStatusRegister(7);
    uint8_t BP0 = readStatusRegister(2);
    value = (BPL << 7);
    BP0 = (BP0 << 2);
    value |= BP0;
  }
  uint8_t mask = 1;
  mask = mask << bits;
  mask = ~mask;
  Status = (Status << bits);
  value &= mask;
  value |= Status;
  chipSelect();
  RH_SPI.transfer(comandflash);
  RH_SPI.transfer(value);
  chipUnselect();
}

void RhioFlash::writeComand(ComandFlash comandflash) {
  chipSelect();
  RH_SPI.transfer(comandflash);
  chipUnselect();
}

void RhioFlash::setComandAndAddress(uint32_t addressM,
                                    ComandFlash comandflash) {
  RH_SPI.transfer(comandflash);
  RH_SPI.transfer(addressM >> 16);  // Menos significativo primero
  RH_SPI.transfer(addressM >> 8);
  RH_SPI.transfer(addressM);
}
