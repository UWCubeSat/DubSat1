
/*
 * mcp_can.c
 *
 *  Created on: Jan 22, 2017
 *      Author: UW Satellite Team
 */
#include "can.h"

void disableCanInterrupt(){
    P5IE &= ~BIT7;
}
void enableCanInterrupt(){
    P5IE |= BIT7;
}
uint8_t canRxErrorCheck(void){
    uint8_t retval = 0;
    // Disable CAN interrupts.
    disableCanInterrupt();
    // Read the Error Flag Register
    readRegister(MCP_EFLG, &retval);
    // Bit mask to only look at Rx Errors
    retval = retval & (RX1OVR | RX0OVR);
    // Reset Error Flag
    bitModify(MCP_EFLG, RX1OVR | RX0OVR, 0x0);
    // Reenable CAN interrupts.
    enableCanInterrupt();
    return retval;
}

uint8_t canTxCheck(void){
    uint8_t retval = 0;
    uint8_t bufferstatus = 0;
    // Disable CAN interrupts.
    disableCanInterrupt();
    // Read TXB0CTRL to see if Buffer 0 RTS is 1
    // That would mean we shouldn't overwrite this register.
    readRegister(MCP_TXB0CTRL, &bufferstatus);
    retval |= (bufferstatus & TXREQ) >> 3;
    // Read TXB1CTRL to see if Buffer 1 RTS is 1
    // That would mean we shouldn't overwrite this register.
    bufferstatus = 0;
    readRegister(MCP_TXB1CTRL, &bufferstatus);
    retval |= (bufferstatus & TXREQ) >> 2;
    // Read TXB2CTRL to see if Buffer 2 RTS is 1
    // That would mean we shouldn't overwrite this register.
    bufferstatus = 0;
    readRegister(MCP_TXB2CTRL, &bufferstatus);
    enableCanInterrupt();
    retval |= (bufferstatus & TXREQ) >> 1;
    return retval;
}


// Dummy callback used in place until we set a
// callback method
void DummyCallback(uint8_t length, uint8_t* data, uint32_t id){}

void setTheFilter(uint8_t address, uint32_t value){
    // Disable CAN interrupts (critical for maintaining 1 SPI transactions at a time rule)
    disableCanInterrupt();

    // Set mode to CFG
    bitModify(MCP_CANCTRL, MCP_REQOP, MODE_CONFIG);

    //Set The Registers
    setRegister(address, (uint8_t) (value >> 21));
    setRegister(address + 1, (uint8_t) (value >> 16) & 0x03 | (uint8_t) (value >> 13) & 0xE0 | 0x08);
    setRegister(address + 2, (uint8_t) (value >> 8));
    setRegister(address + 3, (uint8_t) value);

    //Set mode to Normal
    bitModify(MCP_CANCTRL, MCP_REQOP, MCP_NORMAL);

    // Re-enable CAN interrupts
    enableCanInterrupt();
}


