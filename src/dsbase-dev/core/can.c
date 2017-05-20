
/*
 * mcp_can.c
 *
 *  Created on: Jan 22, 2017
 *      Author: UW Satellite Team
 */
#include "can.h"

// Dummy callback used in place until we set a
// callback method
void DummyCallback(uint8_t length, uint8_t* data){}

uint8_t canInit() {
    spiInit(BIT0);

	//Register values determined using online calculator.

	//step 1: reset CAN controller
    uint8_t rstData[1] = {CAN_RESET};
    spiTransceive(rstData, rstData, 1, BIT0);

	//step 2: initialize buffers, masks, and filters
    // a: CNF1, synchronization jump width length, baud rate prescaler
    //    ref: pg 45
    setRegister(MCP_CNF1, 0x47);
    // b: CNF2, bittime length bit, sample point config bit
    setRegister(MCP_CNF2, 0xBF);
    // c: CNF3, start of frame signal bit, wake up filter bit,
    setRegister(MCP_CNF3, 0x07);

    // Set the interrupt pin low when the following occurs
    // Interrupts for receive
    // TODO: Enable support for error on send
    bitModify(MCP_CANINTE, 0x3,0xFF);

	//step 3: set mode to normal
    bitModify(MCP_CANCTRL, 0xE0, 0x00);

    // step 4: check on REQOP<2:0> on CANCTRL register
    // a: verify normal mode by reading OPMOD bits in the CANCTRL register
    uint8_t canMode;
    readRegister(MCP_CANSTAT, &canMode);

    // Set a dummy callback
    setReceiveCallback(DummyCallback)
    return canMode;
}

uint8_t loadTxBuf(uint8_t address, uint8_t value)
{
    uint8_t sendBuf[2];
    switch (address)
    {
    case MCP_TXB0SIDH:
        sendBuf[0] = CAN_LOADTX;
        break;
    case MCP_TXB0D0:
        sendBuf[0] = CAN_LOADTX | 0x1;
        break;
    case MCP_TXB1SIDH:
        sendBuf[0] = CAN_LOADTX | 0x2;
        break;
    case MCP_TXB1D0:
        sendBuf[0] = CAN_LOADTX | 0x3;
        break;
    case MCP_TXB2SIDH:
        sendBuf[0] = CAN_LOADTX | 0x4;
        break;
    case MCP_TXB2D0:
        sendBuf[0] = CAN_LOADTX | 0x5;
        break;
    default:
        return CAN_FAIL;
    }
    sendBuf[1] =  value;
    spiTransceive(sendBuf, sendBuf, 2, BIT0);
    return 0;
}

uint8_t canSend(uint8_t bufNum, uint8_t* tech, uint8_t* msg) {
    uint8_t sendBuf[6];
    uint8_t sendBuf2[9];
    uint8_t txBuf;

    // Modifying bits for which buffer to write to.
    switch (bufNum)
        {
        case 0:
            sendBuf[0] = CAN_LOADTX;
            sendBuf2[0] = CAN_LOADTX | 0x01;
            txBuf = 1;
            break;
        case 1:
            sendBuf[0] = CAN_LOADTX | 0x2;
            sendBuf2[0] = CAN_LOADTX | 0x03;
            txBuf = 2;
            break;
        case 2:
            sendBuf[0] = CAN_LOADTX | 0x4;
            sendBuf2[0] = CAN_LOADTX | 0x05;
            txBuf = 4;
            break;
        default:
            return CAN_FAIL;
        }

    // 1 byte for the LoadTx instruction
    // 5 bytes for the settings TXB0SDIH to TXB0DLC
    int i = 0;
    for(i = 0; i < 5; i++) {
        sendBuf[i+1] = tech[i];
    }
    spiTransceive(sendBuf, sendBuf, 6, BIT0);

    // 1 byte for the LoadTX instruction
    // 8 bytes for the actual message.
    for (i = 0; i < 8; i++) {
       sendBuf2[i+1] = msg[i];
    }
    spiTransceive(sendBuf2, sendBuf2, 9, BIT0);

    // Request CAN to transmit buffer bufNum
    requestToSend(txBuf);
    return 0;
}

