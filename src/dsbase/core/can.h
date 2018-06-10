/* ========================================
 *
 * Copyright UW Satellite Team C&DH, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/



#ifndef CAN_H
#define CAN_H
#include <inttypes.h>
#include "spi.h"


#define CAN_TX_BUSY 0x07
// Global function pointer to point to the function
// when data is received through CAN
void (*ReceiveCallback0)(uint8_t, uint8_t*, uint32_t);
void (*ReceiveCallback1)(uint8_t, uint8_t*, uint32_t);


// Static fields
uint8_t mcpMode;

/* Initializes MCP25625 chip
 * Output: 0 on success or else error code
 */
uint8_t canInit(void);

/* Check if one or more packets have been dropped, then resets to 0.
 * Output: 0 on no packets dropped, non zero if one or more packets have been dropped.
 */
uint8_t canRxErrorCheck(void);

/* Check if there are any buffers with a request to send in progress (We shouldn't overwrite these)
 * Output: 0 if all buffers are empty, otherwise 1 || 2 || 4 if TxB 0 1 and/or 2 are still waiting to send.
 */
uint8_t canTxCheck(void);

/* bufNum: TxBuf to write to, from 0 to 2
 * Tech: technical details
 *  Msg: message buffer
 */
uint8_t canSend(uint8_t bufNum, uint8_t* tech, uint8_t* msg);

/* Sets register value to value
 * Input: address: register address
 *        value: value to assign
 * Output: 0 on success or else error code
 * TODO: Check for error RH
 */
uint8_t setRegister(uint8_t address, uint8_t value);

/* Reads value at address and changes value to read value.
 * Input: address: register address
 *        *value: value to load
 * Output: 0 on success or else error code
 */
uint8_t readRegister(uint8_t address, uint8_t* value);


/* Sets the specified Tx Buffer to the value
 * Input: address: Transmit buffer
 *        value: value to load
 * Output: 0 on success or else error code
 */
uint8_t loadTxBuf(uint8_t address, uint8_t value);

/* Primes the TxBnCTRL register to be ready for sending
 * Input: n: in the bit format of nnn where n could be a
 *        0 or 1. Setting to 1 sets the register to be ready
 *        for transmission.
 * Output: 0 on success or else error code
 */
uint8_t requestToSend(uint8_t n);

/* Read a message from the CAN bus
 * Input: length: length of message
 *        buf: pointer to array of the message
 *        priority: arbitration id to determine priority
 * Output: 0 on success or else error code
 */
uint8_t readCan(uint8_t length, uint8_t* buf, uint16_t priority);

/* Reads the status of the CAN registers
 * Input: status: The payload to hold the status.
 * Output: 0 on success or else error code
 */
uint8_t readStatus(uint8_t *status);

/* Reads the filter match and message type of the CAN registers
 * Input: status: The payload to hold the status.
 * Output: 0 on success or else error code
 */
uint8_t readRXStatus(uint8_t *status);

/* Set or clear individual bits in status and control
 * registers.
 * Input: address: register address
 * 	         mask: (byte) '1' allows bit to be changed, '0' does not
 * 	         data: (byte) '1' sets to '1', '0' clears to '0'
 */
uint8_t bitModify(uint8_t address, uint8_t mask, uint8_t data);


/* Set the callback function when we receive a message
 * Input: a function pointer that takes a int and an array of int and
 *        returns void
 */
void setReceiveCallback0(void (*ReceiveCallbackArg)(uint8_t, uint8_t*, uint32_t));
void setReceiveCallback1(void (*ReceiveCallbackArg)(uint8_t, uint8_t*, uint32_t));

/* Set a filter & mask
 * Inputs: which filter to use, a mask, and a condition, the message
 * will be received in the buffer that meets the condition:
 * (mask & filter == RECEIVED_DATA & mask)
 *
 * Filters 0 and 1 filter into RX0 buffer
 * Filters 2, 3, 4, and 5 filter into RX1 buffer
 *
 * setReceiveCallback(...) will be replaced with setReceiveZeroCallback(...)
 * and setReceiveOneCallback(...)
 *
 *
 */