uint8_t canInit() {
    spiInit(CS_1);

    //Register values determined using online calculator.

    //step 1: reset CAN controller
    uint8_t rstData[1] = {CAN_RESET};
    spiTransceive(rstData, rstData, 1, CS_1);

    //step 2: initialize buffers, masks, and filters
    // a: CNF1, synchronization jump width length, baud rate prescaler
    //    ref: pg 45
    setRegister(MCP_CNF1, HS_CNF1_SJW | HS_CNF1_BRP);
    // b: CNF2, bittime length bit, sample point config bit
    setRegister(MCP_CNF2, HS_CNF2_BTLMODE | HS_CNF2_SAM | HS_CNF2_PHSEG1 | HS_CNF2_PRSEG);
    // c: CNF3, start of frame signal bit, wake up filter bit,
    setRegister(MCP_CNF3, HS_CNF3_SOF | HS_CNF3_WAKFIL | HS_CNF3_PHSEG2);

    // Set the interrupt pin low when the following occurs
    // Interrupts for receive
    // Enable support for error on send
    bitModify(MCP_CANINTE, RX0IE | RX1IE, RX0IE | RX1IE);

    // Acceptance Filters only on extended identifiers
    bitModify(MCP_RXB0CTRL, RXM1 | RXM0, 0x00);
    bitModify(MCP_RXB1CTRL, RXM1 | RXM0, 0x00);

      //step 3: set mode to normal
    bitModify(MCP_CANCTRL, MCP_REQOP, MCP_NORMAL);

    // step 4: check on REQOP<2:0> on CANCTRL register
    // a: verify normal mode by reading OPMOD bits in the CANCTRL registe
    uint8_t canMode;
    readRegister(MCP_CANSTAT, &canMode);

    // Set a dummy callback
    setReceiveCallback0(DummyCallback);
    setReceiveCallback1(DummyCallback);

    //Set in Interrupt
    // TODO: Enable individual interrupt for Port 1
    //       pg 26 in Interrupts Workshop
    P5DIR |= BIT7;
    P5IE |= BIT7; // P5.7 interrupt enabled
    P5IES |= BIT7; // P5.7 Hi/lo edge
    P5IFG &= ~BIT7; // P5.7 IFG cleared
    P5DIR &= ~BIT7; // Set P5.7 to input direction
    // TODO: Need to set general interrupt enable (GIE) in status register(SR)
    __bis_SR_register(GIE);

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
    spiTransceive(sendBuf, sendBuf, 2, CS_1);
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

    // Send the ID to the MCP
    // 1 byte for the LoadTx instruction
    // 5 bytes for the settings TXB0SDIH to TXB0DLC
    int i = 0;
    for(i = 0; i < 5; i++) {
        sendBuf[i+1] = tech[i];
    }
    disableCanInterrupt();
    spiTransceive(sendBuf, sendBuf, 6, CS_1);

    // Send the Data to the MCP
    // 1 byte for the LoadTX instruction
    // 8 bytes for the actual message.
    for (i = 0; i < 8; i++) {
       sendBuf2[i+1] = msg[i];
    }
    spiTransceive(sendBuf2, sendBuf2, 9, CS_1);

    // Request CAN to transmit buffer bufNum
    requestToSend(txBuf);
    enableCanInterrupt();
    return 0;
}

uint8_t readRegister(uint8_t address, uint8_t* value) {
    uint8_t sendBuf[3] = {CAN_READ, address, 0x00 };
    spiTransceive(sendBuf, sendBuf, 3, CS_1);
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

    spiTransceive(sendBuf, sendBuf, 3, CS_1);
    return 0;
}

uint8_t bitModify(uint8_t address, uint8_t mask, uint8_t data) {
    uint8_t sendBuf[4];
    sendBuf[0] = CAN_BITMODIFY; // Bit Modify Instruction
    sendBuf[1] = address;
    sendBuf[2] = mask;
    sendBuf[3] = data;

    spiTransceive(sendBuf, sendBuf, 4, CS_1);
    return 0;
}

uint8_t requestToSend(uint8_t n) {
    uint8_t instruc = CAN_RTS;
    if (n > 7 ) {
        return CAN_FAIL;
    }
    instruc |= n;

    spiTransceive(&instruc, &instruc, 1, CS_1);
    return 0;
}

uint8_t readStatus(uint8_t *status) {
    uint8_t buf[2];
    buf[0] = CAN_STATUS;
    buf[1] = 0x00;

    spiTransceive(buf, buf, 2, CS_1);

    *status = buf[1];
    return 0;
}

uint8_t readRXStatus(uint8_t *status) {
    uint8_t buf[2];
    buf[0] = CAN_RX_STATUS;
    buf[1] = 0x00;

    spiTransceive(buf, buf, 2, CS_1);
    return 0;
}

