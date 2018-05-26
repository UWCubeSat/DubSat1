//Use eUSCI_A for SPI and eUSCI_B for I2C
#ifndef SPI_H
#define SPI_H

#include <stdio.h>
#include <msp430.h>
#include <stdint.h>

// Configuration macros for UCB0CTLW0.
#define UCMODE_THREE_WIRE_SPI 				 0x0000
#define UCMODE_FOUR_WIRE_SPI_STE_ACTIVE_HIGH 0x0200
#define UCMODE_FOUR_WIRE_SPI_STE_ACTIVE_LOW  0x0400
#define UCMODE_I2C			  				 0x0600

// Clock Divider/other timing macros
#define SPI_CLOCK_DIV 						 0x0000
#define WAIT_TIME 			  				 0xFFFF


// These pins correspond to the chip select pins on our MSP CAN Block.
#define CS_1                                  0x01
#define CS_2                                  0x02
#define CS_3                                  0x04

uint8_t spiWriteInProgress;

void (*spiCallback)(void);
/**
 * @brief Initialize SPI registers required to start transceiving.
 * @details Sets up the control block to reset the device.
 * 			Configure ports necessary for pins.
 * 			Sets up prescaler for clock, which is fed in at 8 MHz.
 * 			Wait for signals to settle.
 * 			Start device.
 * 			
 */
void spiInit(uint8_t csPins);

/**
 * @brief reads/writes to/from SPI bus using buffers
 * @details This method writes *num* bytes from a buffer *pTxBuf* to the
           SPI bus using 3 Wire SPI Mode. It will store the bytes received
           during the write sequence to another client provided buffer,
           *pRxBuf*. The pRxBuf must be at least *num* bytes, and it will
           fill *num* bytes of data.
 * 
 * @param pTxBuf A pointer to the transmit buffer.
 * @param pRxBuf A pointer to the receive buffer.
 * @param num Size of the transmit buffer in bytes.
 */
void spiTransceive(uint8_t *pTxBuf, uint8_t *pRxBuf, size_t num, uint8_t csPin);


#endif	/* SPI_H */