void setTheFilter(uint8_t address, uint32_t value);

// Some of our register config macros:
#define HS_CNF1_SJW 0x80
#define HS_CNF1_BRP 0x03

#define HS_CNF2_BTLMODE 0x80
#define HS_CNF2_SAM 0x00
#define HS_CNF2_PHSEG1 0x38
#define HS_CNF2_PRSEG 0x07

#define HS_CNF3_SOF 0x00
#define HS_CNF3_WAKFIL 0x00
#define HS_CNF3_PHSEG2 0x02

// MASK ADDRESSES
#define CAN_MASK_0       0x20 // RXM0SIDH, RXM0SIDL, RXM0EID8, RXM0EID0
#define CAN_MASK_1       0x24 // RXM1SIDH, RXM1SIDL, RXM1EID8, RXM1EID0

// FILTER ADDRESSES
#define CAN_FILTER_0     0x00 // RXF0SIDH, RXF0SIDL, RXF0EID8, RXF0EID0
#define CAN_FILTER_1     0x04 // RXF1SIDH, RXF1SIDL, RXF1EID8, RXF1EID0
#define CAN_FILTER_2     0x08 // RXF2SIDH, RXF2SIDL, RXF2EID8, RXF2EID0
#define CAN_FILTER_3     0x10 // RXF3SIDH, RXF3SIDL, RXF3EID8, RXF3EID0
#define CAN_FILTER_4     0x14 // RXF4SIDH, RXF4SIDL, RXF4EID8, RXF4EID0
#define CAN_FILTER_5     0x18 // RXF5SIDH, RXF5SIDL, RXF5EID8, RXF5EID0


// OUTPUT CONSTANTS
#define CAN_OK             (0)
#define CAN_FAILINIT       (1)
#define CAN_FAILTX         (2)
#define CAN_MSGAVAIL       (3)
#define CAN_NOMSG          (4)
#define CAN_CTRLERROR      (5)
#define CAN_GETTXBFTIMEOUT (6)
#define CAN_SENDMSGTIMEOUT (7)
#define CAN_FAIL       (0xff)


// Define MCP25625 register addresses
#define MCP_RXF0SIDH    0x00
#define MCP_RXF0SIDL    0x01
#define MCP_RXF0EID8    0x02
#define MCP_RXF0EID0    0x03
#define MCP_RXF1SIDH    0x04
#define MCP_RXF1SIDL    0x05
#define MCP_RXF1EID8    0x06
#define MCP_RXF1EID0    0x07
#define MCP_RXF2SIDH    0x08
#define MCP_RXF2SIDL    0x09
#define MCP_RXF2EID8    0x0A
#define MCP_RXF2EID0    0x0B
#define MCP_BFPCTRL     0x0C
#define MCP_TXRTSCTRL   0x0D
#define MCP_CANSTAT     0x0E
#define MCP_CANCTRL     0x0F

#define MCP_RXF3SIDH    0x10
#define MCP_RXF3SIDL    0x11
#define MCP_RXF3EID8    0x12
#define MCP_RXF3EID0    0x13
#define MCP_RXF4SIDH    0x14
#define MCP_RXF4SIDL    0x15
#define MCP_RXF4EID8    0x16
#define MCP_RXF4EID0    0x17
#define MCP_RXF5SIDH    0x18
#define MCP_RXF5SIDL    0x19
#define MCP_RXF5EID8    0x1A
#define MCP_RXF5EID0    0x1B
#define MCP_TEC         0x1C
#define MCP_REC         0x1D

#define MCP_RXM0SIDH    0x20
#define MCP_RXM0SIDL    0x21
#define MCP_RXM0EID8    0x22
#define MCP_RXM0EID0    0x23
#define MCP_RXM1SIDH    0x24
#define MCP_RXM1SIDL    0x25
#define MCP_RXM1EID8    0x26
#define MCP_RXM1EID0    0x27
#define MCP_CNF3        0x28
#define MCP_CNF2        0x29
#define MCP_CNF1        0x2A
#define MCP_CANINTE     0x2B
#define MCP_CANINTF     0x2C
#define MCP_EFLG        0x2D

