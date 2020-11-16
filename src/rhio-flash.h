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
#include "FXL6408.h"
#endif

#if defined(ARDUINO_SAMD_ZERO)
#include <wiring_private.h>
#endif

class RhioFlash {
 public:
  /***************************************************************************
   *                          Constructor
   **************************************************************************/

  RhioFlash();
  RhioFlash(uint8_t slaveSelectPin);  // Only if you don't use halley

  /***************************************************************************
   *                          Initialization
   **************************************************************************/

  void beginMemory();

  /***************************************************************************
   *                      Device ID and manufacter ID
   **************************************************************************/

  uint32_t readManufacterAndDeviceID();
  uint8_t readDeviceID();

  /***************************************************************************
   *                          Status registers
   **************************************************************************/

  // Las funciones tanto de lectura como de escritura

  /***************************************************************************
   *                       Enable/Disable write
   **************************************************************************/

  void writeEnable();
  void writeDisable();

  /***************************************************************************
   *                            Erase functions
   **************************************************************************/

  void chipErase();
  // Tres funciones mas de borrar

  /***************************************************************************
   *                            Write functions
   **************************************************************************/

  void writeByte(uint8_t value, uint32_t address);

  /***************************************************************************
   *                            Read functions
   **************************************************************************/

  uint8_t readByte(uint32_t address);

 private:
#ifdef Halley
  FXL6408 ioex;
#endif
  typedef enum _ComandFlash {
    READ_ARRAY = 0x0B,
    DUAL_OUTPUT_READ = 0x3B,
    PAGE_ERASE = 0x81,
    BLOCK_ERASE_4KB = 0x20,
    BLOCK_ERASE_32KB = 0x52,
    CHIP_ERASE = 0x60,
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
  void writeComand(ComandFlash comandflash);
};
