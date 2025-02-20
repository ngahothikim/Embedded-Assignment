#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>

// Define MAX7219 Registers
#define MAX7219_NOOP          0x00  // No operation (used for daisy chaining)
#define MAX7219_DIGIT0        0x01  // Digit 0 register
#define MAX7219_DIGIT1        0x02  // Digit 1 register
#define MAX7219_DIGIT2        0x03  // Digit 2 register
#define MAX7219_DIGIT3        0x04  // Digit 3 register
#define MAX7219_DIGIT4        0x05  // Digit 4 register
#define MAX7219_DIGIT5        0x06  // Digit 5 register
#define MAX7219_DIGIT6        0x07  // Digit 6 register
#define MAX7219_DIGIT7        0x08  // Digit 7 register
#define MAX7219_DECODE_MODE   0x09  // Decode mode register
#define MAX7219_INTENSITY     0x0A  // Intensity register
#define MAX7219_SCAN_LIMIT    0x0B  // Scan limit register
#define MAX7219_SHUTDOWN      0x0C  // Shutdown register
#define MAX7219_DISPLAY_TEST  0x0F  // Display test register

void MAX7219_Init(void);
void MAX7219_SendCommand(uint8_t address, uint8_t data);
void MAX7219_ClearDisplay(void);
void MAX7219_DisplayTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void MAX7219_DisplayDate(uint8_t day, uint8_t month, uint16_t year);
void MAX7219_Shutdown(uint8_t enable);
void update_display(void);
#endif // MAX7219_H