uint8_t readRegister(uint8_t address, uint8_t* value) {
    uint8_t sendBuf[3] = {CAN_READ, address, 0x00 };
    spiTransceive(sendBuf, sendBuf, 3, BIT0);
    *value = sendBuf[2];
    return 0;
}

uint8_t setRegister(uint8_t address, uint8_t value) {
    // CAN write
    uint8_t sendBuf[3];
    sendBuf[0] = CAN_WRITE;

    // CAN Write Address
    sendBuf[1] = address;

    // CAN Write Databyte
    sendBuf[2] = value;

    spiTransceive(sendBuf, sendBuf, 3, BIT0);
    return 0;
}

uint8_t bitModify(uint8_t address, uint8_t mask, uint8_t data) {
	uint8_t sendBuf[4];
	sendBuf[0] = CAN_BITMODIFY; // Bit Modify Instruction
	sendBuf[1] = address;
	sendBuf[2] = mask;
	sendBuf[3] = data;

	spiTransceive(sendBuf, sendBuf, 4, BIT0);
	return 0;
}

uint8_t requestToSend(uint8_t n) {
    uint8_t instruc = CAN_RTS;
    if (n > 7 ) {
        return CAN_FAIL;
    }
    instruc |= n;

    spiTransceive(&instruc, &instruc, 1, BIT0);
    return 0;
}

uint8_t readStatus(uint8_t *status) {
    uint8_t buf[2];
    buf[0] = CAN_STATUS;
    buf[1] = 0x00;

    spiTransceive(buf, buf, 2, BIT0);
    return 0;
}

uint8_t readRXStatus(uint8_t *status) {
    uint8_t buf[2];
    buf[0] = CAN_RX_STATUS;
    buf[1] = 0x00;

    spiTransceive(buf, buf, 2, BIT0);
    return 0;
}

void setReceiveCallback(void (*ReceiveCallbackArg)(uint8_t, uint8_t*)) {
    ReceiveCallback = ReceiveCallbackArg
}

// Interrupt handler for Receive Buffer
// PORT1_VECTOR defined in C:\ti\ccsv7\ccs_base\msp430\include\msp430fr5994.h
// GPIO port interrupts are all grouped together.
#pragma vector=PORT1_VECTOR
__interrupt void ReceivedMsg(void) {
    // TODO: Find which buffer got filled
    uint8_t status, rx0if, rx1if, res, length;
    readStatus(&status);

    rx0if = status & 0x01;
    rx1if = status & 1<<1;

    // Receive buffer 0 full
    if (rx0if) {
        // Standard Frame Remote Transmit Request Receive
        uint8_t rxb0dlc // rxb0sidl contains if extended or not extended can , rxb0dlc contains length of received message from bits 3:0
        res = readRegister(MCP_RXB0DLC, &rxb0dlc);

        if (res == 0) {
            length = rxb0dlc & 0x3;  // Data received in bytes

            // Read from receive buffer
            uint8_t buf[8 + 1];
            buf[0] = 0x92;
            spiTransceive(buf, buf, 9, BIT0);
            ReceiveCallback(length, buf);
        }

        bitModify(MCP_CANINTF, 0x1, 0x0);
    }

    // Receive buffer 1 full
    if (rx1if) {
        // Standard Frame Remote Transmit Request Receive
        uint8_t rxb1dlc // rxb0sidl contains if extended or not extended can , rxb0dlc contains length of received message from bits 3:0
        res = readRegister(MCP_RXB1DLC, &rxb1dlc);

        if (res == 0) {
            length = rxb0dlc & 0x3;  // Data received in bytes

            // Read from receive buffer
            uint8_t buf[8 + 1];
            buf[0] = 0x92;
            spiTransceive(buf, buf, 9, BIT0);
            ReceiveCallback(length, buf);
        }

        bitModify(MCP_CANINTF, 0x1, 0x0);
    }
}

