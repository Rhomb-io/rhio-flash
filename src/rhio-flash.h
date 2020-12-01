#pragma once
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
#include <Arduino.h>
#include <SPI.h>

#define Rhomb_io
#define Halley

#ifdef Rhomb_io
#include "rhio-pinmap.h"
#endif

#ifdef Halley
#include "rhio-ioexpander.h"
#endif

#if defined(ARDUINO_SAMD_ZERO)
#include <wiring_private.h>
#endif

class RhioFlash {
 public:
  /***************************************************************************
   *                                 Constructor
   **************************************************************************/

  RhioFlash();
  RhioFlash(uint8_t slaveSelectPin);  // Only if you don't use halley

  /***************************************************************************
   *                               Initialization
   **************************************************************************/

  void beginMemory();

  /***************************************************************************
   *                           Device ID and manufacter ID
   **************************************************************************/

  uint32_t readManufacterAndDeviceID();
  uint16_t readDeviceID();

  /***************************************************************************
   *                             Security functions
   **************************************************************************/

  void setOtpSecurity(uint8_t *value, uint32_t address, uint8_t size);
  void readOtpSecurity(uint8_t *value, uint32_t address, uint8_t size);

  /***************************************************************************
   *                             Status registers
   **************************************************************************/

  void setBlockProtectionLocked(uint8_t Status);
  void setBlockProtection(uint8_t Status);
  void setReset(uint8_t Status);
  uint8_t getBlockProtectionLocked();
  uint8_t getError();
  uint8_t getWriteProtect();
  uint8_t getBlockProtection();
  uint8_t getWriteEnable();
  uint8_t getBusyStatus();

  /***************************************************************************
   *                           Enable/Disable write
   **************************************************************************/

  void writeEnable();
  void writeDisable();

  /***************************************************************************
   *                            Erase functions
   **************************************************************************/

  void erase();
  void blockErase4KB(uint32_t address);
  void blockErase32KB(uint32_t address);
  void pageErase(uint8_t page);
  void bytesErase(uint32_t address, uint16_t size);

  /***************************************************************************
   *                            Write functions
   **************************************************************************/

  void write(uint8_t value, uint32_t address);
  void write(uint8_t *value, uint32_t address, uint16_t size);

  /***************************************************************************
   *                            Read functions
   **************************************************************************/

  uint8_t read(uint32_t address);
  void read(uint8_t *value, uint32_t address, uint16_t size);
  // void readDualArray();

  /***************************************************************************
   *                            Other functions
   **************************************************************************/

  void resetOperation();
  void deepPowerDown();
  void resumeFromDeepPowerDown();
  void ultraDeepPowerDown();
  void exitultraDeepPowerDown();

 private:
#ifdef Halley
  RhioIOExpander ioex;
#endif
  typedef enum _ComandFlash {
    READ_ARRAY = 0x0B,
    DUAL_OUTPUT_READ = 0x3B,
    PAGE_ERASE = 0x81,
    BLOCK_ERASE_4KB = 0x20,
    BLOCK_ERASE_32KB = 0x52,
    CHIP_ERASE = 0x62,
    PAGE_PROGRAM = 0x02,
    WRITE_ENABLE = 0x06,
    WRITE_DISABLE = 0x04,
    PROGRAM_OTP = 0x9B,
    READ_OTP = 0x77,
    READ_STATUS = 0x05,
    WRITE_STATUS1 = 0x01,
    WRITE_STATUS2 = 0x31,
    RESET = 0xF0,
    READ_ALL_ID = 0x9F,
    READ_ID = 0x15,
    DEEP_POWER_DOWN = 0xB9,
    RESUME_FORM_DEEP_POWER_DOWN = 0xAB,
    ULTRA_DEEP_POWER_DOWN = 0x79,
  } ComandFlash;
  ComandFlash comandflash;
  uint8_t _slaveSelectPin;

  void chipSelect();
  void chipUnselect();
  uint8_t readStatusRegister(uint8_t bits);
  void writeStatusRegister(uint8_t bits, ComandFlash comandflash,
                           uint8_t Status);
  void writeComand(ComandFlash comandflash);
  void setComandAndAddress(uint32_t address, ComandFlash comandflash);
};