void setReceiveCallback0(void (*ReceiveCallbackArg)(uint8_t, uint8_t*, uint32_t)) {
    ReceiveCallback0 = ReceiveCallbackArg;
}
void setReceiveCallback1(void (*ReceiveCallbackArg)(uint8_t, uint8_t*, uint32_t)) {
    ReceiveCallback1 = ReceiveCallbackArg;
}
// Interrupt handler for Receive Buffer
// PORT1_VECTOR defined in C:\ti\ccsv7\ccs_base\msp430\include\msp430fr5994.h
// GPIO port interrupts are all grouped together.
#pragma vector=PORT5_VECTOR
__interrupt void ReceivedMsg(void) {
    P5IFG &=~BIT7;
    disableCanInterrupt();
    uint8_t status, rx0if, rx1if, res, length;
    readStatus(&status);
    rx0if = status & 0x01;
    rx1if = status & 0x02;

        // Receive buffer 0 full
        if (rx0if) {
            uint8_t rxb0dlc; // rxb0sidl contains if extended or not extended can , rxb0dlc contains length of received message from bits 3:0
            res = readRegister(MCP_RXB0DLC, &rxb0dlc);

            if (res == 0) {
                // Data received in bytes
                length = rxb0dlc & MCP_RXBNDLC_DLC;

                // Read from receive buffer
                uint8_t buf[8 + 1], msg[8];

                // Read from receive buffer SPI instruction
                buf[0] = CAN_READ_RXBUF | CAN_READ_RXBUF_0;
                spiTransceive(buf, buf, 9, CS_1);

                // Read incoming message ID. This is spread over several
                // registers, hence all the bit masks and bitwise ops.
                uint32_t id = 0;
                uint8_t bufBuf = 0xFF;
                readRegister(MCP_RXB0SIDH, &bufBuf);
                id |= (uint32_t) bufBuf << 21;
                readRegister(MCP_RXB0SIDH + 1, &bufBuf);
                id |= (uint32_t) (bufBuf & 0xE0) << 13;
                id |= (uint32_t) (bufBuf & 0x03) << 16;
                readRegister(MCP_RXB0SIDH + 2, &bufBuf);
                id |= (uint32_t) bufBuf << 8;
                readRegister(MCP_RXB0SIDH + 3, &bufBuf);
                id |= bufBuf;
                // Clear out the interrupt flag for receive buffer 0
                bitModify(MCP_CANINTF, RX0IF, 0x0);
                int i = 0;
                for (i = 0; i < length; i++){
                    msg[i] = buf[i+1];
                }
                enableCanInterrupt();
                ReceiveCallback0(length, msg, id);            
              
            }
        }

    // Receive buffer 1 full
    if (rx1if) {
        disableCanInterrupt();
        uint8_t rxb1dlc;
        res = readRegister(MCP_RXB1DLC, &rxb1dlc);

            if (res == 0) {
                length = rxb1dlc & MCP_RXBNDLC_DLC;
                uint8_t buf[8 + 1], msg[8];
                buf[0] = CAN_READ_RXBUF | CAN_READ_RXBUF_0;
                spiTransceive(buf, buf, 9, CS_1);
                uint32_t id = 0;
                uint8_t bufBuf = 0xFF;
                readRegister(MCP_RXB1SIDH, &bufBuf);
                id |= (uint32_t) bufBuf << 21;
                readRegister(MCP_RXB1SIDH + 1, &bufBuf);
                id |= (uint32_t) (bufBuf & 0xE0) << 13;
                id |= (uint32_t) (bufBuf & 0x03) << 16;
                readRegister(MCP_RXB1SIDH + 2, &bufBuf);
                id |= (uint32_t) bufBuf << 8;
                readRegister(MCP_RXB1SIDH + 3, &bufBuf);
                id |= bufBuf;
                bitModify(MCP_CANINTF, RX1IF, 0x0);
                int i = 0;
                for (i = 0; i < length; i++){
                    msg[i] = buf[i+1];
                }
                enableCanInterrupt();
                ReceiveCallback1(length, msg, id);
        }
    }
    enableCanInterrupt();
}