#define MCP_TXB0CTRL    0x30
#define MCP_TXB1CTRL    0x40
#define MCP_TXB2CTRL    0x50
#define MCP_RXB0CTRL    0x60
#define MCP_RXB1CTRL    0x70

// Receive Buffer
#define MCP_RXB0SIDH    0x61
#define MCP_RXB0DLC     0x65
#define MCP_RXB0D0      0x66
#define MCP_RXB1SIDH    0x71
#define MCP_RXB1DLC     0x75
#define MCP_RXB1D0      0x76

#define MCP_RXBNDLC_DLC 0x0F

// Transmit Buffer
#define MCP_TXB0SIDH    0x31
#define MCP_TXB0D0      0x36
#define MCP_TXB1SIDH    0x41
#define MCP_TXB1D0      0x46
#define MCP_TXB2SIDH    0x51
#define MCP_TXB2D0      0x56

// CANCTRL Register Values
#define MCP_NORMAL      0x00
#define MODE_CONFIG     0x80
#define MCP_REQOP       0xE0

// BFPCTRL Bits
#define B1BFS    0x20
#define B0BFS    0x10
#define B1BFE    0x8
#define B0BFE    0x4
#define B1BFM    0x2
#define B0BFM    0x1

// TXRTSCTRL Bits
#define B1BFS    0x20
#define B0BFS    0x10
#define B1BFE    0x8
#define B0BFE    0x4
#define B1BFM    0x2
#define B0BFM    0x1

// CNF1
#define SJW      0xc0
#define BRP      0x3f


// CNF2
#define BTLMODE  0x80
#define SAM      0x40
#define PHSEG1   0x38
#define PRSEG    0x7

// CNF3
#define SOF      0x80
#define WAKFIL   0x40
#define PHSEG2   0x7

// CANINTE
#define MERRE    0x80
#define WAKIE    0x40
#define ERRIE    0x20
#define TX2IE    0x10
#define TX1IE    0x8
#define TX0IE    0x4
#define RX1IE    0x2
#define RX0IE    0x1

// CANINTF
#define MERRF    0x80
#define WAKIF    0x40
#define ERRIF    0x20
#define TX2IF    0x10
#define TX1IF    0x8
#define TX0IF    0x4
#define RX1IF    0x2
#define RX0IF    0x1

// EFLG
#define RX1OVR     (1<<7)
#define RX0OVR     (1<<6)
#define TXBO       (1<<5)
#define TXEP       (1<<4)
#define RXEP       (1<<3)
#define TXWAR      (1<<2)
#define RXWAR      (1<<1)
#define EWARN      (1<<0)
#define EFLG_ERRORMASK  (0xF8)

// TXBnCTRL
#define ABTF     (1<<6)
#define MLOA     (1<<5)
#define TXERR    (1<<4)
#define TXREQ    (1<<3)
#define TXP1     0x2
#define TXP0     0x1

// RXBnCTRL
#define RXM1     (1<<6)
#define RXM0     (1<<5)
#define RXRTR    (1<<3)
#define BUKT     (1<<2)
#define FILHIT0   0x1
#define FILHIT1   0x2
#define FILHIT2   0x4

// MCP SPI COMMAND
#define CAN_RESET             0xC0
#define CAN_READ              0x03
#define CAN_READ_RXBUF        0x90
#define CAN_WRITE             0x02
#define CAN_LOADTX            0x40
#define CAN_RTS               0x80
#define CAN_STATUS            0xA0
#define CAN_RX_STATUS         0xB0
#define CAN_BITMODIFY         0x05

#define CAN_READ_RXBUF_0      0x02
#define CAN_READ_RXBUF_1      0x06



// Clock rates, other macros,

#endif  //CAN_H
