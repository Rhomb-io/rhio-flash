# rhio-flash

This is a portable library based on Arduino to work with the AT25DF512C SPI Serial Flash Memory. It was developed by rhomb.io and it is open source. The AT25DF512C memory has 512 Kbit of memory and 256 pages of 256 bytes each.

## Features

* Read by SPI
* 512 kbit
* JEDEC Standard Manufacturer and Device ID Read Methodology
* Read/write one byte or array
* Four erase types

## Main functions

+ Constructor
  * RhioFlash(slaveSelectPin)
  * RhioFlash()
* beginMemory()
* erase()
* Write functions
  * write(*value, address, size)
  * write(value, address)
* Read functions
  * read(address)
  * read(*value, address, size)

There are two types of write/read functions depending on that type of value introduce in the function (uint8_t or array). If it introduces an array, is nessecary use a pointer to the array memory and put the size.

## License

This library was developed by rhomb.io and has a GNU/GPL v3 license. For more information, check the License file.
